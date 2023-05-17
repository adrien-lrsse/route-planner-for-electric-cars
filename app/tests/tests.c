#define SNOW_ENABLED
#include "../src/snow.h"
#include "../src/database_reader.h"
#include "../src/coordinates.h"
#include "../src/borne.h"
#include "../src/etape.h"
#include "../src/vehicules.h"
#include "../src/itinerary.h"


describe(test_itinerary_distance){
    it("Performance test"){
        long double longitude_depart = 6.1834097;
        long double latitude_depart =  48.6937223;
        long double longitude_arrivee =  -4.2328062;
        long double latitude_arrivee =  48.1815192;
        int id_voiture =  1;
        int pourcentage_mini_voulu =  20;
        int temps_max_attente_borne =  30;
        int type = 1;
        int pourcentage_autonomie_initiale =  35;

        voiture* ma_voiture = create_voiture(id_voiture);
        ma_voiture->temps_recharge_max_minutes = temps_max_attente_borne;
        ma_voiture->reserve_equivalent_autonomie = ma_voiture->autonomie * ((pourcentage_mini_voulu / 100.0));
        double autonomie_init = ma_voiture->autonomie * ((double)(pourcentage_autonomie_initiale / 100.0));
        set_autonomie(ma_voiture, autonomie_init);
        // print_info(ma_voiture);

        list_bornes_visitees* bornes_visitees = list_bornes_visitees_create(); // pour garder en mémoire les bornes visitées

        // Calcul des étapes pour aller du point A au point B
        etape* resultat = get_liste_etape_itineaire(latitude_depart, longitude_depart, latitude_arrivee, longitude_arrivee, ma_voiture, type, bornes_visitees);
        if (resultat == NULL)
        {
        // printf("Erreur lors du calcul de l'itinéraire\n");
        }
        // export_etape(resultat,-1);
        // Affichage
        // display_etape(resultat);
        // Free de l'espace mémoire allouée
        defer(etape_destroy(resultat));
        // list_bornes_visitees_print(bornes_visitees);//à enlever après
        // printf("test: %d\n",borne_deja_visitee(bornes_visitees, 44829));
        defer(list_bornes_visitees_destroy(bornes_visitees));
        defer(destroy_voiture(ma_voiture));
  }
}

describe(test_formula_distance){
  it("Performance test"){
      long double longitude_depart = 6.1834097;
      long double latitude_depart =  48.6937223;
      long double longitude_arrivee =  -4.2328062;
      long double latitude_arrivee =  48.1815192;
      long double d = distance(longitude_depart,latitude_depart,longitude_arrivee,latitude_arrivee);
      long double reel_d = 772.213087;
  }
  it("Precision test"){
      long double longitude_depart = 6.1834097;
      long double latitude_depart =  48.6937223;
      long double longitude_arrivee =  -4.2328062;
      long double latitude_arrivee =  48.1815192;
      long double d = distance(longitude_depart,latitude_depart,longitude_arrivee,latitude_arrivee);
      long double reel_d = 772.213087;
      assert(d >= reel_d + 2.5L || d >= reel_d-2.5L );
  }
}

snow_main();