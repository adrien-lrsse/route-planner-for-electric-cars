#ifndef DATABASE_ACCESS_H__
#define DATABASE_ACCESS_H__

#include "database_reader.h"
#include "coordinates.h"
#include "borne.h"
#include <stdbool.h>
typedef struct position_point
{
    double distance_depart;
    double distance_arrivee;
    int indice_unique;
} position_point;

typedef struct list_position
{
    struct list_position* next;
    position_point node;
    
} list_position;



list_position* getBorneFromDistance(double latitude_depart, double longitude_depart, double latitude_arrivee, double longitude_arrivee);

void add(list_position** list, double depart, double arrivee, int id_unique);

void print_position(list_position* list);

list_position* list_create(void);

void list_destroy(list_position* one_list);

bool list_is_empty(list_position* one_list);

borne_and_distance plus_proche(list_position* one_list, double autonomie);


borne getInfo(int i);

#endif /* DATABASE_ACCESS_H__ */