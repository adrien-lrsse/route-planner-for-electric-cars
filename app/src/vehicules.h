#ifndef __VEHICULES__
#define __VEHICULES__

#include "database_reader.h"
#include "borne.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct voiture
{
    char* nom;
    int id;
    double autonomie;
    double autonomie_actuelle;
    int puissance;
    int puissance_actuelle;
    int temps_recharge_max_minutes; // 0 si pas de temps spécifié
    double autonomie_max_utilisable; // = autonomie si rien spécifié
} voiture;


voiture* create_voiture(int id);

void destroy_voiture(voiture* vehicule);

void update_vehicule(voiture* vehicule, double distance_parcourue);

void update_puissance(voiture* vehicule);

void update_autonomie(voiture* vehicule);

void recharge(voiture* vehicule, borne* borne_recharge, int temps_minutes);

void print_info(voiture* vehicule);

#endif /* __VEHICULES__ */
