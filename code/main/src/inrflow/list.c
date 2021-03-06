#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "applications.h"
#include "list.h"

/*
 * Initializes a generic list.
 * @input l: Pointer to a struct list_t.
 * @input t_size: Siseof of the data to be stored.
 */
void list_initialize(list_t *l, int t_size)
{
    l->length = 0;
    l->t_size = t_size;
    l->head = NULL;
    l->tail = NULL;
}

/*
 * Remove all the elements of l.
 * @input l: Pointer to a struct list_t. 
 * List l can be reused without reinitialize.
 */
void list_destroy(list_t *l)
{
    node_list *curr;

    while(l->head != NULL) {
        curr = l->head;
        l->head = curr->next;
        free(curr->data);
        free(curr);
    }
    l->length = 0;
    l->tail = NULL;
}

/*
 * Insert an element at the end of the list.
 * @input l: Pointer to a struct list_t.
 * @input elem: Pointer to the data to be stored.
 */
void list_append(list_t *l, void *elem)
{
    node_list *node = malloc(sizeof(node_list));
    node->data = malloc(l->t_size);
    node->next = NULL;
    node->prev = NULL;

    memcpy(node->data, elem, l->t_size);
    if(l->length == 0) {
        l->head = node;
    } 
    else {
        node->prev = l->tail; 
        l->tail->next = node;
    }
    l->tail = node;
    l->length++;
}

/*
 * Insert an element at the start of the list.
 * @input l: Pointer to a struct list_t.
 * @input elem: Pointer to the data to be stored.
 */
node_list *list_insert(list_t *l, void *elem)
{
    node_list *node = malloc(sizeof(node_list));
    node->data = malloc(l->t_size);
    node->next = NULL;
    node->prev = NULL;

    memcpy(node->data, elem, l->t_size);

    if(l->length == 0) {
        l->tail = node;
    }
    else{
        node->next = l->head;
        l->head->prev = node;
    }
    l->head = node;
    l->length++;
    return(node);
}

/*
 * Mark the last element of the list in order to use list_trunc.
 * @input l: Pointer to a struct list_t.
 */
void list_bookmark(list_t *l){

    l->bookmark = l->tail;
}

/*
 * Return the first element of the list.
 * @input l: Pointer to a struct list_t.
 * @elem: Pointer where the element will be stored
 */
void list_head(list_t *l, void **elem)
{
    if(l->length > 0){
        *elem = l->head->data;
    }
    else{
        elem = NULL;
    }
}

/*
 * Return the first element of the list.
 * @input l: Pointer to a struct list_t.
 * @elem: Pointer where the element will be stored
 */
void list_head_node(list_t *l, void **elem)
{
    if(l->length > 0){
        *elem = l->head;
    }
    else{
        elem = NULL;
    }
}

/*
 * Return the last element of the list.
 * @input l: Pointer to a struct list_t.
 * @elem: Pointer where the element will be stored
 */
void list_tail(list_t *l, void **elem)
{
    if(l->length > 0){
        *elem = l->tail->data;
    }
    else{
        elem = NULL;
    }
}

/*
 * Return the first element of the list.
 * @input l: Pointer to a struct list_t.
 * @elem: Pointer where the element will be stored
 */
void list_tail_node(list_t *l, void **elem)
{
    if(l->length > 0){
        *elem = l->tail;
    }
    else{
        elem = NULL;
    }
}


/*
 * Points the current element to head of the list.
 */
void list_reset(list_t *l)
{
    l->current = l->head; 
}

/*
 * Returns 1 if the current element is not NULL. 0 otherwise.
 * @input l: Pointer to a struct list_t.
 * @elem: Pointer where the current element will be stored
 */
long list_next(list_t *l, void **elem)
{
    long next = 0;

    if(l->current != NULL){
        l->rem_current = l->current;
        *elem = l->current->data;
        l->current = l->current->next;
        next = 1;
    }
    return(next);
}

/*
 * Returns 1 if the current element is not NULL. 0 otherwise.
 * @input l: Pointer to a struct list_t.
 * @elem: Pointer where the current element will be stored
 */
