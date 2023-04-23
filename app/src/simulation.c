#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "borne.h"
#include "vehicules.h"
#include "database_reader.h"
#include "coordinates.h"
#include "simulation.h"
#include "itinerary.h"
#include "etape.h"
// #include "borne.h"

borne_simulation* load_bornes(){
    //charge l'ensemble des bornes de la BD
    borne_simulation* tableau_bornes = (borne_simulation*) malloc(17319*sizeof(borne_simulation));
    database_t * database = open_database("../data/database_puissances_nominales.db");
    if (!database->opened_correctly) {
        exit(0);
    }

    // préparation de la requête
    char* sql_commmand = "SELECT consolidated_longitude, consolidated_latitude, id_unique, nom_station, puissance_nominale, nbre_pdc FROM bornes";
    prepare_request_database(database, sql_commmand); 
    
    int rc;
    int compteur = 0;
    while ((rc = sqlite3_step(database->stmt)) != SQLITE_DONE) { // pour chaque résultat
        tableau_bornes[compteur].coordonnees.longitude = sqlite3_column_double(database->stmt, 0);
        tableau_bornes[compteur].coordonnees.latitude = sqlite3_column_double(database->stmt, 1);
        //copie de la chaine de caractère
        char* name = (char*)sqlite3_column_text(database->stmt,3);
        char* nom_station = calloc((strlen(name)+1),sizeof(char));
        strcpy(nom_station,name);

        tableau_bornes[compteur].name = nom_station;
        tableau_bornes[compteur].id = sqlite3_column_int(database->stmt,2);
        tableau_bornes[compteur].puissance_nominale = sqlite3_column_int(database->stmt,4);
        tableau_bornes[compteur].capacite_max = sqlite3_column_int(database->stmt, 5);
        tableau_bornes[compteur].capacite_actuelle = tableau_bornes[compteur].capacite_max;
        //liste des passages des voitures initialisé
        tableau_bornes[compteur].list_passages = NULL;
        compteur++;
    }

    end_request_database(database);
    close_database(database);
    return tableau_bornes;
}


void simulation(borne_simulation* list_bornes, trajet* liste_trajet, int nombre_trajet){
    /*
    Actuellement la fonction établit l'ensemble des passages des voitures passsées en arguments
    Les changements sont directement effectués sur list_bornes

    Contraintes encore présentes:
    nombre max de tick estimé à 24h (24*6*10min)
    pas de changement d'itinéraire en fonction des places (aucun traitement des places effectué)
    pas de stockage des trajets des voitures, il doit être retrouvé
    les listes des passages ne sont pas triés en fonction des ticks croissants
    */
    int total_tick = 24*6; //
    list_int* tab_tick = (list_int*) malloc(total_tick*sizeof(list_int)); //tableau représentant quels bornes vont être changé à chaque tick -> evite un parcours de l'ensemble des bornes lors des ticks
    for (int i=0;i<total_tick;i++) {tab_tick->next_value = NULL;} //initialise l'ensemble des listes
    for (int trajet_i=0;trajet_i<nombre_trajet;trajet_i++) {
        trajet trajet_actuel = liste_trajet[trajet_i];
        int tick = 0;

        // Initialise la distance borne -> arrivée (au début borne = départ)
        long double longitude_depart = list_bornes[trajet_actuel.id_borne_depart].coordonnees.longitude;
        long double latitude_depart = list_bornes[trajet_actuel.id_borne_depart].coordonnees.longitude;
        long double longitude_arrivee = list_bornes[trajet_actuel.id_borne_arrivee].coordonnees.longitude;
        long double latitude_arrivee = list_bornes[trajet_actuel.id_borne_arrivee].coordonnees.longitude;
        double distance_fin = distance(longitude_depart,latitude_depart,longitude_arrivee,latitude_arrivee); 
        
        ////// etape* lst_etape = etape_create(); // valeur de retour
        borne_and_distance proche;
        list_bornes_visitees* bornes_visitees = list_bornes_visitees_create();
        bool arrivee = false;
        bool erreur = false;
        // Tant que la distance borne -> arrivé n'est pas nulle il reste au moins une étape
        while (distance_fin != 0.0 && !arrivee && !erreur)
        {
            if(distance_fin<trajet_actuel.vehicule->autonomie_actuelle){
                arrivee = true;
            } 
            else{
                // Calcul du point le proche de l'arrivée atteignable avec l'autonomie du véhicule en fonction du point traité
                list_position* resultat = getBorneFromDistance(latitude_depart,longitude_depart,latitude_arrivee,longitude_arrivee, bornes_visitees);
                if (list_is_empty(resultat))
                {
                    list_destroy(resultat);
                    erreur = true;
                }
                else {
                    proche = plus_proche(resultat,trajet_actuel.vehicule->autonomie_actuelle, bornes_visitees);
                    if (proche.borne.id == -1){
                        list_destroy(resultat);
                        erreur = true;
                    }
                    else {
                        //ajouter cette borne à la liste des bornes visitées
                        list_bornes_visitees_append(bornes_visitees, proche.borne.id);

                        //calcul du nombre de ticks pour le trajet et le temps de recharge
                        int tick_trajet = temps_trajet(&proche);
                        int tick_recharge = temps_recharge();

                        //arrivee à la borne
                        tick = tick + tick_trajet;
                        ajout_passage(list_bornes, trajet_actuel.vehicule, 1, tick, proche.borne.id);
                        //passage ajoute le moment où la voiture arrive à la borne
                        //composé du triplet (tick_d_arrivee, status(arrive ou quitte), vehicule)

                        if (!borne_in_ticks(&tab_tick[tick],&proche)) {
                            add_borne(&tab_tick[tick],&proche);
                        }

                        //depart de la borne
                        tick = tick + tick_recharge;
                        ajout_passage(list_bornes, trajet_actuel.vehicule, 0, tick, proche.borne.id);

                        if (!borne_in_ticks(&tab_tick[tick],&proche)) {
                            add_borne(&tab_tick[tick],&proche);
                        }

                        // printf("Distance parcourue %f km \n",proche.distance_debut);
                        update_charge(trajet_actuel.vehicule,proche.distance_debut);
                        recharge(trajet_actuel.vehicule,proche.borne.puissance_nominale);
                        // printf("La voiture a une autonomie de %f km\n",one_car->autonomie_actuelle);
                        // printf("La borne a une puissance de : %d kW\n",proche.borne.puissance_nominale);
                        // printf("----------------------------------------------------\n");

                        // Nouvelle distance de fin : borne_atteinte -> arrivée
                        distance_fin = distance(proche.borne.coordonnees.longitude,proche.borne.coordonnees.latitude,longitude_arrivee,latitude_arrivee);
                        latitude_depart = proche.borne.coordonnees.latitude;
                        longitude_depart = proche.borne.coordonnees.longitude;

                
                        // On détruit l'ancienne list_position
                        list_destroy(resultat);
                        free(proche.borne.name);
                    }
                }
                
            }

        }
        list_bornes_visitees_destroy(bornes_visitees); // détruit car non commun à toutes les voitures
    }
    tab_tick_destroy(tab_tick);
}

