#include "database_reader.h"
#include "coordinates.h"
#include "borne.h"
#include "vehicules.h"
#include "itinerary.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "etape.h"
#include <unistd.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a,b)  ((a) < (b) ? (a) : (b))

#define LARGEUR_BANDE(a,b) (fabsl(b-a))

list_position* getBorneFromDistance(long double latitude_depart,long double longitude_depart,long double latitude_arrivee,long double longitude_arrivee, list_bornes_visitees* bornes_visitees){
    // Entrée : Coord  de départ et Coord d'arrivée
    // Sortie : Liste de toutes les bornes avec comme données : 
    // pour chaque borne i : (i : distance_départ_borne, distance_borne_arrivée)

    list_position* retour = list_create(); // Création de la liste de borne qui sera retourné.


  

    
    // Ouverture de la base de données
    database_t * database = open_database("../data/database_clean.db");
    if (!database->opened_correctly) {
        exit(0);
    }
    char* sql_commmand;
    if (LARGEUR_BANDE(latitude_depart,latitude_arrivee) || LARGEUR_BANDE(longitude_arrivee,longitude_depart)){
        sql_commmand = "SELECT consolidated_longitude, consolidated_latitude, id_unique FROM bornes WHERE (consolidated_longitude BETWEEN (?-0.5)  AND (?+0.5)) AND (consolidated_latitude BETWEEN (?-0.5)  AND (?+0.5)) ";

    } else {
        sql_commmand = "SELECT consolidated_longitude, consolidated_latitude, id_unique FROM bornes WHERE (consolidated_longitude BETWEEN ?  AND ?) AND (consolidated_latitude BETWEEN ?  AND ?) ";

    }

    // Préparation de la requête

    prepare_request_database(database, sql_commmand); // préparation de la requête
    sqlite3_bind_double(database->stmt, 1, MIN(longitude_depart,longitude_arrivee)); //bind des valeurs "?"
    sqlite3_bind_double(database->stmt, 2, MAX(longitude_arrivee,longitude_depart));
    sqlite3_bind_double(database->stmt, 3, MIN(latitude_depart,latitude_arrivee));
    sqlite3_bind_double(database->stmt, 4,MAX(latitude_arrivee,latitude_depart)); 

    int rc;
    if ((rc = sqlite3_step(database->stmt)) == SQLITE_DONE)
    {
        printf("Pas de borne dans cette zone\n\n\n");
          end_request_database(database);
    close_database(database);
        return retour;
    }
    else
    {

    // Déclaration des variables nécessaires pour la lecture des résultats de la requête
    long double longitude;
    long double latitude;
    long double distance_depart_borne;
    long double distance_borne_arrivee;
    int id_unique;
    
    while ((rc = sqlite3_step(database->stmt)) != SQLITE_DONE) { // pour chaque résultat

        //Lecture du résultat de la requête
        longitude = sqlite3_column_double(database->stmt, 0); // la longitude
        latitude = sqlite3_column_double(database->stmt, 1); // la latitude
        id_unique = sqlite3_column_int(database->stmt, 2); // l'identifiant unique
        // Traitement
        distance_depart_borne = distance(longitude_depart,latitude_depart,longitude,latitude);  // calcul de la distance départ -> borne
        distance_borne_arrivee = distance(longitude,latitude,longitude_arrivee,latitude_arrivee); // calcul de la distance borne -> arrivée
        add(&retour,distance_depart_borne,distance_borne_arrivee,id_unique); // ajout des résultat de la borne i à la liste de retour
    }
    // Fermeture de la base

    end_request_database(database);
    close_database(database);
    return retour;
        
    }
}

void add(list_position** list, double depart, double arrivee, int id_unique){
    // Entrée : une liste, la distance départ -> borne, la distance borne -> arrivée, l'id de la borne en question.
    // Sortie : ajoute de l'élement (id, départ->borne, borne->arrivé) à list

    // cas ou la liste est vide
    if (list_is_empty(*list)){
        list_position* new_item = malloc(sizeof(list_position));
        new_item->next = NULL;
        new_item->node.indice_unique = id_unique;
        new_item->node.distance_depart = depart;
        new_item->node.distance_arrivee = arrivee;
        *list = new_item;
    }
    // cas ou la liste est non vide
    else {
        list_position* i = *list;
        while(i->next != NULL){
            i = i->next;
        }
        list_position* new_item = malloc(sizeof(list_position));
        new_item->next = NULL;
        new_item->node.indice_unique = id_unique;
        new_item->node.distance_depart = depart;
        new_item->node.distance_arrivee = arrivee;
        i->next = new_item;
    }
    
}


