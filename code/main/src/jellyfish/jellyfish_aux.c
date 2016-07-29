#include "../inrflow/list.h"
#include "jellyfish.h"
#include "allocation_jellyfish.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

long arrays_eq(long *a1, long *a2, long i){

    long j;
    long eq = 1;

    if(i <= a1[0] && i<=a2[0]){
        for(j = 0;j <= i; j++){
            if(a1[j+1] != a2[j+1]){
                eq = 0;
                break;
            }         
        }
    }
    return(eq);
}


/**
 * Decides if two lists are equal until element i.
 * ONLY works with longs.
 */
long list_is_in(list_t *l, long *l1){

    long current = 0;
    long eq = 0;
    long i = 0;
    long *l2;
    long **l2_aux;
    node_list *l_curr;

    l_curr = l->head;

    while(i < l->length){
        l2_aux = (long**)l_curr->data;
        l2 = *l2_aux;
        if(l2[0] != l1[0]){
            l_curr = l_curr->next;
            i++;
            continue;
        }

        current = 1;

        while(current < l1[0] && l1[current] == l2[current]){
            current++;
        }
        if(current == l1[0]){
            eq = 1;
            break;
        }
        else{
            l_curr = l_curr->next;
            i++;
        }
    }
    return(eq);
}

long list_shorter_from_a_to_b(list_t *B, long **l_dst, long ecmp, long shortest_path_length){

    long i = 0;
    long ret = 0;
    long **l2_aux;
    long *l2;
    long *l_aux = NULL;
    node_list *l_curr = NULL;
    node_list *l_curr_aux = NULL;
    long min = LONG_MAX;

    l_curr = B->head;

    while(i < B->length){
        l2_aux = (long**)l_curr->data;
        l2 = *l2_aux;
        if(l2[0] < min){
            min = l2[0];
            l_aux = l2;
            l_curr_aux = l_curr;
        } 
        l_curr = l_curr->next;
        i++;
    } 
    if(l_aux[0] <= shortest_path_length || ecmp == 0 ){
        ret = 1;
        *l_dst = l_aux;
        list_rem_node(B, l_curr_aux);
    }
    return(ret);
}

long select_path(list_t *B, long **l_dst, long n_current, long H, long ths){

    long i = 0;
    long ret = 0;
    long **l2_aux;
    long *l2;
    long *l_aux = NULL;
    node_list *l_curr = NULL;
    node_list *l_curr_aux = NULL;
    long min = LONG_MAX;

    l_curr = B->head;

    while(i < B->length){
        l2_aux = (long**)l_curr->data;
        l2 = *l2_aux;
        if(l2[0] < min){
            min = l2[0];
            l_aux = l2;
            l_curr_aux = l_curr;
        } 
        l_curr = l_curr->next;
        i++;
    } 
    if(l_aux[0] <= H){
        ret = 1;
        *l_dst = l_aux;
        list_rem_node(B, l_curr_aux);
    }
    else if((l_aux[0] > H) && n_current < ths){
        ret = 2;
        *l_dst = l_aux;
        list_rem_node(B, l_curr_aux);
    }
    else{
        ret = 0;
    }
    return(ret);
}

void print_array(long *arr){

    long i;

    for(i = 0; i < arr[0]; i++){
        printf("%ld - ", arr[i+1]);
    }
    printf("\n");
}

long check_selected_set(long candidate, long n_selected, long *selected){

    long j = 0;
    long i = 0;
    long res = 1;
    long *path = NULL;

    while(i < n_selected){
        path = get_path(candidate, selected[i], 0); 
        while(j < path[0]-1){
            if(selected[i] == path[j+2]){
                break;
            } 
            j++;
            if(j == n_selected){
                res = 0;
            }
        }
        i++;
    }
    return(res);
}







