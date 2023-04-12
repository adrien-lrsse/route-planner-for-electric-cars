#ifndef BORNE_H__
#define BORNE_H__

#include "coordinates.h"

typedef struct borne
{
    coord_pt coordonnees;
    char* name;
    int id;
} borne;

typedef struct borne_and_distance
{
    borne borne;
    double distance_debut;
    double distance_fin;
} borne_and_distance;



#endif /* BORNE_H__ */
