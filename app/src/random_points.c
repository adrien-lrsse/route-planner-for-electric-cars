#include "random_points.h"


void generate_x_numbers(int x){
    
    srand(time(NULL));
    for (int i=0; i<x; i++){
        printf("%d, ",rand()%39105) ;
        //penser à prendre en compte si c'est 0 mm si ça arrivera jms...
    }
    
    //return rand()%10;//génère un nombre entre 0 et 9
}

coord_pt* recuperation_coord_ligne(int num_ligne){
    FILE *fichier = NULL;
    fichier = fopen("../../app/data/bd_villes_france.csv", "r+");
    if (fichier == NULL)
    {
        printf("Impossible d'ouvrir le fichier\n");
        return NULL;
    }
    coord_pt* point = malloc(sizeof(coord_pt));
    char ligne[1000];
    int compteur = 0;
    //char *key = malloc((strlen(ligne)+1)* sizeof(char));

    while (fgets(ligne, 1000, fichier) != NULL) {
        if (compteur == num_ligne){
            printf("%s\n", ligne);

            sscanf(ligne, "%*[^;];%*[^;];%*[^;];%*[^;];%*[^;];%Lf,%Lf", &point->latitude, &point->longitude);
            //strcpy(key, ligne);
        }
        compteur+=1;
    }

    //printf("latitude: %Lf, longitude: %Lf\n", point->latitude, point->longitude);
    //printf("%s\n", key);
    //free(point);
    return point;
}

//39105 lignes dans le doc csv

