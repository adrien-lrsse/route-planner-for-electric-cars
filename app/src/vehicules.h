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
    double autonomie_actuelle; // autonomie que je peux utiliser pour voyager = ON NE PREND PAS EN COMPTE LA BATTERIE EN RESERVE
    double reserve_equivalent_autonomie; // équivalent en km du pourcentage minimum de batterie voulu en réserve
    int puissance;
    int puissance_actuelle; // puisssance TOTALE disponible = ON PREND EN COMPTE LA BATTERIE EN RESERVE
    int temps_recharge_max_minutes; // 0 si pas de temps spécifié
} voiture;


voiture* create_voiture(int id);

void destroy_voiture(voiture* vehicule);

void update_charge(voiture* vehicule, double distance_parcourue);

void update_puissance(voiture* vehicule);

void update_autonomie(voiture* vehicule);

void recharge(voiture* vehicule, int puissance);

void print_info(voiture* vehicule);

void set_autonomie(voiture* vehicule, double autonomie);

#endif /* __VEHICULES__ */
