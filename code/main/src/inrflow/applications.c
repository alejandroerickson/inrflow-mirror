#include "applications.h"
#include "dynamic_engine.h"
#include "allocation.h"
#include "gen_trace.h"
#include "list.h"
#include "io.h"
#include "metrics.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>

void init_workload(list_t *workload){

    list_initialize(workload, sizeof(application));

    switch(applications_type){

        case NONE_APP:
            break;
        case FILE_APP:
            read_applications_from_file(applications_file);
            break;
        default:
            printf("There is no policy to generate applications.\n");
            exit(-1);
    }
}

void init_running_application(application *next_app){

    long i;
    long n_events;
    node_list *node;

    next_app->task_events = malloc(sizeof(list_t*) * next_app->size);
    next_app->task_events_occurred = malloc(sizeof(list_t*) * next_app->size);
    for(i = 0;i < next_app->size; i++){
        next_app->task_events[i] = malloc(sizeof(list_t));
        next_app->task_events_occurred[i] = malloc(sizeof(list_t));
        list_initialize(next_app->task_events[i], sizeof(event));
        list_initialize(next_app->task_events_occurred[i], sizeof(event));
    }
    gen_trace(next_app);
    next_app->remaining_flows = malloc(sizeof(long) * next_app->size);
    for(i = 0;i < next_app->size; i++){
        next_app->remaining_flows[i] = 0;
        n_events = insert_new_events(next_app, i);
        if(n_events == 0){
            next_app->tasks_finished++;
        }
    }
    list_head_node(&workload, (void*)&node);
    next_app->node = node; 
    next_app->info.start_time = sched_info->makespan;
    next_app->info.waiting_time =  (next_app->info.start_time -  next_app->info.arrive_time);
    list_rem_head_append(&workload, &running_applications);
    if(verbose){
        printf("Application %ld starts at makespan %llu\n",next_app->info.id, sched_info->makespan);
    }    
}

void finish_application(application *app){

    long i;
    if(verbose){
        printf("Application %ld finishes at makespan %llu\n",app->info.id, sched_info->makespan);
    }
    metrics.applications.n_apps++;
    app->info.end_time = sched_info->makespan;
    app->info.runtime = (app->info.end_time - app->info.start_time);
    app->info.avg_flows_distance = ((float)app->info.flows_distance / (float)app->info.n_flows);
    app->info.avg_flows_latency = ((float)app->info.flows_latency / (float)app->info.n_flows);
    list_append(&metrics.applications.apps, &app->info);
    for(i = 0; i < app->size; i++){
        list_destroy(app->task_events[i]);
        list_destroy(app->task_events_occurred[i]);
        free(app->task_events[i]);
        free(app->task_events_occurred[i]);
    
    }
    free(app->remaining_flows);
    free(app->task_events);
    free(app->task_events_occurred);
    release_application(app);
    list_rem_node(&running_applications, app->node);
}





