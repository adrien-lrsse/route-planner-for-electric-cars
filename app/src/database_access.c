#include "database_reader.h"
#include "coordinates.h"
#include "borne.h"
#include "database_access.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


list_position* getBorneFromDistance(double latitude_depart, double longitude_depart, double latitude_arrivee, double longitude_arrivee){
    list_position* retour = list_create();
    database_t * database = open_database("./../data/database.db");
    if (!database->opened_correctly) {
        exit(0);
    }
    char* sql_commmand = "SELECT consolidated_longitude, consolidated_latitude, id_unique FROM bornes";

    prepare_request_database(database, sql_commmand); // préparation de la requête
    
    int rc;
    double longitude;
    double latitude;
    double distance_depart_borne;
    double distance_borne_arrivee;
    int id_unique;
    int i = 0;
    while ((rc = sqlite3_step(database->stmt)) != SQLITE_DONE) { // pour chaque résultat
        longitude = sqlite3_column_double(database->stmt, 0); // on récupère les valeurs
        latitude = sqlite3_column_double(database->stmt, 1);
        id_unique = sqlite3_column_int(database->stmt, 2);
        distance_depart_borne = distance(longitude_depart,latitude_depart,longitude,latitude);
        distance_borne_arrivee = distance(longitude,latitude,longitude_arrivee,latitude_arrivee);
        // printf("%f / %f\n",distance_depart_borne,distance_borne_arrivee);
        add(&retour,distance_depart_borne,distance_borne_arrivee,id_unique);
        // printf("%d\n",i);
        i++;
    }
    end_request_database(database);
    close_database(database);
    return retour;
}

void add(list_position** list, double depart, double arrivee, int id_unique){
    if (list_is_empty(*list)){
        list_position* new_item = malloc(sizeof(list_position));
        new_item->next = NULL;
        new_item->node.indice_unique = id_unique;
        new_item->node.distance_depart = depart;
        new_item->node.distance_arrivee = arrivee;
        *list = new_item;
    }
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
    return NULL;
}

void list_destroy(list_position* one_list){
    list_position* i = one_list;
    while (i!=NULL){
        list_position* temp = i;
        i = i->next;
        free(temp);
    } 
}


bool list_is_empty(list_position* one_list){
    return (one_list == NULL);
}


borne_and_distance plus_proche(list_position* one_list, double autonomie){
    double max_distance = 0.0;
    double min_distance = 200000.0;
    borne_and_distance selectionner;
    int ind = -1;
    list_position* i = one_list;
    while (i->next!=NULL)
    {
        if(i->node.distance_depart<autonomie && i->node.distance_arrivee<min_distance){
            max_distance = i->node.distance_depart;
            min_distance = i->node.distance_arrivee;
            ind = i->node.indice_unique;
            
        }
        i = i->next;
    }
    selectionner.borne =  getInfo(ind);
    selectionner.distance_debut = max_distance;
    selectionner.distance_fin = min_distance;
    return selectionner;
    
}

borne getInfo(int i){
   borne res;
   database_t * database = open_database("./../data/database.db");
   if (!database->opened_correctly) {
       exit(0);
   }
   char* name;
   double longitude;
   double latitude;
   char* sql_commmand = "SELECT nom_station, consolidated_longitude, consolidated_latitude FROM bornes WHERE id_unique = ?";
   prepare_request_database(database,sql_commmand);
   sqlite3_bind_int(database->stmt, 1, i); // on bind la valeur de "?" dans la requête
   sqlite3_step(database->stmt);

   name = sqlite3_column_text(database->stmt, 0);
   longitude = sqlite3_column_double(database->stmt, 1);
   latitude = sqlite3_column_double(database->stmt,2);
   char* nom_station = malloc((strlen(name)+1)*sizeof(char));
   strcpy(nom_station,name);
   res.coordonnees.latitude = latitude;
   res.coordonnees.longitude = longitude;
   res.id = i;
   res.name = nom_station;
   end_request_database(database);
   close_database(database);
   return res;
}