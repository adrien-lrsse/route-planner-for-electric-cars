  #define SNOW_ENABLED

#include "main.h"
#include "database_reader.h"
#include "coordinates.h"
#include "borne.h"
#include "database_access.h"
  #include "snow.h"


// int main (void){
//     double distance_fin = distance(0.163932,49.513496,7.214612,43.6689951);
//     list_position* resultat = getBorneFromDistance(49.513496,0.163932,43.6689951,7.214612);
//         int i = 0;
//     do
//     {
//         borne_and_distance proche = plus_proche(resultat,100);
//                     distance_fin = distance(proche.borne.coordonnees.longitude,proche.borne.coordonnees.latitude,7.214612,43.6689951);
//                     list_destroy(resultat);

//             printf("Etape %d : \nBorne %s, vous êtes à %f km du point précédent, plus que %f km avant l'arrivée // id_unique = %d\n",i,proche.borne.name,proche.distance_debut,proche.distance_fin, proche.borne.id);
//             resultat = getBorneFromDistance(proche.borne.coordonnees.latitude,proche.borne.coordonnees.longitude,43.6689951,7.214612);
//             distance_fin = distance(proche.borne.coordonnees.longitude,proche.borne.coordonnees.latitude,7.214612,43.6689951);
//             free(proche.borne.name);
//             i++;
            
//     } while (distance_fin != 0.0);
    

//     // while (distance_fin != 0.0)
//     // {
//     //         borne_and_distance proche = plus_proche(resultat,100);
//     //         printf("Etape %d : \nBorne %s, vous êtes à %f km du point précédent, plus que %f km avant l'arrivée\n",i,proche.borne.name,proche.distance_debut,proche.distance_fin);
//     //         resultat = getBorneFromDistance(proche.borne.coordonnees.latitude,proche.borne.coordonnees.longitude,43.637548,1.375103);
        
//     //         distance_fin = distance(proche.borne.coordonnees.longitude,proche.borne.coordonnees.latitude,1.375103,43.637548);
            
//     //         i++;
//     // }
    
//     // printf("Latitude : %f \n",proche.coordonnees.latitude);
//     // printf("Longitude : %f \n",proche.coordonnees.longitude);
//     // list_position* resultat2 = getBorneFromDistance(proche.coordonnees.latitude,proche.coordonnees.longitude,43.637548,1.375103);
//     // borne proche2 = plus_proche(resultat2,100);
//     // printf("Etape 1 : %s se trouve à %f km du point précédent \n",proche.name,proche.coordonnees.d);
//     // printf("Etape 2 : %s se trouve à %f km du point précédent \n",proche2.name,proche2.coordonnees.d);
//     // print_position(resultat);
//     list_destroy(resultat);
//     // list_destroy(resultat2);
//     // borne su = getInfo(15);
//     // printf("%d",su.id);
//     // free(su.name);
//     return 0;
// }

describe(test_itineraire){
    it("Conformance test"){
       double distance_fin = distance(0.163932,49.513496,7.214612,43.6689951);
    list_position* resultat = getBorneFromDistance(49.513496,0.163932,43.6689951,7.214612);
        int i = 0;
    do
    {
        borne_and_distance proche = plus_proche(resultat,100);
                    distance_fin = distance(proche.borne.coordonnees.longitude,proche.borne.coordonnees.latitude,7.214612,43.6689951);
                    list_destroy(resultat);

            printf("Etape %d : \nBorne %s, vous êtes à %f km du point précédent, plus que %f km avant l'arrivée // id_unique = %d\n",i,proche.borne.name,proche.distance_debut,proche.distance_fin, proche.borne.id);
            resultat = getBorneFromDistance(proche.borne.coordonnees.latitude,proche.borne.coordonnees.longitude,43.6689951,7.214612);
            distance_fin = distance(proche.borne.coordonnees.longitude,proche.borne.coordonnees.latitude,7.214612,43.6689951);
            free(proche.borne.name);
            i++;
            
    } while (distance_fin != 0.0);
        list_destroy(resultat);

    }
    }


 snow_main();
