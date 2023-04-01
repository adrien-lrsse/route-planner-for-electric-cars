#include "coordinates.h"


double distance(double longitude_a, double latitude_a, double longitude_b, double latitute_b){
    longitude_a = to_radians(longitude_a);
    longitude_b = to_radians(longitude_b);
    latitude_a = to_radians(latitude_a);
    latitute_b = to_radians(latitute_b);
    double dlambda = longitude_b-longitude_a;
    double distance = acos(sin(latitude_a)*sin(latitute_b)+cos(latitude_a)*cos(latitute_b)*cos(dlambda));
    return 6378.137*distance;
}

double to_radians(double degree){
    return degree * 3.14159 / 180;
}


/* int main(void)
{
    printf("%f\n",distance(-0.916585,44.040540,-0.500087,43.889896));
    return 0;
}
 */
