#include "metrics.h"
#include "list.h"
#include "globals.h"
#include <stdlib.h>
#include <stdio.h>

void init_metrics(struct metrics_t *metrics){

    //Scheduling metrics
    metrics->scheduling.makespan = 0;
    metrics->scheduling.avg_waiting_time = 0.0;
    
    //Applications metrics
    metrics->applications.n_apps = 0;
    list_initialize(&metrics->applications.apps, sizeof(struct app_metrics));
    
    //Execution metrics
    metrics->execution.runtime = 0;
    metrics->execution.avg_agg_bw = 0.0;
    metrics->execution.n_steps = 0;
    metrics->execution.links_info = NULL;
    list_initialize(&metrics->execution.agg_bw, sizeof(float));
}

void set_makespan(struct metrics_t *metrics, unsigned long long makespan){
     metrics->scheduling.makespan = makespan;
}

unsigned long long get_makespan(struct metrics_t *metrics){
    return( metrics->scheduling.makespan);
}

void report_metrics(struct metrics_t *metrics){
    
    long i;
    FILE *fd_sched, *fd_applications, *fd_execution, *fd_list_applications;
    char sched_filename[100]; 
    char applications_filename[100]; 
    char list_applications_filename[100]; 
    char execution_filename[100]; 
    app_metrics *a_m = NULL;
    float *agg_bw = NULL;

    snprintf(sched_filename,100,"%s_%s_%s_fr%.2f_injmode%d_seed%ld.%s.scheduling", get_network_token(), get_filename_params(), get_routing_token(), failure_rate, flow_inj_mode,r_seed, applications_file);
    snprintf(applications_filename,100,"%s_%s_%s_fr%.2f_injmode%d_seed%ld.%s.applications", get_network_token(), get_filename_params(), get_routing_token(), failure_rate, flow_inj_mode, r_seed, applications_file);
    snprintf(list_applications_filename,100,"%s_%s_%s_fr%.2f_injmode%d_seed%ld.%s.list_applications", get_network_token(), get_filename_params(), get_routing_token(), failure_rate, flow_inj_mode, r_seed, applications_file);
    snprintf(execution_filename,100,"%s_%s_%s_fr%.2f_injmode%d_seed%ld.%s.execution", get_network_token(), get_filename_params(), get_routing_token(), failure_rate, flow_inj_mode, r_seed, applications_file);

    if((fd_sched = fopen(sched_filename, "w")) == NULL){
        printf("Error opening the scheduling file.\n");  
        exit(-1);
    }
    fprintf(fd_sched,"Makespan: %llu\n", metrics->scheduling.makespan);

    fclose(fd_sched);
    
    if((fd_applications = fopen(applications_filename, "w")) == NULL){
        printf("Error opening the applications file.\n");  
        exit(-1);
    }
    fprintf(fd_applications,"Number of applications: %ld\n", metrics->applications.n_apps);

    fclose(fd_applications);

    if((fd_list_applications = fopen(list_applications_filename, "w")) == NULL){
        printf("Error opening the applications file.\n");  
        exit(-1);
    }
    for(i =0; i < metrics->applications.n_apps;i++){
        list_head(&metrics->applications.apps, (void*)&a_m);
        fprintf(fd_applications,"Id:                     %ld\n", a_m->id);
        fprintf(fd_applications,"Arrive time:            %llu\n", a_m->arrive_time);
        fprintf(fd_applications,"Start time:             %llu\n", a_m->start_time);
        fprintf(fd_applications,"Finish time:            %llu\n", a_m->end_time);
        fprintf(fd_applications,"Runtime:                %llu\n", a_m->runtime);
        fprintf(fd_applications,"Waiting time:           %llu\n", a_m->waiting_time);
        fprintf(fd_applications,"Number of flows:        %ld\n", a_m->n_flows);
        fprintf(fd_applications,"Total distance:         %ld\n", a_m->flows_distance);
        fprintf(fd_applications,"Average flows distance: %.5f\n", a_m->avg_flows_distance);
        fprintf(fd_applications,"Total flows latency:    %.ld\n", a_m->flows_latency);
        fprintf(fd_applications,"Average flows latency:  %.5f\n", a_m->avg_flows_latency);
        fprintf(fd_applications,"\n-----------------------------------------\n");
        list_rem_head(&metrics->applications.apps);
    }

    fclose(fd_list_applications);
    
    if((fd_execution = fopen(execution_filename, "w")) == NULL){
        printf("Error opening the execution file.\n");  
        exit(-1);
    }
    fprintf(fd_execution,"Runtime:                      %llu\n", metrics->execution.runtime);
    fprintf(fd_execution,"Average latency:              %.5f\n", metrics->execution.avg_latency);
    fprintf(fd_execution,"Aggregated bandwidth:         %.5f\n", metrics->execution.avg_agg_bw);
    fprintf(fd_execution,"Number of steps:               %ld\n", metrics->execution.n_steps);
    fprintf(fd_execution,"Number of links shared:             ");
    for(i = 0; i < metrics->applications.n_apps + 1; i++){
        fprintf(fd_execution, "%ld ", metrics->execution.links_info[i]);
    }
    fprintf(fd_execution,"\n");
    free(metrics->execution.links_info);
    fprintf(fd_execution,"Dynamics aggregated bandwith: ");
    while(list_length(&metrics->execution.agg_bw) > 0){
        list_head(&metrics->execution.agg_bw, (void*)&agg_bw);
        fprintf(fd_execution, "%.5f ", *agg_bw);
        list_rem_head(&metrics->execution.agg_bw);
    }    
    fprintf(fd_execution,"\n");
    fclose(fd_execution);
}

void update_metrics(struct metrics_t *metrics){
    
    long i,j;

    metrics->execution.links_info = malloc((metrics->applications.n_apps + 1) * sizeof(long));
    for(i = 0; i < metrics->applications.n_apps + 1; i++){
        metrics->execution.links_info[i] = 0;
    }
    for(i=0; i<servers+switches; i++) {
        network[i].nports=get_radix(i);
        for(j=0; j<network[i].nports; j++) {
            metrics->execution.links_info[network[i].port[j].link_info[0]]++;

        }
    }

}
