#ifndef COORDINATES_H__
#define COORDINATES_H__

#include <math.h>
#include <stdio.h>

#ifndef SUPPRESS_PRINTFS
#define printf(...) printf(__VA_ARGS__)
#else
#define printf(...) (void)0
#endif

typedef struct coord_pt{
    long double longitude;
    long double latitude;
    double d;
} coord_pt;

long double distance(long double longitude_a, long double latitude_a, long double longitude_b, long double latitude_b);


long double to_radians(long double degree);


#endif /* COORDINATES_H__ */
