#include "borne.h"

list_bornes_visitees* list_bornes_visitees_create(void){
    //creation d'une liste
    list_bornes_visitees* new_list=malloc(sizeof(list_bornes_visitees));
    new_list->next = NULL;
    return new_list;
}

bool list_bornes_visitees_is_empty(list_bornes_visitees *one_list){
    //return true si la one_list est vide, false sinon
    if (one_list->next==NULL){
        return true;
    }else{
        return false;
    }
}

void list_bornes_visitees_destroy(list_bornes_visitees* one_list){
    //free les adresses allouées pour one_list
    if (list_bornes_visitees_is_empty(one_list)==1){
        free(one_list);
    }
    else{
        list_bornes_visitees_destroy(one_list->next);
        free(one_list);
    }
}

void list_bornes_visitees_append(list_bornes_visitees* one_list, int id_borne){
    //ajoute l'identifiant d'une borne (id_borne) à la fin de la liste des bornes visitées (one_list)
    list_bornes_visitees* last_elt = one_list;
    while (list_bornes_visitees_is_empty(last_elt)==false){
        last_elt=last_elt->next;
    }
    list_bornes_visitees* tail = list_bornes_visitees_create();
    last_elt->id_borne_visitee = id_borne;
    last_elt->next = tail;
}

bool borne_deja_visitee(list_bornes_visitees* one_list, int id_borne){
    //return true si one_list contient id_borne, false sinon
    //vérifie si une borne a déjà été visitée
    
    list_bornes_visitees* liste = one_list;
    while (list_bornes_visitees_is_empty(liste) == 0){
        if (liste->id_borne_visitee == id_borne){
            return true;
        }
        liste = liste->next;
    }
    return false;
}

void list_bornes_visitees_print(list_bornes_visitees *one_list){
    list_bornes_visitees* tmp = one_list;
    printf("bornes_visitees:[");
    while (list_bornes_visitees_is_empty(tmp)==0){
        printf("%d ,\n", tmp->id_borne_visitee);
        tmp=tmp->next;
    }
    printf("]\n");
}
