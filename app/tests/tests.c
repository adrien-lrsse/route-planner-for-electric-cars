#define SNOW_ENABLED
#include "../src/snow.h"
#include "../src/database_reader.h"
#include "../src/coordinates.h"
#include "../src/borne.h"
#include "../src/etape.h"
#include "../src/vehicules.h"
#include "../src/itinerary.h"
#include "../src/random_points.h"
#include "../src/thread.h"

#define NB_FORKS 75
#define NB_ITINERAIRES 1000
#define DIST_MIN 300

describe(test_itinerary_distance){
    it("Conformance test"){
        long double longitude_depart = 6.1834097;
        long double latitude_depart =  48.6937223;
        long double longitude_arrivee =  -4.2328062;
        long double latitude_arrivee =  48.1815192;
        int id_voiture =  1;
        int pourcentage_mini_voulu =  20;
        int temps_max_attente_borne =  30;
        int type = 1;
        int pourcentage_autonomie_initiale =  35;

        database_t * database = open_database("./../data/database.db");
        if (!database->opened_correctly) {
            exit(0);
        }

        voiture* ma_voiture = create_voiture(id_voiture);
        ma_voiture->temps_recharge_max_minutes = temps_max_attente_borne;
        ma_voiture->reserve_equivalent_autonomie = ma_voiture->autonomie * ((pourcentage_mini_voulu / 100.0));
        double autonomie_init = ma_voiture->autonomie * ((double)(pourcentage_autonomie_initiale / 100.0));
        set_autonomie(ma_voiture, autonomie_init);
        // print_info(ma_voiture);

        list_bornes_visitees* bornes_visitees = list_bornes_visitees_create(); // pour garder en mémoire les bornes visitées

        // Calcul des étapes pour aller du point A au point B
        etape* resultat = get_liste_etape_itineaire(latitude_depart, longitude_depart, latitude_arrivee, longitude_arrivee, ma_voiture, type, bornes_visitees, database);
        defer(close_database(database));
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

describe(test_threads){
  it("Conformance test"){
    thread_export_init();
    int nb_boucles = NB_ITINERAIRES/NB_FORKS;
    if (nb_boucles * NB_FORKS < NB_ITINERAIRES){ nb_boucles++; }
    for (int j = 0; j < nb_boucles; j++){
      trajets_aleatoires* tab = generate_x_random_itinerary(NB_FORKS+2,DIST_MIN);
      for (int i = 0; i < NB_FORKS; i++){
        trajet* data = &tab->traj[i];
        int pid = fork();
        if (pid == 0){
          thread_main(data);
          destroy_trajets_aleatoires(tab);
          exit(0);
        }
      }
      while((wait(NULL) != -1) || (errno != ECHILD));
      printf("Boucle numéro %d/%d finie\n", j+1, nb_boucles);
      destroy_trajets_aleatoires(tab);
    }
  }
}

describe(test_random){
  it("Conformance test"){
    trajets_aleatoires* tab = generate_x_random_itinerary(NB_FORKS,DIST_MIN);
    destroy_trajets_aleatoires(tab);
  }
}


snow_main();
