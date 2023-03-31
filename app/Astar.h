#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#include "list.h"

typedef struct triplet_sommet {
    long double distance;
    long double distance_fin;
    list_t* chemin;
} triplet_sommet;

void Astar(double tab[], int taille_tab, int point_debut, int point_fin, double* distance_finale, list_t* chemin);

long double distance(double* tab[], int i, int j);

int meilleur_chemin(triplet_sommet* file[], int* vus[], int taille_tab);

#endif /* __FUNCTIONS_H__ */
