#ifndef __THREAD__
#define __THREAD__

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "itinerary.h"
#include "coordinates.h"
#include "struct_mat_adj.h"
#include "database_reader.h"
#include "export.h"
#include "thread.h"
#include "vehicules.h"

#ifndef SUPPRESS_PRINTFS
#define printf(...) printf(__VA_ARGS__)
#else
#define printf(...) (void)0
#endif

typedef struct thread_data {
    long double longitude_depart;
    long double latitude_depart;
    long double longitude_arrivee;
    long double latitude_arrivee;
    int id_voiture;
    int pourcentage_mini_voulu;
    int temps_max_attente_borne;
    int type;
    int pourcentage_autonomie_initiale;
    int thread;
} thread_data;

typedef struct thread_data_list {
    thread_data* data;
    struct thread_data_list* next;
} thread_data_list;

void thread_data_list_add(thread_data_list* list, thread_data* data);

void thread_data_list_destroy(thread_data_list* list);

void* thread_main(void* arg);

#endif /* __THREAD__ */
