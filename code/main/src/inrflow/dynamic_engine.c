#include "applications.h"
#include "dynamic_engine.h"
#include "list.h"
#include "metrics.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "float.h"
#include <math.h>

metrics_t metrics;
list_t list_cpus;
list_t list_flows;
long dmetrics_step;
float agg_bw;
long steps;
/**
 * Performs a dynamic run in which flows are placed following the workload description (message causality and cpu periods).
 * Workloads are described simply by sends, receives and cpu bursts:
 *
 * Send: insert flow(s) into the network.
 * Wait: stall the execution until the required flows are received.
 * CPU: stall the execution for a determined number of time units.
 */
void run_dynamic()
{
    unsigned long long time_next_app;

    agg_bw = 0.0;
    steps = 0;
    dmetrics_step = 1;
    init_workload(&workload);
    init_scheduling(servers);
    init_metrics(&metrics);
    list_initialize(&list_cpus, sizeof(event*));
    list_initialize(&list_flows, sizeof(event*));

    while(list_length(&workload) > 0 || list_length(&running_applications) > 0){
        time_next_app = schedule_next_application();
        update_events(time_next_app);
    }
    set_makespan(&metrics, sched_info->makespan);
    finish_scheduling(servers);
    update_metrics(&metrics);
    report_metrics(&metrics);
}

/**
 * Calculates the time in which the first event (CPU, flow sent or appication arrival) will finish or occur.
 * Then updates all events until that time.
 */
void update_events(unsigned long long time_next_app)
{

    long t_next;

    t_next = time_next_event(&list_cpus, &list_flows);

    if((time_next_app - sched_info->makespan > 0) && (time_next_app - sched_info->makespan < t_next)){
        t_next = time_next_app - sched_info->makespan;
    }

    if(dmetrics_time > 0){
        if((sched_info->makespan + t_next) / dmetrics_time >= dmetrics_step ){
            t_next = (dmetrics_time * dmetrics_step) - sched_info->makespan;
            dmetrics_step++;
        }
    }

    sched_info->makespan += t_next;
    if(verbose == 1){
        printf("Makespan: %llu Flows sent: %ld\n",sched_info->makespan, list_length(&list_flows));
    }
    update_cpus(&list_cpus, t_next);
    update_flows(&list_flows, t_next);
}

/**
 * Calculates the time in which the first event (CPU, flow sent) will finish or occur.
 */
