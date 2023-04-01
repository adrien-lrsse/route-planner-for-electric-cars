#include "struct_mat_adj.h"

mat_adj* create_mat_adj(int taille){
    // Entrée : taille d'un cote de la matrice
    // Sortie : Adresse mémoire d'une zone de taille (n(n+1)/2) réservé
    mat_adj* new_el = malloc(sizeof(mat_adj));
    new_el->distance = malloc((taille*(taille+1)/2)*sizeof(double));
    new_el->length = taille;
    return new_el;
}


double get(mat_adj* mat, int i, int j){
    // (1,1) est la première valeur
    // (n,n) est la derniere valeur
    double value = *(mat->distance+((i*(i-1))/2)+(j-1));
    return value;
}

void define(mat_adj* mat, int i, int j,double value){
    *(mat->distance+((i*(i-1))/2)+(j-1)) = value;
}

void mat_destroy(mat_adj* mat){
    free(mat->distance);
    free(mat);
}