void print_position(list_position* list){
    list_position* i = list;
    while(i->next != NULL){
        printf("%d : distance_depart : %f / distance_arrive : %f\n", i->node.indice_unique,i->node.distance_depart,i->node.distance_arrivee);
        i = i->next;
    }   
}

list_position* list_create(void){
    // création d'une liste
    return NULL;
}

void list_destroy(list_position* one_list){
    // permet de free les adresses allouées pour one_list
    list_position* i = one_list;
    while (i!=NULL){
        list_position* temp = i;
        i = i->next;
        free(temp);
    } 
}


bool list_is_empty(list_position* one_list){
    // Entrée : une liste de type list_position
    // Permet de savoir si une liste est vide ou non
    // Sortie : un booléen
    return (one_list == NULL);
}


list_bornes_visitees* list_bornes_visitees_create(void){
    //creation d'une liste
    list_bornes_visitees* new_list=malloc(sizeof(list_bornes_visitees));
    new_list->next = NULL;
    return new_list;
}

bool list_bornes_visitees_is_empty(list_bornes_visitees *one_list){
    //return true si la one_list est vide, false sinon
    if (one_list->next==NULL){
        return true;
    }else{
        return false;
    }
}

void list_bornes_visitees_destroy(list_bornes_visitees* one_list){
    //free les adresses allouées pour one_list
    if (list_bornes_visitees_is_empty(one_list)==1){
        free(one_list);
    }
    else{
        list_bornes_visitees_destroy(one_list->next);
        free(one_list);
    }
}

void list_bornes_visitees_append(list_bornes_visitees* one_list, int id_borne){
    //ajoute l'identifiant d'une borne (id_borne) à la fin de la liste des bornes visitées (one_list)
    list_bornes_visitees* last_elt = one_list;
    while (list_bornes_visitees_is_empty(last_elt)==false){
        last_elt=last_elt->next;
    }
    list_bornes_visitees* tail = list_bornes_visitees_create();
    last_elt->id_borne_visitee = id_borne;
    last_elt->next = tail;
}

bool borne_deja_visitee(list_bornes_visitees* one_list, int id_borne){
    //return true si one_list contient id_borne, false sinon
    //vérifie si une borne a déjà été visitée
    
    list_bornes_visitees* liste = one_list;
    while (list_bornes_visitees_is_empty(liste) == 0){
        if (liste->id_borne_visitee == id_borne){
            return true;
        }
        liste = liste->next;
    }
    return false;
}

void list_bornes_visitees_print(list_bornes_visitees *one_list){
    list_bornes_visitees* tmp = one_list;
    printf("bornes_visitees:[");
    while (list_bornes_visitees_is_empty(tmp)==0){
        printf("%d ,\n", tmp->id_borne_visitee);
        tmp=tmp->next;
    }
    printf("]\n");
}


borne_and_distance plus_proche(list_position* one_list, double autonomie, list_bornes_visitees* bornes_visitees){
    // Entrée : liste de borne avec leur distance, autonomie du véhicule
    // Sortie : point le plus proche de l'arrivée qui respecte l'autonomie du véhicule
    long double max_distance = -1.0L;
    long double min_distance = 200000.0L;
    borne_and_distance selectionner;
    int ind = -1;
    list_position* i = one_list;
    while (i->next!=NULL)
    {
        // si on trouve un point plus proche de l'arrivée et qui ne nécessite pas plus de kilomètre que l'autonomie, et qui n'a pas déjà été visité, alors c'est le nouveau point le proche
        if(i->node.distance_depart<autonomie && i->node.distance_arrivee<min_distance && borne_deja_visitee(bornes_visitees, i->node.indice_unique)==0){
            max_distance = i->node.distance_depart;
            min_distance = i->node.distance_arrivee;
            ind = i->node.indice_unique;
            
        }
        i = i->next;
    }
    if (ind == -1){
        selectionner.borne.id = -1;
    }
    else {
    selectionner.borne =  getInfo(ind); // recupère les infos du points
    selectionner.distance_debut = max_distance;
    selectionner.distance_fin = min_distance;
    }
    return selectionner;
    
}