void ajout_passage(borne_simulation* list_bornes, voiture* one_car, int type_stationnement, int tick, int borneId){
    passage_voiture* new_passage = (passage_voiture*) malloc(sizeof(passage_voiture));
    new_passage->next_passage = NULL;
    new_passage->status_passage = type_stationnement;
    new_passage->tick = tick;
    new_passage->voiture = one_car;
    passage_voiture* list_vehicules = list_bornes[borneId].list_passages;
    if (list_vehicules == NULL) {
        list_vehicules = new_passage;
    }
    else {
        if (list_vehicules->next_passage == NULL && list_vehicules->tick <= new_passage->tick){ //s'il n'y a qu'un seul passage
            list_vehicules->next_passage = new_passage;
        }
        else if (list_vehicules->next_passage == NULL && list_vehicules->tick > new_passage->tick) {
            list_vehicules->next_passage = new_passage;
            // copie du premier élément dans le second
            new_passage->status_passage = list_vehicules->status_passage;
            new_passage->tick = list_vehicules->tick;
            new_passage->voiture = list_vehicules->voiture; //& possiblement obligatoire car list_vehicules->voiture va changer
            //remplacement du premier
            list_vehicules->status_passage = type_stationnement;
            list_vehicules->tick = tick;
            list_vehicules->voiture = one_car;
        }
        else {
            while (list_vehicules->next_passage != NULL && list_vehicules->tick <= new_passage->tick) {
                list_vehicules = list_vehicules->next_passage;
            }
            new_passage->next_passage = list_vehicules->next_passage;
            list_vehicules->next_passage = new_passage;
        }
    }
}

int temps_recharge() {
    return 2;
}

int temps_trajet(borne_and_distance* proche) {
    double duree = proche->distance_debut/100; //distance/vitesse
    int nombre_ticks = (int) round(duree/10);
    return nombre_ticks;
}

bool borne_in_ticks(list_int* tick_list, borne_and_distance* proche) {
    // vérifie si la borne proche est dans la liste des bornes de tick_list
    bool dedans = false;
    while (!dedans && tick_list->next_value != NULL) {
        if (tick_list->borne_id == proche->borne.id) {
            dedans = true;
        }
        tick_list = tick_list->next_value;
    }
    return dedans;
}

void add_borne(list_int* tick_list, borne_and_distance* proche) {
    list_int* new_borne = (list_int*) malloc(sizeof(list_int));
    new_borne->borne_id = proche->borne.id;
    new_borne->next_value = NULL;
    while (tick_list->next_value != NULL) {
        tick_list = tick_list->next_value;
    }
    tick_list->next_value = new_borne;
}

void destroy_tab(borne_simulation* tab){
    for (int i=0;i<17319;i++){
        free(tab[i].name);
        passage_list_destroy(tab[i].list_passages);
    }
    free(tab);
}

void tab_tick_destroy(list_int* tab_tick) {
    if (tab_tick != NULL) {
        if (tab_tick->next_value != NULL) {
            tab_tick_destroy(tab_tick->next_value);
            free(tab_tick);
        }
    }
}

void passage_list_destroy(passage_voiture* passages){
    if (passages != NULL) {
        if (passages->next_passage != NULL) {
            passage_list_destroy(passages->next_passage);
            free(passages);
        }
    }
}

int main(void) {
    //défition des trajets
    int nombre_trajets = 1;
    trajet* liste_trajet = (trajet*) malloc(nombre_trajets*sizeof(trajet));
    
    liste_trajet[0].id_borne_depart = 12;
    liste_trajet[0].id_borne_arrivee = 12000;

    voiture* voiture1 = (voiture*) malloc(sizeof(voiture));
    liste_trajet[0].vehicule = voiture1;

    //chargement bornes
    borne_simulation* tab_bornes = load_bornes();

    free(voiture1);
    free(liste_trajet);
    destroy_tab(tab_bornes);
    return 0;
}
