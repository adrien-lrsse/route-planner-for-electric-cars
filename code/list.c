#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "list.h"

list_t* list_create() {
    list_t *new_list = (list_t*)malloc(sizeof(list_t));
    new_list->value = -1; // the tail is always -1
    return new_list;
}

bool list_is_empty(list_t* one_list) {
    return (one_list->value == -1);

}

void list_prepend(list_t* one_list, int one_value) {
    list_t *new_list = list_create();
    // place la liste dans un nouvel élément
    new_list->value = one_list->value;
    new_list->next_value_address = one_list->next_value_address;
    //change la tete de liste
    one_list->value = one_value;
    one_list->next_value_address = new_list; // pointe vers l'ancienne liste
}

int list_first(list_t* one_list) {
    return one_list->value;
}

void list_append(list_t* one_list, int one_value) {
    list_t *last_element = one_list;
    //find last element of the list
    while (last_element->value != -1) {
        last_element = last_element->next_value_address;
    }
    //set the last element and add a new tail
    list_t *tail_of_list = list_create();
    last_element->value = one_value;
    last_element->next_value_address = tail_of_list;
}

int list_last(list_t* one_list) {
    list_t *last_element = one_list;
    if (!list_is_empty(one_list)) {
        while ((last_element->next_value_address)->value != -1) {
            last_element = last_element->next_value_address;
        } 
    }
    return last_element->value;       
}

void list_print(list_t* one_list) {
    printf("[");
    list_t *last_element = one_list;
    while ((last_element->next_value_address)->value != -1) {
        printf("%d, ",last_element->value);
        last_element = last_element->next_value_address;
    }
    printf("%d]\n",last_element->value);
}

void list_insert(list_t* one_list, int one_value, int index) {
    if (index == 0) {
        list_prepend(one_list, one_value);
    }
    else
    {
        int compteur = 0;
        list_t *last_element = one_list;
        while (compteur < index-1) {
            compteur++;
            last_element = last_element->next_value_address;
        }
        list_t *new_element = list_create();
        new_element->next_value_address =  last_element->next_value_address;
        new_element->value = one_value;
        last_element->next_value_address = new_element;
    }
}

int list_get(list_t* one_list, int index) {
    int compteur = 0;
    list_t *last_element = one_list;
    while (compteur < index) {
        compteur++;
        last_element = last_element->next_value_address;
    }
    return last_element->value;
}

int list_index_of(list_t* one_list, int one_value) {
    int compteur = 0;
    list_t *last_element = one_list;
    while ((last_element->value != -1) && (last_element->value != one_value)) {
        compteur++;
        last_element = last_element->next_value_address;
    }
    return compteur;
}

void list_destroy(list_t* one_list) {
    if (one_list->value == -1) {
        free(one_list);
    }
    else {
        list_destroy(one_list->next_value_address);
        free(one_list);
    }
}
