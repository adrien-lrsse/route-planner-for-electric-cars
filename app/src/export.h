#ifndef EXPORT_H__
#define EXPORT_H__

#include "etape.h"
#include <stdio.h>

#ifndef SUPPRESS_PRINTFS
#define printf(...) printf(__VA_ARGS__)
#else
#define printf(...) (void)0
#endif

void export_etape(etape* one_etape, int my_pid);

#endif /* EXPORT_H__ */
