#ifndef __SIMULATION_H__
#define __SIMULATION_H__

#include "vehicules.h"
#include "borne.h"

#ifndef SUPPRESS_PRINTFS
#define printf(...) printf(__VA_ARGS__)
#else
#define printf(...) (void)0
#endif

typedef struct trajet_simul
{
    int id_borne_depart;
    int id_borne_arrivee;
    voiture* vehicule;
} trajet_simul;


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
    int entier;
    struct list_int *next_value;
}list_int;

typedef struct list_int_head {
    list_int* head;
} list_int_head;

typedef struct export_data_el {
    coord_pt* coordonnees;
    int status_borne;
    struct export_data_el* next;
} export_data_el;

typedef struct export_data_list {
    export_data_el* head;
} export_data_list;

typedef struct export_data
{
    int total_entries;
    export_data_list* list_passages;
} export_data;

borne_simulation* load_bornes();

void simulation(borne_simulation* list_bornes, trajet_simul* list_trajet_simul, int nb_trajet_simul);

void destroy_tab(borne_simulation* tab);

int temps_trajet(borne_and_distance* proche);

int temps_recharge(voiture* voiture, borne* borne);

bool borne_in_ticks(list_int* tick_list, borne_and_distance* proche);

void add_borne(list_int* tick_list, borne_and_distance* proche);

void tab_tick_destroy(list_int* tab_tick);

void passage_list_destroy(passage_voiture_head* passage);

void ajout_passage(borne_simulation *list_bornes, int id_voiture, int tick, int duree_charge, borne *borne);

passage_voiture* creer_passage(int id_voiture, int status_passage, int places_restantes, int tick);

void passage_voiture_head_append(passage_voiture_head* one_list, int id_voiture, int status_passage, int places_restantes, int tick);
passage_voiture* passage_voiture_head_pop(passage_voiture_head* one_list);
passage_voiture* passage_voiture_head_pop_i(passage_voiture_head* one_list, int i);

int update_passage(passage_voiture_head* file_d_attente, int id_voiture, int difference);
int est_dans(export_data* data, coord_pt* coordonnes);
void list_int_append(list_int_head* one_list, int value);
void update_data(export_data* data, coord_pt* coordonnees, int status_borne);
int int_est_dans(list_int_head * one_list, int value);
void remove_list_int(list_int_head* one_list, int value);
void data_append(export_data* data, coord_pt* coordonnees, int status_passage);

void destroy_data(export_data* tab);
void destroy_data_list(export_data_el* one_data_list);

#endif /* __SIMULATION_H__*/
