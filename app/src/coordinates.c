#include "coordinates.h"


long double distance(long double longitude_a, long double latitude_a, long double longitude_b, long double latitude_b){
    longitude_a = to_radians(longitude_a);
    longitude_b = to_radians(longitude_b);
    latitude_a = to_radians(latitude_a);
    latitude_b = to_radians(latitude_b);
    long double dlambda = longitude_b - longitude_a;
    long double distance = acosl(sinl(latitude_a) * sinl(latitude_b) + cosl(latitude_a) * cosl(latitude_b) * cosl(dlambda));
    return 6378.137L * distance;
}

long double to_radians(long double degree){
    return degree * 3.14159265358979323846L / 180.0L;
}



/* int main(void)
{
    printf("%f\n",distance(-0.916585,44.040540,-0.500087,43.889896));
    return 0;
}
 */
