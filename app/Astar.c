#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Astar.h"
#include "list.h"

void Astar(double* tab, int taille_tab, int point_debut, int point_fin, double* distance_finale, list_t* chemin){
    //variable initialisation
    int* vus[taille_tab]; //tableau de booléens
    for (int i=0;i<taille_tab;i++){vus[i]=0;} //on initialise vus à non vus
    triplet_sommet* file[taille_tab];//"file" des sommets avec distance au point de départ, distance à la fin, et le chemin emprunté
    int sommet_actuel = point_debut;//sommet en cours de traitement
    vus[point_debut] = 1;

    //initialisation
    for (int i=0;i<taille_tab;i++){
        file[i]->distance = distance(tab,point_debut,i);
        file[i]->distance_fin = file[i]->distance + distance(tab,i,point_fin);
        list_t* new_list = list_create();
        list_append(file[i]->chemin,sommet_actuel);
    }
    sommet_actuel = meilleur_chemin(file, vus, taille_tab);
    vus[sommet_actuel] = 1;
    //main core
    while (sommet_actuel != point_fin) { //tant qu'on est pas sur le sommet final
        for (int i=0;i<taille_tab;i++) {
            if (vus[i] == 0) { //traiter seulement les sommets pas encore vus
                double nouvelle_distance = file[sommet_actuel]->distance+distance(tab,sommet_actuel,i);
                double nouvelle_distance_fin = nouvelle_distance + distance(tab,i,point_fin);
                if (nouvelle_distance_fin<file[i]->distance_fin){
                    file[i]->distance = nouvelle_distance;
                    file[i]->distance_fin = nouvelle_distance_fin;
                    list_append(file[i]->chemin,sommet_actuel);
                }
            }
        }
        sommet_actuel = meilleur_chemin(file, vus, taille_tab);
        vus[sommet_actuel] = 1;
    }
    *distance_finale = file[point_fin]->distance_fin;
    chemin = file[point_fin]->chemin;
}

int meilleur_chemin(triplet_sommet* file[], int* vus[], int taille_tab) {
    int minimum = -1;
    int nouveau_sommet=0;
    for (int i=0;i<taille_tab;i++){
        if ((vus[i] == 0) && (file[i]->distance_fin<minimum || minimum==-1)) {
            minimum = file[i]->distance_fin;
            nouveau_sommet = i;
        }
    }
    return nouveau_sommet;
}
