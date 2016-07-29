#include "applications.h"
#include "scheduling.h"
#include "globals.h"
#include "../jellyfish/allocation_jellyfish.h"
#include <stdio.h>
#include <stdlib.h>

long allocate_application(application *next_app){

    long i, j, k;
    long alloc =0;
    long *cores = NULL;
    long *cores_aux = NULL;
    long local_server, local_core;

    cores = malloc(sizeof(long) * next_app->size);
    switch(next_app->allocation){
        case SEQUENTIAL_ALLOC:
            i=0;
            k = 0;
            if(sched_info->free_cores >=  next_app->size){
                while(!alloc && i < sched_info->total_servers){
                    j = 0;
                    while(!alloc && j < server_cores){
                        if(sched_info->servers[i].cores[j] == -1){
                            cores[k++] = (i * server_cores) + j;
                            if(k == next_app->size){
                                alloc = 1;
                            }
                        }
                        j++;
                    } 
                    i++;
                } 
            }
            break;
        case RANDOM_ALLOC:
            if(sched_info->free_cores >=  next_app->size){
                cores_aux = calloc(sched_info->total_cores, sizeof(long));
                i = 0;
                j = rand() % sched_info->total_servers;
                k = rand() % server_cores;
                while(i < next_app->size){
                    local_core = (j * server_cores) + k;
                    if(sched_info->servers[j].cores[k] == -1 && cores_aux[local_core] == 0){
                        cores_aux[local_core] = 1;
                        cores[i++] = local_core;
                    }
                        j = rand() % sched_info->total_servers;
                        k = rand() % server_cores;
                }
                free(cores_aux);
                alloc = 1;
            }
            break;
        case JELLYFISH_SPREAD_ALLOC:
            if(sched_info->free_cores >=  next_app->size){
                alloc = jellyfish_allocation_spread(next_app, cores); 
            }
            break;
        case JELLYFISH_RANDOM_ALLOC:
            if(sched_info->free_cores >=  next_app->size){
                alloc = jellyfish_allocation_random(next_app, cores); 
            }
            break;
        case JELLYFISH_CONTIGUITY_ALLOC:
            if(sched_info->free_cores >=  next_app->size){
                alloc = jellyfish_allocation_contiguous(next_app, cores); 
            }
            break;
        case JELLYFISH_LOCALITY_ALLOC:
            if(sched_info->free_cores >=  next_app->size){
                alloc = jellyfish_allocation_locality(next_app, cores); 
            }
            break;
        case JELLYFISH_QUASICONTIGUITY_ALLOC:
            if(sched_info->free_cores >=  next_app->size){
                alloc = jellyfish_allocation_quasicontiguous(next_app, cores); 
            }
            break;
        default:
            printf("Unkwown allocation strategy.\n");
            exit(-1);
    }

    if(alloc == 1){
        for(i = 0; i < next_app->size; i++){
            local_server = cores[i] / server_cores;
            local_core = cores[i] % server_cores;
            sched_info->servers[local_server].cores[local_core] = next_app->info.id;

            sched_info->servers[local_server].free_cores--;
            sched_info->servers[local_server].busy_cores++;
        }
        next_app->translation = cores;
        sched_info->busy_cores += next_app->size;
        sched_info->free_cores -= next_app->size;
    }
    else{
        free(cores);
    }
    return(alloc);
}

void release_application(application *app){

    long i;
    long local_server, local_core;

    for(i = 0; i < app->size; i++){
        local_server = app->translation[i] / server_cores;
        local_core = app->translation[i] % server_cores;
        sched_info->servers[local_server].cores[local_core] = -1;
        sched_info->servers[local_server].free_cores++;
        sched_info->servers[local_server].busy_cores--;
        if(sched_info->servers[local_server].cont == 1){
            sched_info->servers[local_server].cont = 0;
        }

    }
    free(app->translation);
    sched_info->busy_cores -= app->size;
    sched_info->free_cores += app->size;
}



