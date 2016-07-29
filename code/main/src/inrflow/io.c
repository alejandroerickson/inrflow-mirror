#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "globals.h"
#include "applications.h"
#include "misc.h"
#include "literal.h"
#include "list.h"

long load_mapping_from_file(char *file, long ntasks, long *translation)
{
    FILE *fd;
    long node;
    long i = -1;

    if((fd = fopen(file, "r")) == NULL){
        printf("Error opening the mapping file.\n");  
        exit(-1);
    }

    while (!feof (fd) && fscanf(fd, "%ld", &node) && i++ < ntasks ){
        translation[i] = node;
    }
    fclose(fd);
    return(i); 
}

void read_applications_from_file(char *file){

    FILE *fd;
    long i;
    char pattern[50];
    char *pattern_aux;
    long servers = get_servers();
    application app;

    app.info.start_time = 0;
    app.info.end_time = 0;
    app.info.n_flows = 0;
    app.info.flows_distance = 0;
    app.info.flows_latency = 0;
    app.packets = 8192;
    app.comp_time = 10;
    app.phases = 1;
    app.running = 0;
    app.tasks_finished = 0;
    app.info.id = 1;

    if((fd = fopen(file, "r")) == NULL){
        printf("Error opening the workload file.\n");  
        exit(-1);
    }

    while(!feof(fd)){  
        fscanf(fd,"%llu %s %ld %s\n",&app.info.arrive_time, pattern, &app.size, app.allocation_type); 
        if(app.size > servers){
            printf("WARNING: Application %ld with size %ld is larger than the network: %ld.\n",app.info.id,app.size, servers);
            exit(-1);
        } 
        pattern_aux = strtok(pattern, "_");
        if(!literal_value(tpatterns_l, pattern_aux, (int *) &app.pattern)) {
            printf("Error: Unknown traffic pattern - %s\n", pattern_aux);
            exit(-1);
        }
        app.pattern_nparam=0;
        if(app.pattern == FILE_PATTERN){
            pattern_aux = strtok(NULL, "_");
            sscanf(pattern_aux, "%s", app.pattern_file);
        }
        
            while ( (pattern_aux = strtok(NULL, "_")) && app.pattern_nparam < MAX_TRAFFIC_PARAMS){
                app.pattern_params[app.pattern_nparam++] = atoi(pattern_aux);
            }
        
        for (i = app.pattern_nparam; i < MAX_TRAFFIC_PARAMS; i++)
            app.pattern_params[i] = -1; // undefine values, should not be checking these anyway.

        if(!literal_value(allocation_l, app.allocation_type, (int *) &app.allocation)) {
            printf("Error: Unknown allocation pattern - %s\n", app.allocation_type);
            exit(-1);
        }
        app.phases = app.pattern_params[0];
        list_append(&workload, &app);
        app.info.id++;
    }  
    fclose(fd);
}
