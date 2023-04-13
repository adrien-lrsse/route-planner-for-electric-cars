#ifndef COORDINATES_H__
#define COORDINATES_H__

#include <math.h>
#include <stdio.h>


typedef struct coord_pt{
    double longitude;
    double latitude;
    double d;
} coord_pt;

double distance(double longitude_a, double latitude_a, double longitude_b, double latitute_b);


double to_radians(double degree);


#endif /* COORDINATES_H__ */
