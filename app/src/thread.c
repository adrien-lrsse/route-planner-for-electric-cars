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

void* thread_main(void* arg) {
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

  trajet* my_data = (trajet*)arg;

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

  list_bornes_visitees* bornes_visitees = list_bornes_visitees_create(); // pour garder en mémoire les bornes visitées

  // Calcul des étapes pour aller du point A au point B
  etape* resultat = get_liste_etape_itineaire(my_data->depart->latitude, my_data->depart->longitude, my_data->arrivee->latitude, my_data->arrivee->longitude, ma_voiture, my_data->type, bornes_visitees);
  if (resultat == NULL)
  {
    printf("Erreur lors du calcul de l'itinéraire\n");
    pthread_exit(EXIT_SUCCESS);
  }
  //printf("Thread numéro %d : export en cours...\n", my_data->thread);
  //export_etape(resultat, my_data->thread);
  // Affichage
  // display_etape(resultat);
  // Free de l'espace mémoire allouée
  etape_destroy(resultat);
  list_bornes_visitees_destroy(bornes_visitees);
  destroy_voiture(ma_voiture);
  pthread_exit(EXIT_SUCCESS);
}