borne getInfo(int i){
    // Entrée : Identifiant unique d'une borne
    // Sortie : Information sur la borne i
    borne res;

    // Ouverture de la base
    database_t * database = open_database("./../data/database.db");
    if (!database->opened_correctly) {
        exit(0);
    }

    // Préparation de la requête
    char* name;
    long double longitude;
    long double latitude;
    int puissance_nominale;

    char* sql_commmand = "SELECT nom_station, consolidated_longitude,consolidated_latitude,puissance_nominale FROM bornes WHERE id_unique = ?";
    prepare_request_database(database,sql_commmand);
    sqlite3_bind_int(database->stmt, 1, i); // on bind la valeur de "?" dans la requête
    sqlite3_step(database->stmt);

    // Traitement de la sortie
    name =  (char*)sqlite3_column_text(database->stmt, 0); // nom de la station
    longitude = sqlite3_column_double(database->stmt, 1); // longitude de la station
    latitude = sqlite3_column_double(database->stmt,2); // latitude de la station
    puissance_nominale = sqlite3_column_int(database->stmt,3); // puissance nominale de la station

    // Copie de la chaine de caractères
    char* nom_station = calloc((strlen(name)+1),sizeof(char));
    strcpy(nom_station,name);
    res.name = nom_station;

    res.coordonnees.latitude = latitude;
    res.coordonnees.longitude = longitude;
    res.id = i;
    res.puissance_nominale = puissance_nominale;

    //Fermeture de la base
    end_request_database(database);
    close_database(database);

    return res;
}

etape* get_liste_etape_itineaire(long double latitude_depart, long double longitude_depart, long double latitude_arrivee, long double longitude_arrivee, voiture* one_car, int type, list_bornes_visitees* bornes_visitees){
    /*
    Arguments : 
        - latitude_depart : latitude du point de départ
        - longitude_depart : longitude du point de départ
        - latitude_arrivee : latitude du point d'arrivée
        - longitude_arrivee : longitude du point d'arrivée
        - autonomie : autonomie du véhicule
        - temps_max_attente_borne : temps maximum d'attente à une borne
        - type : 1 pour distance, 2 pour temps
        - bornes_visitees: liste des bornes déjà visitées par l'algorithme
    */
    if (type == 1){
        return get_liste_etape_itineaire_type_distance(latitude_depart, longitude_depart, latitude_arrivee, longitude_arrivee, one_car, bornes_visitees);
    }
    else if (type == 2){
        return get_liste_etape_itineaire_type_temps(latitude_depart, longitude_depart, latitude_arrivee, longitude_arrivee, one_car, bornes_visitees);
    }
    else {
        return NULL;
    }
}

etape* get_liste_etape_itineaire_type_distance(long double latitude_depart,long double longitude_depart,long double latitude_arrivee,long double longitude_arrivee,voiture* one_car, list_bornes_visitees* bornes_visitees){
    // Entrée : Coordonnées de départ et d'arrivée'
    // Sortie : Liste des étapes pour atteindre l'arrivée

    // Initialise la distance borne -> arrivée (au début borne = départ)
    double distance_fin = distance(longitude_depart,latitude_depart,longitude_arrivee,latitude_arrivee); 
    
    etape* lst_etape = etape_create(); // valeur de retour
    borne_and_distance proche;
    bool arrivee = false;
    // Tant que la distance borne -> arrivé n'est pas nulle il reste au moins une étape
    while (distance_fin != 0.0 && !arrivee)
    {
        if(distance_fin<one_car->autonomie_actuelle){
            arrivee = true;
        } 
        else{
            // Calcul du point le proche de l'arrivée atteignable avec l'autonomie du véhicule en fonction du point traité
            list_position* resultat = getBorneFromDistance(latitude_depart,longitude_depart,latitude_arrivee,longitude_arrivee, bornes_visitees);
            if (list_is_empty(resultat))
            {
                list_destroy(resultat);
                etape_destroy(lst_etape);
                return etape_create();

            }
            else {
                proche = plus_proche(resultat,one_car->autonomie_actuelle, bornes_visitees);
                if (proche.borne.id == -1){
                    list_destroy(resultat);
                    etape_destroy(lst_etape);
                    return etape_create();
                }
                else {
                    etape_add(lst_etape,proche);

                    //ajouter cette borne à la liste des bornes visitées
                    list_bornes_visitees_append(bornes_visitees, proche.borne.id);

                    // printf("Distance parcourue %f km \n",proche.distance_debut);
                    update_charge(one_car,proche.distance_debut);
                    recharge(one_car,proche.borne.puissance_nominale);
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

    return lst_etape;
}



etape* get_liste_etape_itineaire_type_temps(long double latitude_depart, long double longitude_depart, long double latitude_arrivee, long double longitude_arrivee, voiture* one_car, list_bornes_visitees* bornes_visitees){
    return get_liste_etape_itineaire_type_distance(latitude_depart, longitude_depart, latitude_arrivee, longitude_arrivee, one_car, bornes_visitees);
}