long time_next_event(list_t *list_cpus, list_t *list_flows){

    long i;
    long min = LONG_MAX;
    long time_next = 0;
    application *app = NULL;
    event *ev = NULL;
    event **ev_p = NULL;
    dflow_t *flow = NULL;

    list_reset(&running_applications);
    while(list_next(&running_applications, (void*)&app)){
        for(i = 0; i < app->size; i++){
            list_reset(app->task_events_occurred[i]);
            while(list_next(app->task_events_occurred[i],(void*)&ev)){
                switch(ev->type){
                    case COMPUTATION:
                        if(ev->count == ev->length){
                            list_append(list_cpus, &ev);
                        }
                        break;
                    case SENDING:
                        if(ev->count == ev->length){
                            ev->dflow.start_time = sched_info->makespan;
                            flow = &ev->dflow;
                            app->info.flows_distance += mark_route_dynamic(app->info.id, &flow, get_server_i(app->translation[ev->pid]), get_server_i(app->translation[ev->pid2]));
                            //                            printf("D: %ld %ld --> %ld\n",app->info.flows_distance, get_server_i(app->translation[ev->pid]),get_server_i(app->translation[ev->pid2]));
                            list_insert(list_flows, &ev);
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }
    list_reset(list_cpus);
    while(list_next(list_cpus, (void*)&ev_p)){
        if((*ev_p)->count < min){
            min = (*ev_p)->count;
        }
    }

    min_links_bandwidth();

    list_reset(list_flows);
    while(list_next(list_flows, (void*)&ev_p)){
        time_next = (long)ceilf((*ev_p)->count / (*ev_p)->dflow.speed);
        if(time_next < min){
            min = time_next;
        }
    }
    return(min);
}

/**
 * Insert new events from a given task.
 */
long insert_new_events(application *app, long ntask){

    long done = 0;
    long num = 0;

    event *ev;
    list_reset(app->task_events[ntask]);
    while(!done && list_next(app->task_events[ntask], (void*)&ev)){
        switch(ev->type){
            case COMPUTATION:
                done = 1;
                if(num == 0 && list_length(app->task_events_occurred[ntask]) == 0){
                    list_append(app->task_events_occurred[ntask], ev);
                    list_rem_elem(app->task_events[ntask]);
                    num++;
                }
                else if( list_length(app->task_events[ntask]) > 0){
                    num = 1;
                }
                break;
            case SENDING:
                if(flow_inj_mode > app->remaining_flows[ev->pid] || flow_inj_mode == 0){
                    app->info.n_flows++;
                    app->remaining_flows[ev->pid]++;
                    if(flow_inj_mode == app->remaining_flows[ev->pid])
                        done = 1;
                    list_append(app->task_events_occurred[ntask], ev);
                    list_rem_elem(app->task_events[ntask]);
                    num++;
                }
                break;
            case RECEPTION:
                num++;
                done = 1;
                break;
            default:
                printf("Unknown event type\n.");
                exit(-1);
        }
    }
    return(num);
}

/**
 * Update the remaining time of the CPU events using the previously calculated time.
 * Checks if a task has finished and, when all tasks have finished, finish the application.
 */
void update_cpus(list_t *list_cpus, long t_next){

    event **ev = NULL;
    long num = 0;

    list_reset(list_cpus);
    while(list_next(list_cpus, (void*)&ev)){
        (*ev)->count -= t_next;
        if((*ev)->count == 0){
            num = insert_new_events((*ev)->app, (*ev)->pid);
            if(num == 0 && (*ev)->app->remaining_flows[(*ev)->pid] == 0){
                (*ev)->app->tasks_finished++;
                if((*ev)->app->tasks_finished == (*ev)->app->size){
                    finish_application((*ev)->app);
                }
            }
            list_rem_head((*ev)->app->task_events_occurred[(*ev)->pid]);
            list_rem_elem(list_cpus);
        }
    }
}

/**
 * Update the remaining time of the SEND events using the previously calculated time.
 * Checks if a task has finished and, when all tasks have finished, finish the application.
 */
void update_flows(list_t *list_flows, long t_next){

    event **ev = NULL;
    application *app;
    long pid, pid2;
    long num_ev_send, num_ev_rec;

    list_reset(list_flows);
    while(list_next(list_flows, (void*)&ev)){
        (*ev)->count -= ((*ev)->dflow.speed * t_next);
        (*ev)->dflow.speed = FLT_MAX;
        if((*ev)->count <= 0){
            app = (*ev)->app;
            pid = (*ev)->pid;
            pid2 = (*ev)->pid2;
            app->info.flows_latency += (sched_info->makespan - (*ev)->dflow.start_time);
            app->remaining_flows[pid]--;
            remove_flow(&(*ev)->dflow);
            remove_reception_event((*ev)->app->task_events[pid2], (*ev)->id, pid2, pid, (*ev)->length);
            remove_send_event(app->task_events_occurred[pid], (*ev)->id, pid, pid2, (*ev)->length);
            num_ev_rec = insert_new_events(app, pid2);
            num_ev_send = insert_new_events(app, pid);
            if( num_ev_send == 0 || num_ev_rec == 0 ){
                if (num_ev_send == 0 && app->remaining_flows[pid] == 0){
                    app->tasks_finished++;
                }
                if(num_ev_rec == 0 && app->remaining_flows[pid2] == 0 && (pid != pid2)){
                    app->tasks_finished++;
                }
                if(app->tasks_finished == app->size){
                    finish_application(app);
                }
            }
            list_rem_elem(list_flows);
        }
    }
}

/**
 * Follows the route between servers i & j and update flow counts.
 * Side effects: If a path exists, update link_hops and server_hops
 *
 * @return 1 if there is a path between them, 0 otherwise.
 */
long mark_route_dynamic(long id, dflow_t **flow, long src, long dst)
{
    long current = src;
    long path_length = 0;
    node_list *node;
    long next_port;
    route_t path;

    if(init_routing(src, dst) == -1){
        finish_route();
        return 0; // this pair of servers are disconnected;
    }
    while(current != dst) {	// not at destination yet
        next_port = route(current, dst);
        path_length++;
        path.node = current;
        path.port = next_port;
        network[current].port[next_port].flows++;
        if(network[current].port[next_port].link_info[id] == 0){
            network[current].port[next_port].link_info[0]++;
        }
        network[current].port[next_port].link_info[id]++;
        list_append(&network[current].port[next_port].dflows, (void*)&(*flow));
        list_tail_node(&network[current].port[next_port].dflows, (void*)&node);
        path.n_l = node;
        list_append(&(*flow)->path, &path);
        current = network[current].port[next_port].neighbour.node;
    }
    finish_route();
    return(path_length);
}

void remove_flow(dflow_t *flow){

    route_t *path;

    list_reset(&flow->path);
    while(list_next(&flow->path, (void*)&path)){
        network[path->node].port[path->port].flows--;
        list_rem_node(&network[path->node].port[path->port].dflows, path->n_l);
        list_rem_elem(&flow->path);
    }
}

long remove_send_event(list_t *events, long id, long pid, long pid2, long length){

    long finish = 0;
    event *ev = NULL;

    list_reset(events);
    while(list_next(events, (void*)&ev)){
        if(ev->id == id && ev->type == SENDING && ev->pid == pid && ev->pid2 == pid2 && ev->length == length){
            finish = 1;
            list_rem_elem(events);
            break;
        }
    }
    return(finish);
}

long remove_reception_event(list_t *events, long id, long pid, long pid2, long length){

    long finish = 0;
    event *ev = NULL;

    list_reset(events);
    while(list_next(events, (void*)&ev)){
        if(ev->id == id && ev->type == RECEPTION && ev->pid == pid && ev->pid2 == pid2 && ev->length == length){
            finish = 1;
            list_rem_elem(events);
            break;
        }
    }
    return(finish);
}

void min_links_bandwidth(){

    long i,j;
    long links = 0;
    float total_link_bw = 0.0;
    float link_bw = 0.0;
    float l_bandwidth = 0;
    float bandwidth = 0;
    float min_speed = FLT_MAX;
    list_t *dflows;
    dflow_t **flow;

    for(i=0; i<servers+switches; i++) {
        //network[i].nports=get_radix(i);
        for(j=0; j<network[i].nports; j++) {
            if(network[i].port[j].neighbour.node == -1 || network[i].port[j].neighbour.port == -1)
                continue;
            links++;
            link_bw = 0;
            l_bandwidth = (float)network[i].port[j].bandwidth_capacity;
            bandwidth = l_bandwidth / network[i].port[j].flows;
            dflows = &network[i].port[j].dflows;
            if(bandwidth < min_speed)
                min_speed = bandwidth;
            list_reset(dflows);
            while(list_next(dflows, (void*)&flow)){
                if(bandwidth < (*flow)->speed){
                    (*flow)->speed = bandwidth;
                }
                link_bw += (*flow)->speed;
            }
            total_link_bw += (link_bw / (float)l_bandwidth);
        }
    }
    agg_bw += (((total_link_bw / links) - agg_bw) / (float)(++steps));
    metrics.execution.avg_agg_bw += ((link_bw - metrics.execution.avg_agg_bw) / (float)(++metrics.execution.n_steps));
    if((dmetrics_time > 0 && (sched_info->makespan % dmetrics_time) == 0)){
        list_append(&metrics.execution.agg_bw, &agg_bw);
        agg_bw = 0.0;
        steps = 0;
    }

}


