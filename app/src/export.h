#ifndef EXPORT_H__
#define EXPORT_H__

#include "etape.h"
#include "vehicules.h"
#include "simulation.h"
#include <stdio.h>

#ifndef SUPPRESS_PRINTFS
#define printf(...) printf(__VA_ARGS__)
#else
#define printf(...) (void)0
#endif

void export_etape(etape* one_etape);

void thread_export(etape* one_etape);

void thread_export_init(void);


#endif /* EXPORT_H__ */
