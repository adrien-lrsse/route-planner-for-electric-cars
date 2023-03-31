#ifndef __FUNCTIONS_list_H__
#define __FUNCTIONS_list_H__

typedef struct list_t{
    int value;
    struct list_t* next_value_address;
} list_t;

list_t* list_create();
bool list_is_empty(list_t* one_list);
void list_prepend(list_t* one_list, int one_value);
int list_first(list_t* one_list);
void list_append(list_t* one_list, int one_value);
int list_last(list_t* one_list);
void list_print(list_t* one_list);
void list_insert(list_t* one_list, int one_value, int index);
int list_get(list_t* one_list, int index);
int list_index_of(list_t* one_list, int one_value);

#endif /* __FUNCTIONS_list_H__ */
