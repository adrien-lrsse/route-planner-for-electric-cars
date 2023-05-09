#ifndef __SIMULATION_H__
#define __SIMULATION_H__

#include "borne.h"
#include "vehicules.h"

#ifndef SUPPRESS_PRINTFS
#define printf(...) printf(__VA_ARGS__)
#else
#define printf(...) (void)0
#endif

typedef struct trajet
{
    int id_borne_depart;
    int id_borne_arrivee;
    voiture* vehicule;
} trajet;


typedef struct passage_voiture{
    voiture* voiture;
    int status_passage;
    int tick;
    struct passage_voiture* next_passage;
} passage_voiture;

typedef struct borne_simulation
{
    coord_pt coordonnees;
    char* name;
    int id;
    int puissance_nominale;
    int capacite_actuelle;
    int capacite_max; 
    passage_voiture* list_passages;
} borne_simulation;

typedef struct list_int
{
    int borne_id;
    struct list_int *next_value;
}list_int;


borne_simulation* load_bornes();

void simulation(borne_simulation* list_bornes, trajet* list_trajet, int nb_trajet);

void destroy_tab(borne_simulation* tab);

int temps_trajet(borne_and_distance* proche);

int temps_recharge();

bool borne_in_ticks(list_int* tick_list, borne_and_distance* proche);

void add_borne(list_int* tick_list, borne_and_distance* proche);

void tab_tick_destroy(list_int* tab_tick);

void passage_list_destroy(passage_voiture* passage);

void ajout_passage(borne_simulation* list_bornes, voiture* one_car, int type_stationnement, int tick, int borneId);

#endif /* __SIMULATION_H__*/
