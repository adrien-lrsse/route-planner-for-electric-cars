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
    database_t * database = open_database("../data/database.db");
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
                list_position* resultat = getBorneFromDistance(latitude_depart,longitude_depart,latitude_arrivee,longitude_arrivee);
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
                        int tick_recharge = temps_recharge(trajet_actuel.vehicule,&proche.borne);

                        //arrivee à la borne
                        tick = tick + tick_trajet;
                        ajout_passage(list_bornes, trajet_actuel.vehicule, tick, &proche.borne);
                        //passage ajoute le moment où la voiture arrive à la borne
                        //composé du triplet (tick_d_arrivee, status(arrive ou quitte), vehicule)

                        if (!borne_in_ticks(&tab_tick[tick],&proche)) {
                            add_borne(&tab_tick[tick],&proche);
                        }

                        //depart de la borne
                        tick = tick + tick_recharge;
                        // ajout_passage(list_bornes, trajet_actuel.vehicule, 0, tick, proche.borne.id);

                        if (!borne_in_ticks(&tab_tick[tick],&proche)) {
                            add_borne(&tab_tick[tick],&proche);
                        }

                        update_charge(trajet_actuel.vehicule,proche.distance_debut);
                        recharge(trajet_actuel.vehicule,proche.borne.puissance_nominale);

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

void ajout_passage(borne_simulation* list_bornes, int id_voiture, int tick, int duree_charge, borne* borne){
    // le tick en paramètre est le tick auquel la voiture arrive à la station

    int tick_recharge = tick+duree_charge;
    int borneId = borne->id;

    passage_voiture_head* file_d_attente = (passage_voiture_head*) malloc(sizeof(passage_voiture_head));

    passage_voiture_head_append(file_d_attente, id_voiture, 1, list_bornes[borneId].capacite_max-1, tick);
    passage_voiture_head_append(file_d_attente, id_voiture, 0, list_bornes[borneId].capacite_max, tick+tick_recharge);

    passage_voiture_head* list_passages_head = list_bornes[borneId].list_passages;//pointe la structure
    passage_voiture* current = list_passages_head->head;//1er élément

    passage_voiture* tampon = creer_passage(0,0,0,0);

    int nombre_places_restantes;

    if (list_passages_head == NULL) {
        list_passages_head->head = passage_voiture_head_pop(file_d_attente);
        list_passages_head->head->next_passage = passage_voiture_head_pop(file_d_attente);
        free(tampon);
        free(file_d_attente);
        return;
    }

    passage_voiture* passage_file_attente = file_d_attente->head;
    int passage_ajoute = 0;
    int compteur = 0;

    if (passage_file_attente->tick <= current->tick) { 
        tampon->next_passage = passage_voiture_head_pop(file_d_attente);
        tampon->next_passage->next_passage = current;
        list_passages_head->head = tampon->next_passage;
        tampon->next_passage = NULL; //useless but just in case
        passage_file_attente = file_d_attente->head;
    }
    while (current->next_passage != NULL && file_d_attente->head != NULL){
        if (current->places_restantes == -1 && current->status_passage == 1) {//si une voiture est arrivée et qu'elle n'a pas de places (diapo 31-32)
            current->status_passage = 2;
            current->places_restantes = 0;
            passage_voiture_head_append(file_d_attente,current->id_voiture,3,-1,current->tick);
        }

        switch (current->next_passage->status_passage) {//mets à jour le nombre de places d'après
        case 1:
            current->next_passage->places_restantes = current->places_restantes-1;
            break;
        
        case 0:
            current->next_passage->places_restantes = current->places_restantes+1;
            break;

        default:
            break;
        }

        passage_file_attente = file_d_attente->head;// réinitialise le pointeur à la tête de la file d'attente
        passage_ajoute = 0; //booleen indiquant si un passage a été ajouté dans la liste
        compteur = 0;
        while (passage_file_attente->next_passage != NULL) { //regarde chaque élement de la file d'attente
            if (passage_ajoute == 0) { // distinguer le cas où un élément a été ajouté et traiter les cas de modification de la liste
            if (current->next_passage->tick >= passage_file_attente->tick && passage_file_attente->status_passage == 0) {//si l'élément dans la file d'attente a un tick plus faible que le prochain élément de la liste et que la voiture doit partir (diapo 10)
                tampon->next_passage = passage_voiture_head_pop_i(file_d_attente, compteur);
                tampon->next_passage->next_passage = current->next_passage;
                current->next_passage = tampon->next_passage;
                tampon->next_passage->places_restantes = current->places_restantes+1; // actualise les places de l'élément ajouté (car la modification de parcours est faite avant le traitement des élements de la file)
                tampon->next_passage = NULL; //useless but in case
                passage_ajoute = 1;
            }
            else if (current->next_passage->tick >= passage_file_attente->tick && passage_file_attente->status_passage == 1 && current->places_restantes > 0) {//si l'élément de la file d'attente a un tick plus faible, que la voiture arrive et qu'il reste de la place (diapo 36-37)
                tampon->next_passage = passage_voiture_head_pop_i(file_d_attente, compteur);
                tampon->next_passage->next_passage = current->next_passage;
                current->next_passage = tampon->next_passage;
                tampon->next_passage->places_restantes = current->places_restantes-1;
                tampon->next_passage = NULL;
                passage_ajoute = 1;
            }
            else if (passage_file_attente->status_passage == 3 && current->places_restantes > 0) {
                tampon->next_passage = passage_voiture_head_pop_i(file_d_attente, compteur);
                tampon->next_passage->next_passage = current->next_passage;
                tampon->next_passage->status_passage = 1; //retour de 3 à 1 pour le type de passage
                if (update_passage(file_d_attente,tampon->next_passage->id_voiture,current->tick-tampon->next_passage->tick) == 0) {//si la valeur de retour est 0, cela veut dire que le passage de sortie n'est pas dans la file d'attente et qu'il faudra le supprimer de la liste des passages (diapo 39)
                    passage_voiture_head_append(file_d_attente, tampon->next_passage->id_voiture, 4, -1, current->tick-tampon->next_passage->tick);
                }
                tampon->next_passage->tick = current->tick;
                current->next_passage = tampon->next_passage;
                tampon->next_passage->places_restantes = current->places_restantes-1;
                tampon->next_passage = NULL;
                passage_ajoute = 1;
            }
            }

            //tests qui devront toujours avoir lieu même si un passage a été ajouté
            if (current->next_passage->tick >= passage_file_attente->tick && passage_file_attente->status_passage == 1 && current->places_restantes == 0) {//une voiture arrive mais il n'y plus de place
                tampon->next_passage = creer_passage(passage_file_attente->id_voiture, 2, 0, current->tick);
                tampon->next_passage->next_passage = current->next_passage;
                current->next_passage = tampon->next_passage;
                tampon->next_passage = NULL;
                passage_file_attente->status_passage = 3;
            }
            else if (passage_file_attente->status_passage == 4 && current->next_passage->status_passage == 0 && current->next_passage->id_voiture == passage_file_attente->id_voiture) {//si une voiture doit partir mais le tick de sortie n'est pas le bon (décalé)
                passage_file_attente->status_passage = 0;
                passage_file_attente->tick = passage_file_attente->tick + current->next_passage->tick;
                tampon->next_passage = current->next_passage;
                current->next_passage = current->next_passage->next_passage;
                free(tampon->next_passage);
                tampon->next_passage = NULL;
                switch (current->next_passage->status_passage) {//mets à jour le nombre de places d'après
                case 1:
                    current->next_passage->places_restantes = current->places_restantes-1;
                    break;
                
                case 0:
                    current->next_passage->places_restantes = current->places_restantes+1;
                    break;

                default:
                    break;
                }
            }
            passage_file_attente = passage_file_attente->next_passage;
            compteur++;
        }
        current = current->next_passage;
    }

    free(tampon);
    free(file_d_attente);
}

