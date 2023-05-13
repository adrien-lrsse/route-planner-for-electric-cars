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
}trajet;

typedef struct trajets_aleatoires{
    int taille; //nb de trajets aléatoires qu'on veut générer
    trajet* traj;
}trajets_aleatoires;

trajets_aleatoires* generate_x_random_itinerary(int nb_trajet);
void destroy_trajets_aleatoires(trajets_aleatoires* t);
void generate_x_numbers(int x, int* tab);
coord_pt* recuperation_coord_ligne(int num_ligne);

#endif /* RANDOM_POINTS_H__ */
