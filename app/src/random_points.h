#ifndef RANDOM_POINTS_H__
#define RANDOM_POINTS_H__

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "coordinates.h"


typedef struct trajet{
    coord_pt* depart;
    coord_pt* arrivee;
    int id_voiture;
    int pourcentage_mini_voulu;
    int temps_max_attente_borne;
    int type;
    int pourcentage_autonomie_initiale;
    int thread;
}trajet;

typedef struct trajets_aleatoires{
    int taille; //nb de trajets aléatoires qu'on veut générer
    trajet* traj;
}trajets_aleatoires;

trajets_aleatoires* generate_x_random_itinerary(int nb_trajet);
void destroy_trajets_aleatoires(trajets_aleatoires* t);
void generate_x_numbers(int x, int* tab);
coord_pt* recuperation_coord_ligne(int num_ligne);
void all_random_parameters(trajets_aleatoires* trajet_existant);

#endif /* RANDOM_POINTS_H__ */
