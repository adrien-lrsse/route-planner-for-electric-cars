#ifndef __THREAD__
#define __THREAD__

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include "itinerary.h"
#include "coordinates.h"
#include "struct_mat_adj.h"
#include "database_reader.h"
#include "export.h"
#include "thread.h"
#include "vehicules.h"
#include "random_points.h"

#ifndef SUPPRESS_PRINTFS
#define printf(...) printf(__VA_ARGS__)
#else
#define printf(...) (void)0
#endif

typedef struct thread_data_list {
    trajet* data;
    struct thread_data_list* next;
} thread_data_list;

void thread_data_list_add(thread_data_list* list, trajet* data);

void thread_data_list_destroy(thread_data_list* list);

int thread_main(trajet* my_data);

void main_tests(int nb_forks, int nb_itineraires, int dist_min);

#endif /* __THREAD__ */
