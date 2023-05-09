#ifndef __MAIN__
#define __MAIN__

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "coordinates.h"
#include "struct_mat_adj.h"
#include "database_reader.h"
#include "export.h"
#include "thread.h"

#define NB_THREADS 1000

#ifndef SUPPRESS_PRINTFS
#define printf(...) printf(__VA_ARGS__)
#else
#define printf(...) (void)0
#endif

int main(int argc, char* argv[]);

void* old_main(void* arg);

#endif /* __MAIN__ */
