#include "allocation_jellyfish.h"
#include "jellyfish_aux.h"
#include "jellyfish.h"
#include "../inrflow/applications.h"
#include "../inrflow/globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


extern long switches; 
extern long servers; 
extern long ports_switches;
extern long ports_servers;

long jellyfish_allocation_spread(application *app, long *cores){

    long i = 0;
    long k = 0;
    long l = 0;
    long m = 0;

    while(l < app->size){
        while(k < switches){
            if(sched_info->servers[i].cores[m] == -1){
                cores[l] = (i * server_cores) + m;
                l++;
                if(l == app->size){
                    break;
                }
            }
            i = (i + ports_servers) % servers;
            k++;
        }
        k = 0;
        i++;
    }
    return(1);
}

long jellyfish_allocation_random(application *app, long *cores){

    long i, j, k, l;
    long *sw_aux = NULL;
    long local_server, local_core;

    sw_aux = calloc(switches, sizeof(long));
    i = 0;
    j = rand() % switches;
    while(i < app->size){
        k = 0;
        while(sw_aux[j] == 1){
            j = rand() % switches;
        }
        sw_aux[j] = 1;
        while((i < app->size) && k < ports_servers){
            l = 0;
            while((i < app->size) && l < server_cores){
                local_server = (j * ports_servers) + k;
                local_core = (local_server * server_cores) + l;
                if(sched_info->servers[local_server].cores[l] == -1){
                    cores[i++] = local_core;
                }
                l++;
            }
            k++;
        }
    }
    free(sw_aux);
    return(1);
}



long jellyfish_allocation_locality(application *app, long *cores){

    long i = 0;
    long j = 0;
    long k = -1;
    long l = 0;
    long m = 0;
    long alloc = 0;
    long num_switches = 0;
    long num_av_servers = 0;
    long num_av_cores = 0;
    long n_dist_aux = 0;
    long *switches_dist = malloc(switches * sizeof(long));
    long *av_servers = malloc(ports_servers * sizeof(long));
    long *av_cores = malloc(server_cores * sizeof(long));
    long *total_cores = calloc(server_cores * ports_servers * switches, sizeof(long));

    //n_switches = (long)ceil((float)app->size / ports_servers);
    //n_dist = (long)ceil(log((float)n_switches)/log(ports_switches));

    while(num_av_servers == 0 && k < switches){
        num_av_servers = available_servers(++k, av_servers, ports_servers);
    }
    while( i < app->size){
        num_switches = nodes_distance_jellyfish(k, switches_dist, n_dist_aux);
        //printf("Distance: %ld %ld\n ",n_dist_aux, server_cores * ports_servers * switches);
        //print_vector_aux(num_switches, switches_dist);
        j = 0;
        while(!alloc && j < num_switches){
            num_av_servers = available_servers(switches_dist[j], av_servers, ports_servers);
            l = 0;
            while(!alloc && l < num_av_servers){
                num_av_cores = available_cores(av_servers[l], av_cores, ports_servers);   
                m = 0;
                while(m < num_av_cores && i < app->size){
                    if(total_cores[av_cores[m]] == 0){
                        total_cores[av_cores[m]] = 1;
                        cores[i++] = av_cores[m];
                    }
                    m++;
                }
                if(i == app->size){
                    alloc = 1;
                }
                l++;
            }
            j++;
        }
        n_dist_aux++;
    }
    free(switches_dist);
    free(av_servers);
    free(av_cores);
    free(total_cores);
    return(alloc);
}

long jellyfish_allocation_quasicontiguous(application *app, long *cores){

    long alloc = 0;

    return(alloc);
}

long jellyfish_allocation_contiguous(application *app, long *cores){

    long i = 0;
    long j = 0;
    long k = -1;
    long l = 0;
    long alloc = 0;
    long res = 0;
    long num_switches = 0;
    long n_switches;
    long num_av_servers = 0;
    long n_dist_aux = 0;
    long *switches_dist = malloc(switches * sizeof(long));
    long *av_servers = malloc(ports_servers * sizeof(long));
    //long *av_cores = malloc(server_cores * sizeof(long));

    n_switches = (long)ceil((float)app->size / ports_servers);
    //n_dist = (long)ceil(log((float)n_switches)/log(ports_switches));

    long *selected = malloc(n_switches * sizeof(long));
    long n_selected = 0;

    while(!alloc && k < switches - 1){
        num_av_servers = available_servers(++k, av_servers, ports_servers);
        if(num_av_servers == 0){
            continue;
        }
        n_dist_aux = 0;
        n_selected = 0;
        while(!alloc && (n_selected < n_switches && n_dist_aux < switches)){
            num_switches = nodes_distance_jellyfish(k, switches_dist, n_dist_aux);
            j = 0;
            while(j < num_switches){
                if(are_available_servers(switches_dist[j], ports_servers)){
                    res = check_selected_set(switches_dist[j], n_selected, selected);
                    if(res){
                        selected[n_selected++] = switches_dist[j];
                        if(n_selected == n_switches){
                            alloc = 1;
                            break;
                        }
                    }
                }
                j++;
            }
            n_dist_aux++;
        }
    }
    i = 0;
    j = 0;
    while(i < app->size){
        long server_id = selected[j] * ports_servers;
        l = 0;
        while(l < ports_servers){
            sched_info->servers[server_id + l].cont = 1;
            k = 0;
            while(k < server_cores){
                sched_info->servers[server_id + l].cores[k] = 1;
                cores[i++] = ((server_id + l) * ports_servers) + k;
                k++;
            }
            l++;
        }
        j++;
    }
    free(selected);
    free(switches_dist);
    free(av_servers);
    print_vector_aux(app->size, cores);
    return(alloc);
}

void print_vector_aux(long size, long *vec){

    long i;

    for(i = 0; i < size; i++){
        printf("%ld ", vec[i]);
    }
    printf("\n");
}