long list_prev(list_t *l, void **elem)
{
    long prev = 1;

    if(l->current != NULL){
        l->rem_current = l->current;
        *elem = l->current->data;
        l->current = l->current->prev;
    }
    else{
        prev = 0;
    }
    return(prev);
}

/*
 * @return l: Number of elements of the list.
 */
long list_length(list_t *l)
{
    return l->length;
}

/*
 * @return: 0 if l is empty, 1 otherwise.
 */
long list_empty(list_t *l){

    long empty = 0;

    if(l->length == 0){
        empty = 1;
    }

    return(empty);
}

/*
 * Remove the first element from the list.
 */
void list_rem_head(list_t *l)
{
    node_list *node;

    node = l->head;
    if(l->length == 1){
        l->head = NULL;
        l->tail = NULL;
    }
    else{
        l->head = l->head->next;
        l->head->prev = NULL;
    }
    l->length--;
    free(node->data);
    free(node);
}

/*
 * Remove the current element from the list.
 */
void list_rem_current(list_t *l)
{
    node_list *node;

    node = l->current;
    if(l->length == 1){
        l->head = NULL;
        l->tail = NULL;
        l->current = NULL;
    }
    else if(l->head == node){
        l->head = node->next;
        l->head->prev = NULL;        
        l->current = node->next;
    }
    else if(l->tail == node){
        l->tail = node->prev;
        l->tail->next = NULL;        
        l->current = NULL;
    }
    else{
        node->prev->next = node->next;
        node->next->prev = node->prev;
        l->current = node->next;
    }
    l->length--;
    free(node->data);
    free(node);
}

/*
 * Remove the last element from the list.
 */
void list_rem_tail(list_t *l)
{
    node_list *node;

    node = l->tail;
    if(l->length == 1){
        l->head = NULL;
        l->tail = NULL;
    }
    else{
        l->tail = l->tail->prev;     
        l->tail->next = NULL;
    }
    l->length--;
    free(node->data);
    free(node);
}


/*
 * Remove the elements from the bookmark to the end.
 */
void list_trunc(list_t *l)
{
    node_list *curr;

    if(l->bookmark != NULL){
        curr = l->bookmark->next;
        node_list *aux;
        while(curr != NULL) {
            aux = curr;
            curr = curr->next;
            free(aux->data);
            free(aux);
            l->length--;
        }
        l->tail = l->bookmark;
        l->bookmark->next = NULL;
    }
}

/* Concats two lists.
 * @input l1, l2: Two list
 * The result is stored in l1 and l2 can be reused.
 */
void list_concat(list_t *l1, list_t *l2)
{
    if(l2->length != 0){
        if(l1->length == 0){
            l1->head = l2->head;
            l1->tail = l2->tail;
        }
        else{
            l1->tail->next = l2->head;
            l2->head->prev = l1->tail;
            l1->tail = l2->tail;
        }
        l1->length += l2->length;
        l2->length = 0;
        l2->head = NULL;
        l2->tail = NULL;
        l2->bookmark = NULL;
    }
}

/*
 * Remove a node from the list.
 */
void list_rem_elem(list_t *l)
{
    struct node_list *node;
    node = l->rem_current;

    if(l->length == 1){
        l->head = NULL;
        l->tail = NULL;
        l->current = NULL;
    }
    else if(l->head == node){
        l->head = node->next;
        l->head->prev = NULL;        
        l->current = node->next;
    }
    else if(l->tail == node){
        l->tail = node->prev;
        l->tail->next = NULL;        
        l->current = NULL;
    }
    else{
        node->prev->next = node->next;
        node->next->prev = node->prev;
        l->rem_current = node->prev;
    }
    l->length--;
    free(node->data);
    free(node);
}

/*
 * Remove the first element from one list and appen it into another.
 */
void list_rem_head_append(list_t *sl, list_t *dl)
{
    node_list *node;

    node = sl->head;
    if(sl->length == 1){
        sl->head = NULL;
        sl->tail = NULL;
    }
    else{
        sl->head = sl->head->next;
        sl->head->prev = NULL;
    }
    sl->length--;

    if(dl->length == 0) {
        dl->head = node;
    } 
    else {
        node->prev = dl->tail; 
        dl->tail->next = node;
    }
    node->next = NULL;
    dl->tail = node;
    dl->length++;
}

