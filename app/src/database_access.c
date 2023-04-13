#include "database_reader.h"
#include "coordinates.h"
#include "borne.h"
#include "database_access.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>



#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a,b)  ((a) < (b) ? (a) : (b))

#define LARGEUR_BANDE(a,b) (fabs(b-a))

list_position* getBorneFromDistance(long double latitude_depart,long double longitude_depart,long double latitude_arrivee,long double longitude_arrivee){
    list_position* retour = list_create();
    database_t * database = open_database("./../data/database.db");
    if (!database->opened_correctly) {
        exit(0);
    }
    char* sql_commmand = "SELECT consolidated_longitude, consolidated_latitude, id_unique FROM bornes WHERE (consolidated_longitude BETWEEN ?  AND ?) AND (consolidated_latitude BETWEEN ? and ?) ";

    prepare_request_database(database, sql_commmand); // préparation de la requête
    sqlite3_bind_double(database->stmt, 1, MIN(longitude_depart,longitude_arrivee));
    sqlite3_bind_double(database->stmt, 2, MAX(longitude_arrivee,longitude_depart));
    sqlite3_bind_double(database->stmt, 3, MIN(latitude_depart,latitude_arrivee));
    sqlite3_bind_double(database->stmt, 4,MAX(latitude_arrivee,latitude_depart)); // on bind la valeur de "?" dans la requête
    // printf("%f\n",LARGEUR_BANDE(longitude_arrivee,longitude_depart));

    int rc;
    long double longitude;
    long double latitude;
    long double distance_depart_borne;
    long double distance_borne_arrivee;
    int id_unique;
    int i = 0;
    while ((rc = sqlite3_step(database->stmt)) != SQLITE_DONE) { // pour chaque résultat
        longitude = sqlite3_column_double(database->stmt, 0); // on récupère les valeurs
        latitude = sqlite3_column_double(database->stmt, 1);
        id_unique = sqlite3_column_int(database->stmt, 2);
        // printf("%Lf / %Lf\n",longitude,latitude);
        // printf("%Lf / %Lf\n", longitude_depart,latitude_depart);

        distance_depart_borne = distance(longitude_depart,latitude_depart,longitude,latitude);
        distance_borne_arrivee = distance(longitude,latitude,longitude_arrivee,latitude_arrivee);
        // printf("%Lf / %Lf\n",distance_depart_borne,distance_borne_arrivee);
        add(&retour,distance_depart_borne,distance_borne_arrivee,id_unique);

            //  printf("%d\n",id_unique);
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
    long double max_distance = -1.0L;
    long double min_distance = 200000.0L;
    borne_and_distance selectionner;
    int ind = -1;
    list_position* i = one_list;
    while (i->next!=NULL)
    {
        // printf("Distance départ : %f / Distance arrivée : %f\n", i->node.distance_depart,i->node.distance_arrivee<min_distance);
        if(i->node.distance_depart<autonomie && i->node.distance_arrivee<min_distance){
            max_distance = i->node.distance_depart;
            min_distance = i->node.distance_arrivee;
            ind = i->node.indice_unique;
            
        }
        i = i->next;
    }
    // printf("selectionner : %d\n",ind);
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
   long double longitude;
   long double latitude;
   char* sql_commmand = "SELECT nom_station, consolidated_longitude, consolidated_latitude FROM bornes WHERE id_unique = ?";
   prepare_request_database(database,sql_commmand);
   sqlite3_bind_int(database->stmt, 1, i); // on bind la valeur de "?" dans la requête
   sqlite3_step(database->stmt);

   name = sqlite3_column_text(database->stmt, 0);
   longitude = sqlite3_column_double(database->stmt, 1);
   latitude = sqlite3_column_double(database->stmt,2);
//    printf("%s\n",name);
   char* nom_station = calloc((strlen(name)+1),sizeof(char));
   strcpy(nom_station,name);
   res.coordonnees.latitude = latitude;
   res.coordonnees.longitude = longitude;
   res.id = i;
   res.name = nom_station;
   end_request_database(database);
   close_database(database);
   return res;
}