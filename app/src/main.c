// #define SNOW_ENABLED

#include "main.h"
#include "database_reader.h"
#include "coordinates.h"
#include "borne.h"
#include "itinerary.h"
// #include "snow.h"
#include "etape.h"
#include "vehicules.h"

int main(int argc, char* argv[])
{
  /*
  Arguments : 
  1 : longitude du point de départ
  2 : latitude du point de départ
  3 : longitude du point d'arrivée
  4 : latitude du point d'arrivée
  5 : ID voiture
  6 : pourcentage de batterie que l'utilisateur veut garder en réserve --> METTRE 0 PAR DEFAUT SI ON NE VEUT PAS DE RESERVE
  7 : temps maximum d'attente à une borne --> EN MINUTES ET METTRE 0 SI PAS DE TEMPS MAX
  8 : type de trajet --> 1 pour le plus court, 2 pour le plus rapide    
  */


  printf("%d\n",argc);
  long double longitude_depart = strtold(argv[1],NULL);
  long double latitude_depart = strtold(argv[2],NULL);
  long double longitude_arrivee = strtold(argv[3],NULL);
  long double latitude_arrivee = strtold(argv[4],NULL);
  int id_voiture = strtod(argv[5],NULL);
  int pourcentage_mini_voulu = strtod(argv[6],NULL);
  int temps_max_attente_borne = strtod(argv[7],NULL);
  int type = strtod(argv[8],NULL);

  // long double longitude_depart = 2.3483915;
  // long double latitude_depart =  48.8534951;
  // long double longitude_arrivee =  5.3699525;
  // long double latitude_arrivee =  43.2961743;
  // int id_voiture =  10;
  // int pourcentage_mini_voulu =  80;
  // int temps_max_attente_borne =  0;
  // int type = 1;

  voiture* ma_voiture = create_voiture(id_voiture);
  ma_voiture->temps_recharge_max_minutes = temps_max_attente_borne;
  ma_voiture->autonomie_max_utilisable = ma_voiture->autonomie * (1.0-(pourcentage_mini_voulu / 100.0));
  print_info(ma_voiture);

  // Calcul des étapes pour aller du point A au point B
  etape* resultat = get_liste_etape_itineaire(latitude_depart, longitude_depart, latitude_arrivee, longitude_arrivee, ma_voiture, type);
  if (resultat == NULL)
  {
    printf("Erreur lors du calcul de l'itinéraire\n");
    return 1;
  }
  export_etape(resultat);
  // Affichage
  display_etape(resultat);
  // Free de l'espace mémoire allouée
  etape_destroy(resultat);
  destroy_voiture(ma_voiture);
  return 0;
}

     
    