/*
 * Remove a node from the list.
 */
void list_rem_node(list_t *l, node_list *node)
{

    if(l->length == 1){
        l->head = NULL;
        l->tail = NULL;
        l->current = NULL;
    }
    else if(l->head == node){
        l->head = node->next;
        l->head->prev = NULL;        
        l->current = node->next;
    }
    else if(l->tail == node){
        l->tail = node->prev;
        l->tail->next = NULL;        
        l->current = NULL;
    }
    else{
        node->prev->next = node->next;
        node->next->prev = node->prev;
        l->current = node->next;
    }
    l->length--;
    free(node->data);
    free(node);
}

void list_get_i(list_t *l, long num, void **elem){

    long current = 0;
    node_list *curr;

    if(num <0 || num > l->length - 1){
        printf("Error get: i out of bounds\n");
        exit(-1);
    }

    if(num == 0){
        *elem = l->head->data;
    }
    else if(num == l->length - 1){
        *elem = l->tail->data;
    }
    else{
        curr = l->head;
        while(current < num){
            curr = curr->next;
            current++;
        }
        *elem = curr->data;
    }
}

void list_copy(list_t *l_src, list_t *l_dst, long i, long j){

    long current = 0;
    node_list *curr;
    node_list *node;

    if(i < 0 || j > l_src->length - 1 || j < i){
        printf("Error copy: i out of bounds\n");
        exit(-1);
    }

    curr = l_src->head;

    while(current < i){
        curr = curr->next;
        current++;
    }
    while(current <= j){
        node = malloc(sizeof(node_list));
        node->data = malloc(l_dst->t_size);
        node->next = NULL;
        if(l_dst->head == NULL){
            node->prev = NULL;
            l_dst->head = node;
        } 
        else {
            node->prev = l_dst->tail; 
            l_dst->tail->next = node;
        }
        memcpy(node->data, curr->data, l_src->t_size);
        l_dst->tail = node;
        l_dst->length++;
        curr = curr->next;
        current++;
    } 
}

/**
 * Decides if two lists are equal until element i.
 * ONLY works with longs.
 */
long list_eq(list_t *l1, list_t *l2, long i){

    long current = 0;
    long eq = 0;
    node_list *curr1; 
    node_list *curr2; 

    if((i <= l1->length - 1) && i <= (l2->length - 1)){


        curr1 = l1->head;
        curr2 = l2->head;

        while((current) <= i && (*(long*)(curr1->data) == *(long*)(curr2->data))){
            curr1 = curr1->next;
            curr2 = curr2->next;
            current++;
        }
        if(current > i){
            eq = 1;
        }
    }
    return(eq);
}


/* Concats two lists.
 * @input l1, l2: Two list
 * The result is stored in l1 and l2 can be reused.
 */
void list_concat2(list_t *l_dst, list_t *l1, list_t *l2)
{
    if(l1->length == 0 || l2->length == 0){
        printf("Error: i out of bounds\n");
        exit(-1);
    }

    if(l1->length == 1){
        l1->head = l2->head;
    }
    else{
        l1->tail->prev->next = l2->head;
        l2->head->prev = l1->tail->prev;
    }
    free(l1->tail->data);
    free(l1->tail);
    l_dst->head = l1->head;
    l_dst->tail = l2->tail;
    l_dst->length = l1->length + l2->length - 1;
    l1->head = NULL;
    l2->head = NULL;
    l1->tail = NULL;
    l2->tail = NULL;
    l1->length = 0;
    l2->length = 0;

}

void destroy_list_list(list_t *l){

    list_t *l_aux;
    node_list *curr; 
    curr = l->head;

    while(curr != NULL){
        l_aux = (list_t*)(curr->data);
        list_destroy(l_aux);
        free(l_aux);
        curr = curr->next;
        free(curr);
    }
}




