#include "database_reader.h"
#include "coordinates.h"
#include "borne.h"
#include "itinerary.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "etape.h"


#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a,b)  ((a) < (b) ? (a) : (b))

#define LARGEUR_BANDE(a,b) (fabs(b-a))

list_position* getBorneFromDistance(long double latitude_depart,long double longitude_depart,long double latitude_arrivee,long double longitude_arrivee){
    // Entrée : Coord  de départ et Coord d'arrivée
    // Sortie : Liste de toutes les bornes avec comme données : 
    // pour chaque borne i : (i : distance_départ_borne, distance_borne_arrivée)

    list_position* retour = list_create(); // Création de la liste de borne qui sera retourné.


    // Ouverture de la base de données
    database_t * database = open_database("./../data/database_clean.db");
    if (!database->opened_correctly) {
        exit(0);
    }

    // Préparation de la requête
    char* sql_commmand = "SELECT consolidated_longitude, consolidated_latitude, id_unique FROM bornes WHERE (consolidated_longitude BETWEEN ?  AND ?) AND (consolidated_latitude BETWEEN ? and ?) ";

    prepare_request_database(database, sql_commmand); // préparation de la requête
    sqlite3_bind_double(database->stmt, 1, MIN(longitude_depart,longitude_arrivee)); //bind des valeurs "?"
    sqlite3_bind_double(database->stmt, 2, MAX(longitude_arrivee,longitude_depart));
    sqlite3_bind_double(database->stmt, 3, MIN(latitude_depart,latitude_arrivee));
    sqlite3_bind_double(database->stmt, 4,MAX(latitude_arrivee,latitude_depart)); 


    // Déclaration des variables nécessaires pour la lecture des résultats de la requête
    int rc;
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


borne_and_distance plus_proche(list_position* one_list, double autonomie){
    // Entrée : liste de borne avec leur distance, autonomie du véhicule
    // Sortie : point le plus proche de l'arrivée qui respecte l'autonomie du véhicule
    long double max_distance = -1.0L;
    long double min_distance = 200000.0L;
    borne_and_distance selectionner;
    int ind = -1;
    list_position* i = one_list;
    while (i->next!=NULL)
    {
        // si on trouve un point plus proche de l'arrivée et qui ne nécessite pas plus de kilomètre que l'autonomie, alors c'est le nouveau point le proche
        if(i->node.distance_depart<autonomie && i->node.distance_arrivee<min_distance){
            max_distance = i->node.distance_depart;
            min_distance = i->node.distance_arrivee;
            ind = i->node.indice_unique;
            
        }
        i = i->next;
    }
    selectionner.borne =  getInfo(ind); // recupère les infos du points
    selectionner.distance_debut = max_distance;
    selectionner.distance_fin = min_distance;
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

    char* sql_commmand = "SELECT nom_station, consolidated_longitude, consolidated_latitude FROM bornes WHERE id_unique = ?";
    prepare_request_database(database,sql_commmand);
    sqlite3_bind_int(database->stmt, 1, i); // on bind la valeur de "?" dans la requête
    sqlite3_step(database->stmt);

    // Traitement de la sortie
    name =  (char*)sqlite3_column_text(database->stmt, 0); // nom de la station
    longitude = sqlite3_column_double(database->stmt, 1); // longitude de la station
    latitude = sqlite3_column_double(database->stmt,2); // latitude de la station

    // Copie de la chaine de caractères
    char* nom_station = calloc((strlen(name)+1),sizeof(char));
    strcpy(nom_station,name);
    res.name = nom_station;

    res.coordonnees.latitude = latitude;
    res.coordonnees.longitude = longitude;
    res.id = i;

    //Fermeture de la base
    end_request_database(database);
    close_database(database);

    return res;
}

etape* get_liste_etape_itineaire(long double latitude_depart,long double longitude_depart,long double latitude_arrivee,long double longitude_arrivee){
    // Entrée : Coordonnées de départ et d'arrivée'
    // Sortie : Liste des étapes pour atteindre l'arrivée

    // Initialise la distance borne -> arrivée (au début borne = départ)
    double distance_fin = distance(longitude_depart,latitude_depart,longitude_arrivee,latitude_arrivee); 
    
    etape* lst_etape = etape_create(); // valeur de retour
    borne_and_distance proche;

    // Tant que la distance borne -> arrivé n'est pas nul il reste au moins une étape
    while (distance_fin != 0.0)
    {
        // Calcul du point le proche de l'arrivée atteignable avec l'autonomie du véhicule en fonction du point traité
        list_position* resultat = getBorneFromDistance(latitude_depart,longitude_depart,latitude_arrivee,longitude_arrivee);
        proche = plus_proche(resultat,100);
        etape_add(lst_etape,proche);

        // Nouvelle distance de fin : borne_atteinte -> arrivée
        distance_fin = distance(proche.borne.coordonnees.longitude,proche.borne.coordonnees.latitude,longitude_arrivee,latitude_arrivee);
        latitude_depart = proche.borne.coordonnees.latitude;
        longitude_depart = proche.borne.coordonnees.longitude;

        // On détruit l'ancienne list_position
        list_destroy(resultat);
        free(proche.borne.name);
            
    } ;

    return lst_etape;
}
