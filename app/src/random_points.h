#ifndef RANDOM_POINTS_H__
#define RANDOM_POINTS_H__

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "coordinates.h"

// typedef struct trajet_
// {
//     coord_pt depart;
//     coord_pt arrivee;
// }trajet_;


// typedef struct tab_trajets{
//     int taille; //nb de trajets aléatoires qu'on veut générer
//     trajet_ trajet;
// }

void generate_x_numbers(int x);
coord_pt* recuperation_coord_ligne(int num_ligne);

#endif /* RANDOM_POINTS_H__ */
