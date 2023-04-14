#include "etape.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

etape* etape_create(void){
    etape* new = malloc(sizeof(etape));
    new->list = NULL;
    new->size = 0;
    return new;
}


void etape_add(etape* one_etape, borne_and_distance borne){
    // Entrée : une liste d'étape, une étape à ajouter
    // Ajoute une etape à la liste on_etape
    // Sortie : (void)

    one_etape->size++;
    one_etape->list = realloc(one_etape->list,(one_etape->size)*sizeof(borne_and_distance));
    *(one_etape->list+one_etape->size-1) = borne;

    // Copie de la chaine de caractère            
    char* cpy_name = malloc((strlen(borne.borne.name)+1)*sizeof(char));
    strcpy(cpy_name,borne.borne.name);
    (one_etape->list+one_etape->size-1)->borne.name = cpy_name;

}

void display_etape(etape* one_etape){
    // Permet d'afficher la liste des étapes
    for (int i = 0; i < one_etape->size; i++)
    {
        printf("Etape %d :\n",i+1);
        printf("Borne %s, vous êtes à %f km du point précédent, plus que %f km avant l'arrivée // id_unique = %d\n",(one_etape->list+i)->borne.name,(one_etape->list+i)->distance_debut,(one_etape->list+i)->distance_fin,(one_etape->list+i)->borne.id);
    }  
}

void etape_destroy(etape* one_etape){
    // Permet de free l'espace allouée
    for (int i = 0; i < one_etape->size; i++)
    {
       free((one_etape->list+i)->borne.name);
    }
    free(one_etape->list);
    free(one_etape);
    
}