int update_passage(passage_voiture_head* file_d_attente, int id_voiture, int difference){
    //décale tous les passages de la voiture id_voiture
    if (file_d_attente->head == NULL) {return 0;}
    passage_voiture* current = file_d_attente->head;
    int present = 0;
    while (current->next_passage != NULL) {
        if (current->id_voiture == id_voiture) {
            current->tick = current->tick+difference; 
            present = 1;
        }
        current = current->next_passage;
    }
    if (current->id_voiture == id_voiture) {
        current->tick = current->tick+difference;
        present = 1;
    }
    return present;
}

passage_voiture* creer_passage(int id_voiture, int status_passage, int places_restantes, int tick){
    passage_voiture* new_passage = (passage_voiture*) malloc(sizeof(passage_voiture));
    new_passage->next_passage = NULL;
    new_passage->status_passage = status_passage;
    new_passage->places_restantes = places_restantes;
    new_passage->tick = tick;
    new_passage->id_voiture = id_voiture;
    return new_passage;
}

void passage_voiture_head_append(passage_voiture_head* one_list, int id_voiture, int status_passage, int places_restantes, int tick) {
    passage_voiture* new_passage = creer_passage(id_voiture, status_passage, places_restantes, tick);
    passage_voiture* current = one_list->head;
    while (current->next_passage != NULL){
       current = current->next_passage;
    }
    current->next_passage = new_passage;    
}

passage_voiture* passage_voiture_head_pop(passage_voiture_head* one_list){
    passage_voiture* first_element = one_list->head;
    if (first_element->next_passage == NULL){
        one_list->head = NULL;
    }
    else {
        one_list->head = first_element->next_passage;
    }
    first_element->next_passage = NULL;
    return first_element;
}

passage_voiture* passage_voiture_head_pop_i(passage_voiture_head* one_list, int i){
    passage_voiture* current = one_list->head;
    passage_voiture* retour = one_list->head;
    if (i == 0) {
        retour = passage_voiture_head_pop(one_list);
        return retour;
    }
    int compteur = 0;
    while (current->next_passage != NULL && compteur < i-1) {
        compteur++;
        current = current->next_passage;
    }
    retour = current->next_passage;
    current->next_passage = current->next_passage->next_passage;
    return retour;
}


int temps_recharge(voiture* voiture, borne* borne) {
    int puissance_a_charger = voiture->puissance-voiture->puissance_actuelle;
    int temps = (int) puissance_a_charger/borne->puissance_nominale;
    int nombre_tick;
    if (temps >= voiture->temps_recharge_max_minutes) {
        nombre_tick = (int) round(voiture->temps_recharge_max_minutes/10);
    }
    else {
        nombre_tick = (int) round(temps/10);
    }
    return nombre_tick;
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
    //définition des trajets
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
