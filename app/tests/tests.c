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

#define NB_THREADS 20
#define NB_ITINERAIRES 100
#define DIST_MIN 100

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

describe(test_threads){
    it("Conformance test"){
      int nb_boucles = NB_ITINERAIRES/NB_THREADS;
      thread_export_init();

      for(int i=0; i<nb_boucles;i++){
        //printf("BOUCLE NUMERO %d\n",i);
        pthread_mutex_t mutex;
        pthread_mutex_init(&mutex, NULL);
        trajets_aleatoires* tab = generate_x_random_itinerary(NB_THREADS,DIST_MIN);

        pthread_t *tid = malloc(NB_THREADS * sizeof(pthread_t));
        thread_data_list* liste_thread = malloc(sizeof(thread_data_list));
        liste_thread->data = NULL;
        liste_thread->next = NULL;
        for(int i=0; i<NB_THREADS; i++){

          trajet* data = &tab->traj[i];
          data->thread = i;
          thread_data_list_add(liste_thread, data);
          data->mutex = mutex;
          // long double longitude_depart = 6.1834097;
          // data->depart->longitude = longitude_depart;
          // long double latitude_depart =  48.6937223;
          // long double longitude_arrivee =  -4.2328062;
          // long double latitude_arrivee =  48.1815192;
          // int id_voiture =  1;
          // int pourcentage_mini_voulu =  20;
          // int temps_max_attente_borne =  30;
          // int type = 1;
          // int pourcentage_autonomie_initiale =  35;
          // data->depart->latitude = latitude_depart;
          // data->arrivee->longitude = longitude_arrivee;
          // data->arrivee->latitude = latitude_arrivee;
          // data->id_voiture = id_voiture;
          // data->pourcentage_mini_voulu = pourcentage_mini_voulu;
          // data->temps_max_attente_borne = temps_max_attente_borne;
          // data->type = type;
          // data->pourcentage_autonomie_initiale = pourcentage_autonomie_initiale;

          printf("%Lf ", data->depart->longitude);
          printf("%Lf ", data->depart->latitude);
          printf("%Lf ", data->arrivee->longitude);
          printf("%Lf ", data->arrivee->latitude);
          printf("%d ", data->id_voiture);
          printf("%d ", data->pourcentage_mini_voulu);
          printf("%d ", data->temps_max_attente_borne);
          printf("%d ", data->type);
          printf("%d\n", data->pourcentage_autonomie_initiale);

          if (pthread_create(&tid[i], NULL, &thread_main, data)){
            printf("Error creating thread %d\n", i);
          }
          //printf("Thread %d created\n", i);
        }

        for (int i = 0; i < NB_THREADS; i++) {
          if (pthread_join(tid[i], NULL)){
            printf("Error joining thread %d\n", i);
          }
          //printf("Thread %d finished\n", i);
        }
        free(tid);
        thread_data_list_destroy(liste_thread);
        destroy_trajets_aleatoires(tab);
        pthread_mutex_destroy(&mutex);
      }
  }
}

describe(test_random){
  it("Conformance test"){
    trajets_aleatoires* tab = generate_x_random_itinerary(NB_THREADS,DIST_MIN);
    destroy_trajets_aleatoires(tab);


  }
}


snow_main();
