// #define SNOW_ENABLED

#include "main.h"
#include "database_reader.h"
#include "coordinates.h"
#include "borne.h"
#include "itinerary.h"
// #include "snow.h"
#include "etape.h"

int main(int argc, char* argv[])
{
  printf("%d\n",argc);
  long double longitude_depart = strtold(argv[1],NULL);
  long double latitude_depart = strtold(argv[2],NULL);
  long double longitude_arrivee = strtold(argv[3],NULL);
  long double latitude_arrivee = strtold(argv[4],NULL);
  double autonomie = strtod(argv[5],NULL);


   // Calcul des étapes pour aller du point A au point B
      etape* resultat = get_liste_etape_itineaire(latitude_depart,longitude_depart,latitude_arrivee,longitude_arrivee,autonomie);
      export_etape(resultat);
      // Affichage
      display_etape(resultat);
      // Free de l'espace mémoire allouée
      etape_destroy(resultat);
  return 0;
}

     
    



