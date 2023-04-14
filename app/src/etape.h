#ifndef ETAPE_H__
#define ETAPE_H__

#include "borne.h"


typedef struct etape
{
    borne_and_distance* list;
    int size;
} etape;

etape* etape_create(void);

void etape_add(etape* one_etape, borne_and_distance borne);

void display_etape(etape* one_etape);

void etape_destroy(etape* one_etape);


#endif /* ETAPE_H__*/
