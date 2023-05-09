#ifndef ITINERARY_H__
#define ITINERARY_H__

#include "database_reader.h"
#include "coordinates.h"
#include "borne.h"
#include "etape.h"
#include "vehicules.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifndef SUPPRESS_PRINTFS
#define printf(...) printf(__VA_ARGS__)
#else
#define printf(...) (void)0
#endif

typedef struct position_point
{
    double distance_depart;
    double distance_arrivee;
    int indice_unique;
    int puissance;
} position_point;

typedef struct list_position
{
    struct list_position* next;
    position_point node;
    
} list_position;

etape* get_liste_etape_itineaire(long double latitude_depart, long double longitude_depart, long double latitude_arrivee, long double longitude_arrivee, voiture* one_car, int type, list_bornes_visitees* bornes_visitees);

etape* get_liste_etape_itineaire_type_distance(long double latitude_depart, long double longitude_depart, long double latitude_arrivee, long double longitude_arrivee, voiture* one_car, list_bornes_visitees* bornes_visitees);

etape* get_liste_etape_itineaire_type_temps(long double latitude_depart, long double longitude_depart, long double latitude_arrivee, long double longitude_arrivee, voiture* one_car, list_bornes_visitees* bornes_visitees);

list_position* getBorneFromDistance(long double latitude_depart,long double longitude_depart,long double latitude_arrivee,long double longitude_arrivee);

list_position* getBorneFromDistance_type_temps(long double latitude_depart,long double longitude_depart,long double latitude_arrivee,long double longitude_arrivee);

void add(list_position** list, double depart, double arrivee, int id_unique, int puissance);

void print_position(list_position* list);

list_position* list_create(void);

void list_destroy(list_position* one_list);

bool list_is_empty(list_position* one_list);

borne_and_distance plus_proche(list_position* one_list, double autonomie, list_bornes_visitees* bornes_visitees);

borne_and_distance plus_proche_type_temps(list_position* one_list, double autonomie, list_bornes_visitees* bornes_visitees, voiture* one_car);


borne getInfo(int i);


list_bornes_visitees* list_bornes_visitees_create(void);

#endif /* ITINERARY_H__ */
