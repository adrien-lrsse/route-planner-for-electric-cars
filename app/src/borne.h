#ifndef BORNE_H__
#define BORNE_H__

#include "coordinates.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifndef SUPPRESS_PRINTFS
#define printf(...) printf(__VA_ARGS__)
#else
#define printf(...) (void)0
#endif

typedef struct borne
{
    coord_pt coordonnees;
    char* name;
    int id;
    int puissance_nominale;
} borne;

typedef struct borne_and_distance
{
    borne borne;
    int tick_recharge;
    double distance_debut;
    double distance_fin; // Distance en prenant en compte la recharge de la voiture !! (distance à la fin - autonomie après recharge)
} borne_and_distance;

typedef struct list_bornes_visitees
{
    struct list_bornes_visitees* next;
    int id_borne_visitee;   //chaque borne a un identifiant unique

} list_bornes_visitees;

list_bornes_visitees* list_bornes_visitees_create(void);

bool list_bornes_visitees_is_empty(list_bornes_visitees *one_list);

void list_bornes_visitees_destroy(list_bornes_visitees* one_list);

void list_bornes_visitees_append(list_bornes_visitees* one_list, int id_borne);

bool borne_deja_visitee(list_bornes_visitees* one_list, int id_borne);

void list_bornes_visitees_print(list_bornes_visitees *one_list);

#endif /* BORNE_H__ */
