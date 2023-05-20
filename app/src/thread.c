#include "thread.h"

void thread_data_list_add(thread_data_list* list, trajet* data){
    if (list->data == NULL){
        list->data = data;
        return;
    }
    thread_data_list* tmp = list;
    while(tmp->next != NULL){
        tmp = tmp->next;
    }
    tmp->next = malloc(sizeof(thread_data_list));
    tmp->next->data = data;
    tmp->next->next = NULL;
}

void thread_data_list_destroy(thread_data_list* list){
    thread_data_list* tmp = list;
    while(tmp != NULL){
        thread_data_list* next = tmp->next;
        free(tmp);
        tmp = next;
}
}

int thread_main(trajet* my_data) {
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

  // printf("Longitude départ : %Lf\n", longitude_depart);
  // printf("Latitude départ : %Lf\n", my_data->depart->latitude);
  // printf("Longitude arrivée : %Lf\n", longitude_arrivee);
  // printf("Latitude arrivée : %Lf\n", latitude_arrivee);
  // printf("ID voiture : %d\n", id_voiture);
  // printf("Pourcentage mini voulu : %d\n", pourcentage_mini_voulu);
  // printf("Temps max attente borne : %d\n", temps_max_attente_borne);
  // printf("Type : %d\n", type);
  // printf("Pourcentage autonomie initiale : %d\n", pourcentage_autonomie_initiale);

  voiture* ma_voiture = create_voiture(my_data->id_voiture);
  ma_voiture->temps_recharge_max_minutes = my_data->temps_max_attente_borne;
  ma_voiture->reserve_equivalent_autonomie = ma_voiture->autonomie * ((my_data->pourcentage_mini_voulu / 100.0));
  double autonomie_init = ma_voiture->autonomie * ((double)(my_data->pourcentage_autonomie_initiale / 100.0));
  set_autonomie(ma_voiture, autonomie_init);
  //print_info(ma_voiture);

  database_t * database = open_database("./../data/database.db");
  if (!database->opened_correctly) {
    printf("Erreur lors de l'ouverture de la base de données\n");
      exit(0);
  }

  list_bornes_visitees* bornes_visitees = list_bornes_visitees_create(); // pour garder en mémoire les bornes visitées

  // Calcul des étapes pour aller du point A au point B
  etape* resultat = get_liste_etape_itineaire(my_data->depart->latitude, my_data->depart->longitude, my_data->arrivee->latitude, my_data->arrivee->longitude, ma_voiture, my_data->type, bornes_visitees, database);
  close_database(database);
  if (resultat == NULL)
  {
    printf("Erreur lors du calcul de l'itinéraire\n");
    return EXIT_FAILURE;
  }
  //printf("Thread numéro %d : export en cours...\n", my_data->thread);
  //export_etape(resultat, my_data->thread);
  // Affichage
  // display_etape(resultat);
  // Free de l'espace mémoire allouée
  thread_export(resultat);
  etape_destroy(resultat);
  list_bornes_visitees_destroy(bornes_visitees);
  destroy_voiture(ma_voiture);
  return EXIT_SUCCESS;
}

void main_tests(int nb_forks, int nb_itineraires, int dist_min){
  thread_export_init();
  int nb_boucles = nb_itineraires/nb_forks;
  for (int j = 0; j < nb_boucles; j++){
    trajets_aleatoires* tab = generate_x_random_itinerary(nb_forks,dist_min);
    for (int i = 0; i < nb_forks; i++){
      trajet* data = &tab->traj[i];
      int pid = fork();
      if (pid == 0){
        thread_main(data);
        destroy_trajets_aleatoires(tab);
        return;
      }
    }
    while((wait(NULL) != -1) || (errno != ECHILD));
    printf("Boucle numéro %d/%d finie\n", j+1, nb_boucles);
    destroy_trajets_aleatoires(tab);
  }
  return;
}
