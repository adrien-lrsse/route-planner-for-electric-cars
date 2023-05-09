#include "thread.h"

void thread_data_list_add(thread_data_list* list, thread_data* data){
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
        free(tmp->data);
        free(tmp);
        tmp = next;
}
}
