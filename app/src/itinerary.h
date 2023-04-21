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

typedef struct position_point
{
    double distance_depart;
    double distance_arrivee;
    int indice_unique;
} position_point;

typedef struct list_position
{
    struct list_position* next;
    position_point node;
    
} list_position;

typedef struct list_bornes_visitees
{
    struct list_bornes_visitees* next;
    int id_borne_visitee;   //chaque borne a un identifiant unique

} list_bornes_visitees;

etape* get_liste_etape_itineaire(long double latitude_depart, long double longitude_depart, long double latitude_arrivee, long double longitude_arrivee, voiture* one_car, int type, list_bornes_visitees* bornes_visitees);

etape* get_liste_etape_itineaire_type_distance(long double latitude_depart, long double longitude_depart, long double latitude_arrivee, long double longitude_arrivee, voiture* one_car, list_bornes_visitees* bornes_visitees);

etape* get_liste_etape_itineaire_type_temps(long double latitude_depart, long double longitude_depart, long double latitude_arrivee, long double longitude_arrivee, voiture* one_car, list_bornes_visitees* bornes_visitees);

list_position* getBorneFromDistance(long double latitude_depart,long double longitude_depart,long double latitude_arrivee,long double longitude_arrivee, list_bornes_visitees* bornes_visitees);

void add(list_position** list, double depart, double arrivee, int id_unique);

void print_position(list_position* list);

list_position* list_create(void);

void list_destroy(list_position* one_list);

bool list_is_empty(list_position* one_list);

borne_and_distance plus_proche(list_position* one_list, double autonomie, list_bornes_visitees* bornes_visitees);


borne getInfo(int i);


list_bornes_visitees* list_bornes_visitees_create(void);

bool list_bornes_visitees_is_empty(list_bornes_visitees *one_list);

void list_bornes_visitees_destroy(list_bornes_visitees* one_list);

void list_bornes_visitees_append(list_bornes_visitees* one_list, int id_borne);

bool borne_deja_visitee(list_bornes_visitees* one_list, int id_borne);

void list_bornes_visitees_print(list_bornes_visitees *one_list);

#endif /* ITINERARY_H__ */
