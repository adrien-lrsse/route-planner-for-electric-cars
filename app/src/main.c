// #define SNOW_ENABLED

#include "main.h"
#include "database_reader.h"
#include "coordinates.h"
#include "borne.h"
#include "itinerary.h"
// #include "snow.h"
#include "etape.h"
#include "vehicules.h"
#include "random_points.h"

#define NB_FORK 50
#define NB_ITINERAIRES 100
#define DIST_MIN 100

int main(int argc, char* argv[]) {
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
  9 : autonomie initiale --> EN POURCENTAGE DE L'AUTONOMIE DE LA VOITURE
  */

  // // pour tester la géénération aléatoire de trajet (ici on génère 20 trajets):
  // trajets_aleatoires* tab = generate_x_random_itinerary(20, 500);
  // destroy_trajets_aleatoires(tab);  
  // return 0;

  if (argc ==10){
    printf("%d\n",argc);
    long double longitude_depart = strtold(argv[1],NULL);
    long double latitude_depart = strtold(argv[2],NULL);
    long double longitude_arrivee = strtold(argv[3],NULL);
    long double latitude_arrivee = strtold(argv[4],NULL);
    int id_voiture = strtod(argv[5],NULL);
    int pourcentage_mini_voulu = strtod(argv[6],NULL);
    int temps_max_attente_borne = strtod(argv[7],NULL);
    int type = strtod(argv[8],NULL);
    int pourcentage_autonomie_initiale = strtod(argv[9],NULL);

    database_t * database = open_database("./../data/database.db");
    if (!database->opened_correctly) {
        exit(0);
    }

  // // Ceci sert au débogage car on ne peut pas passer des arguments à l'exécutable
  //   long double longitude_depart = 6.1834097;
  //   long double latitude_depart =  48.6937223;
  //   long double longitude_arrivee =  -4.2328062;
  //   long double latitude_arrivee =  48.1815192;
  //   int id_voiture =  1;
  //   int pourcentage_mini_voulu =  20;
  //   int temps_max_attente_borne =  30;
  //   int type = 1;
  //   int pourcentage_autonomie_initiale =  35;

    voiture* ma_voiture = create_voiture(id_voiture);
    ma_voiture->temps_recharge_max_minutes = temps_max_attente_borne;
    ma_voiture->reserve_equivalent_autonomie = ma_voiture->autonomie * ((pourcentage_mini_voulu / 100.0));
    double autonomie_init = ma_voiture->autonomie * ((double)(pourcentage_autonomie_initiale / 100.0));
    set_autonomie(ma_voiture, autonomie_init);
    // print_info(ma_voiture);

    list_bornes_visitees* bornes_visitees = list_bornes_visitees_create(); // pour garder en mémoire les bornes visitées

    // Calcul des étapes pour aller du point A au point B
    etape* resultat = get_liste_etape_itineaire(latitude_depart, longitude_depart, latitude_arrivee, longitude_arrivee, ma_voiture, type, bornes_visitees, database);
    close_database(database);
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
    list_bornes_visitees_print(bornes_visitees);//à enlever après
    printf("test: %d\n",borne_deja_visitee(bornes_visitees, 44829));
    list_bornes_visitees_destroy(bornes_visitees);
    destroy_voiture(ma_voiture);
    return 0;
  }

// Forks
  if (argc != 10){

    thread_export_init();
    int nb_boucles = NB_ITINERAIRES/NB_FORK;
    for (int j = 0; j < nb_boucles; j++){
      trajets_aleatoires* tab = generate_x_random_itinerary(NB_FORK,DIST_MIN);
      for (int i = 0; i < NB_FORK; i++){
        trajet* data = &tab->traj[i];
        int pid = fork();
        if (pid == 0){
          thread_main(data);
          destroy_trajets_aleatoires(tab);
          return 0;
        }
      }
      while((wait(NULL) != -1) || (errno != ECHILD));
      printf("Boucle numéro %d/%d finie\n", j+1, nb_boucles);
      destroy_trajets_aleatoires(tab);
    }
    return 0;
  }
}
