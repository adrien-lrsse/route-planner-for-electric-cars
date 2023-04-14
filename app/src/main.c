#define SNOW_ENABLED

#include "main.h"
#include "database_reader.h"
#include "coordinates.h"
#include "borne.h"
#include "itinerary.h"
#include "snow.h"
#include "etape.h"

describe(test_itineraire){
    it("Conformance test"){
      // Calcul des étapes pour aller du point A au point B
      etape* resultat = get_liste_etape_itineaire(49.513496,0.163932,43.6689951,7.214612);
      // Affichage
      display_etape(resultat);
      // Free de l'espace mémoire allouée
      defer(etape_destroy(resultat));
    }
}

snow_main();
