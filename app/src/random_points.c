#include "random_points.h"

trajets_aleatoires* generate_x_random_itinerary(int nb_trajet, long double dist_min){
    //entrée: nombre de trajets qu'on veut générer
    //sortie: tableau contenant les coordonnées de départ et d'arrivée de nb_trajets trajets générés aléatoirement
    trajets_aleatoires* res = malloc(sizeof(trajets_aleatoires));
    res->taille = nb_trajet;
    res->traj = malloc(nb_trajet*sizeof(trajet));

    srand(time(NULL));  //pour les nombres aléatoires

    for (int i=0; i<nb_trajet; i++){       
        res->traj[i].depart = recuperation_coord_ligne(rand()%38716);
        res->traj[i].arrivee = recuperation_coord_ligne(rand()%38716);
        long double dist_correspondante = distance(res->traj[i].depart->longitude, res->traj[i].depart->latitude, res->traj[i].arrivee->longitude, res->traj[i].arrivee->latitude);
        
        //----------------------affichage des coordonnées de chaque trajet-------------------------------------
        //printf("test depart %d: %Lf, %Lf\n", j, res->traj[i].depart->latitude, res->traj[i].depart->longitude);
        //printf("test arrivee %d: %Lf, %Lf\n", j+1 , res->traj[i].arrivee->latitude, res->traj[i].arrivee->longitude);
        
        //printf("distance en km du trajet %d: %Lf\n", i, dist_correspondante);
        int nb_essais=0; //limite de tps pour éviter les boucles infinies;
        while (dist_correspondante<dist_min && nb_essais<50){
            //si la taille du trajet n'est pas assez élevée on change de trajet, si plus de 50 essais on arrete
            
            free(res->traj[i].depart);
            res->traj[i].depart = recuperation_coord_ligne(rand()%38716);
            
            free(res->traj[i].arrivee);
            res->traj[i].arrivee = recuperation_coord_ligne(rand()%38715+1); 

            dist_correspondante = distance(res->traj[i].depart->longitude, res->traj[i].depart->latitude, res->traj[i].arrivee->longitude, res->traj[i].arrivee->latitude);
            //printf("distance en km du trajet  %d (rectification %d): %Lf\n", i, nb_essais, dist_correspondante);
            nb_essais++;
        }
        
        res->traj[i].distance_traj=dist_correspondante;
    }

    all_random_parameters(res);
    
    return res;
}

void destroy_trajets_aleatoires(trajets_aleatoires* t){
    if (t==NULL){
        return;
    }
    for(int i=0; i<t->taille; i++){
        free(t->traj[i].depart);
        free(t->traj[i].arrivee);
    }
    free(t->traj);
    free(t);
}


coord_pt* recuperation_coord_ligne(int num_ligne){
    //entrée: numéro de la ligne dans le fichier dont on veut récupérer les coordonnées
    //sortie: renvoie les coordonnées coorrespondantes 
    //penser à free les coordonnées après l'appel de cette fonction
    FILE *fichier = NULL;
    fichier = fopen("../../app/data/bd_villes_france.csv", "r+");
    if (fichier == NULL)
    {
        printf("Impossible d'ouvrir le fichier\n");
        return NULL;
    }
    coord_pt* point = malloc(sizeof(coord_pt));
    char ligne[1000];
    int compteur = 0; //compte les lignes

    while (fgets(ligne, 1000, fichier) != NULL) {
        if (compteur == num_ligne){
            //printf("%s", ligne); //affichage de la ligne
            sscanf(ligne, "%*[^;];%*[^;];%*[^;];%*[^;];%*[^;];%Lf,%Lf", &point->latitude, &point->longitude);
        }
        compteur+=1;
    }
    return point;
}

void all_random_parameters(trajets_aleatoires* trajet_existant){
    // random int voiture between 1 and 310
    // random int pourcentage_mini_voulu between 1 and 75
    // random int temps_max_attente_borne between 1 and 60
    // random int type between 1 and 2
    // random int pourcentage_autonomie_initiale between 5 and 100

    srand(time(NULL));
    for (int i=0; i<trajet_existant->taille; i++){
        trajet_existant->traj[i].id_voiture = rand()%310+1;
        trajet_existant->traj[i].pourcentage_mini_voulu = rand()%75+1;
        trajet_existant->traj[i].temps_max_attente_borne = rand()%60+1;
        trajet_existant->traj[i].type = rand()%2+1;
        trajet_existant->traj[i].pourcentage_autonomie_initiale = rand()%(100-trajet_existant->traj[i].pourcentage_mini_voulu)+trajet_existant->traj[i].pourcentage_mini_voulu;
    }
}

