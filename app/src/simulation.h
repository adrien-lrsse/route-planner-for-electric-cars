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
    int id_voiture;
    int status_passage;
    int places_restantes;
    int tick;
    struct passage_voiture* next_passage;
} passage_voiture;

typedef struct passage_voiture_head{
    passage_voiture* head;
} passage_voiture_head;

typedef struct borne_simulation
{
    coord_pt coordonnees;
    char* name;
    int id;
    int puissance_nominale;
    int capacite_actuelle;
    int capacite_max; 
    passage_voiture_head* list_passages;
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

int temps_recharge(voiture* voiture, borne* borne);

bool borne_in_ticks(list_int* tick_list, borne_and_distance* proche);

void add_borne(list_int* tick_list, borne_and_distance* proche);

void tab_tick_destroy(list_int* tab_tick);

void passage_list_destroy(passage_voiture* passage);

void ajout_passage(borne_simulation *list_bornes, int id_voiture, int tick, int duree_charge, borne *borne);

passage_voiture* creer_passage(int id_voiture, int status_passage, int places_restantes, int tick);

void passage_voiture_head_append(passage_voiture_head* one_list, int id_voiture, int status_passage, int places_restantes, int tick);
passage_voiture* passage_voiture_head_pop(passage_voiture_head* one_list);
passage_voiture* passage_voiture_head_pop_i(passage_voiture_head* one_list, int i);

#endif /* __SIMULATION_H__*/
