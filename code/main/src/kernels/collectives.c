#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../inrflow/list.h"
#include "../inrflow/gen_trace.h"

/**
 * Prints a trace in the standard output performing a all-to-all interchange. (non-optimized)
 *
 *@param nodes	The number of nodes. It should be a power of two value, if not an error is printed in the error output.
 *@param size	The size of the burst.
 */
void all2all(application *app, list_t **task_events, long nodes, long size, long computation, long phases)
{
    long i, j, k;
    long tag_s;
    long tag_r;

    // Check whether th number of phases is given or not. If not, set to 1.
    if(phases == -1)
        phases = 1;

    for (k = 0; k < phases; k++) {
        tag_s = 0;
        for (i = 0; i < nodes; i++){	
            //insert_computation(app, task_events, i, computation);
            for (j = 0; j < nodes; j++){
                if(i != j)
                    insert_send(app, task_events, i, (i + j) % nodes, tag_s++, size);
            }
        }
        tag_r = 0;
        for (i = 0; i < nodes; i++){	
            for (j = 0; j < nodes; j++){
                if(i != j)
                    insert_recv(app, task_events, i, (i + j) % nodes, tag_r++, size);
            }
        }
    }
}

void many_all2all(application *app, list_t **task_events, long nodes, long size, long computation, long phases){

    long i, j;
    long src = 0;
    long dst = 0;
    long curr_group = 0;
    long group_size = app->pattern_params[1]; 
    long tag = 0;

    // Check whether the number of phases is given or not. If not, set to 1.
    if(phases == -1)
        phases = 1;


    for (j = 0; j < phases; j++){
        for(i = 0; i < nodes; i++){
            //insert_computation(app, task_events, 0, computation);
        }
        src = 0;
        dst = 0;
        curr_group = 0;
        tag = 0;
        while(src != -1 || dst != -1){
            insert_send(app, task_events, src, dst, tag++, size);
            src++;
            if(src % group_size == 0 || src == nodes) {
                src = curr_group * group_size;
	        dst++;
	        if(dst == nodes) {
	            src = -1;
	            dst = -1;
	        } 
                else if(dst % group_size == 0) {
	            curr_group++;
	            src = curr_group * group_size;
                }
            }
        }
        src = 0;
        dst = 0;
        curr_group = 0;
        tag = 0;
        while(src != -1 || dst != -1){
            insert_recv(app, task_events, src, dst, tag++, size);
            src++;
            if(src % group_size == 0 || src == nodes) {
                src = curr_group * group_size;
	        dst++;
	        if(dst == nodes) {
	            src = -1;
	            dst = -1;
	        } 
                else if(dst % group_size == 0) {
	            curr_group++;
	            src = curr_group * group_size;
                }
            }
        }

    }
}

void many_all2all_rnd(application *app, list_t **task_events, long nodes, long size, long computation, long phases){

    long i, j, r, tmp;
    long src = 0;
    long dst = 0;
    long curr_group = 0;
    long group_size = app->pattern_params[1]; 
    long *translate;
    long tag;
    
    // Check whether th number of phases is given or not. If not, set to 1.
    if(phases == -1)
        phases = 1;


    translate=malloc(nodes * sizeof(long));
    
    for (i = 0; i < nodes; i++) {
        translate[i] = i;
    }
    for (i = 0; i < nodes; i++) {
		r = rand() % nodes;
		tmp = translate[i];
		translate[i] = translate[r];
		translate[r] = tmp;
    }

    for (j = 0; j < phases; j++){
        tag = 0;
        for(i = 0; i < nodes; i++){
            //insert_computation(app, task_events, translate[i], computation);
        }
        src = 0;
        dst = 0;
        curr_group = 0;
        tag = 0;
        while(src != -1 || dst != -1){
            insert_send(app, task_events, translate[src], translate[dst], tag++, size);
            src++;
            if(src % group_size == 0 || src == nodes) {
                src = curr_group * group_size;
	        dst++;
	        if(dst == nodes) {
	            src = -1;
	            dst = -1;
	        } 
                else if(dst % group_size == 0) {
	            curr_group++;
	            src = curr_group * group_size;
                }
            }
        }
        src = 0;
        dst = 0;
        curr_group = 0;
        tag = 0;
        while(src != -1 || dst != -1){
            insert_recv(app, task_events, translate[src], translate[dst], tag++, size);
            src++;
            if(src % group_size == 0 || src == nodes) {
                src = curr_group * group_size;
	        dst++;
	        if(dst == nodes) {
	            src = -1;
	            dst = -1;
	        } 
                else if(dst % group_size == 0) {
	            curr_group++;
	            src = curr_group * group_size;
                }
            }
        }

    }
    free(translate);
}

/**
 * Prints a trace in the standard output performing a one-to-all interchange. (non-optimized bcast)
 *
 *@param nodes	The number of nodes.
 *@param size	The size of the burst.
 */
void one2all(application *app, list_t **task_events, long nodes, long size, long computation, long phases)
{
    long i, l;
    long tag_s, tag_r;

    // Check whether th number of phases is given or not. If not, set to 1.
    if(phases == -1)
        phases = 1;

    for (l = 0; l < phases; l++){
        tag_s = 0;
        tag_r = 0;
        for (i = 1; i < nodes; i++)	
        {	
            insert_send(app, task_events, 0, i, tag_s++, size);
            insert_recv(app, task_events, 0, i, tag_r++, size);
        }
    }
}

/**
 * Prints a trace in the standard output performing a one-to-all interchange. (non-optimized bcast)
 *
 *@param nodes	The number of nodes.
 *@param size	The size of the burst.
 */
void one2all_rnd(application *app, list_t **task_events, long nodes, long size, long computation, long phases)
{
    long i, l;
    long src = rand() % nodes;
    long tag_s, tag_r;

    // Check whether th number of phases is given or not. If not, set to 1.
    if(phases == -1)
        phases = 1;

    for (l = 0; l < phases; l++){
        tag_s = 0;
        tag_r = 0;
        for (i = 0; i < nodes; i++)	
        {	
            if(src != i){
                insert_send(app, task_events, src, i, tag_s++, size);
                insert_recv(app, task_events, src, i, tag_r++, size);
            }
        }
    }
}

/**
 * Prints a trace in the standard output performing a all-to-one interchange. (non-optimized reduce)
 *
 *@param nodes	The number of nodes.
 *@param size	The size of the burst.
 */
void all2one(application *app, list_t **task_events, long nodes, long size, long computation, long phases)
{
    long i, l;
    long tag_s, tag_r;

    // Check whether th number of phases is given or not. If not, set to 1.
    if(phases == -1)
        phases = 1;


    for (l = 0; l < phases; l++){
        tag_s = 0;
        tag_r = 0;
        for (i = 1; i < nodes; i++)	
        {	
            insert_send(app, task_events, i, 0, tag_s++, size);
            insert_recv(app, task_events, i, 0, tag_r++, size);
        }
    }

}

/**
 * Prints a trace in the standard output performing a all-to-one interchange. (non-optimized reduce)
 *
 *@param nodes	The number of nodes.
 *@param size	The size of the burst.
 */
void all2one_rnd(application *app, list_t **task_events, long nodes, long size, long computation, long phases)
{
    long i, l;
    long dst = rand() % nodes;
    long tag_s, tag_r;

    // Check whether th number of phases is given or not. If not, set to 1.
    if(phases == -1)
        phases = 1;

    for (l = 0; l < phases; l++){
        tag_s = 0;
        tag_r = 0;
        for (i = 0; i < nodes; i++)	
        {	
            if(dst != i){
                insert_send(app, task_events, i, dst, tag_s++, size);
                insert_recv(app, task_events, i, dst, tag_r++, size);
            }
        }
    }
}

/**
 * Prints a trace in the standard output performing a all-to-one interchange. (non-optimized reduce)
 *
 *@param nodes	The number of nodes.
 *@param size	The size of the burst.
 */
void ptp(application *app, list_t **task_events, long nodes, long size, long computation, long phases)
{
    long l;

    // Check whether th number of phases is given or not. If not, set to 1.
    if(phases == -1)
        phases = 1;

    for (l = 0; l < 1; l++){
        insert_send(app, task_events, 0, 1, 0, 30000);
        insert_recv(app, task_events, 0, 1, 0, 30000);
        insert_send(app, task_events, 1, 0, 1, 60000);
        insert_recv(app, task_events, 1, 0, 1, 60000);

    }
}

/**
 * Initializes the random pattern. It requires 1 parameter: the total number of flows. Flows are generated independently: source and destination are selected randomly using a uniform distribution.
 * @param nservers The number of communicating servers
 * @param nparam The number of parameters in the list of parameters
 * @param params A list with the parameters for the traffic pattern
 */
void randomapp(application *app, list_t **task_events, long nodes, long size, long computation, long phases)
{
    long i, l, tag_s, tag_r,seed, src, dst;
    char statenew[256];
    char *stateold;

    // Check whether th number of phases is given or not. If not, set to 1.
    if(phases == -1)
        phases = 1;

    seed = rand() % nodes;
    for (l = 0; l < phases; l++){
        stateold = initstate(seed, statenew, 256);
        tag_s = 0;
        for(i = 0; i < app->pattern_params[1];i++){
            src = random()%nodes;
            dst = random()%nodes;
            insert_send(app, task_events, src, dst, tag_s++, size);
        }
	setstate(stateold);
        stateold = initstate(seed, statenew, 256);
        tag_r = 0;
        for(i = 0; i < app->pattern_params[1];i++){
            src = random()%nodes;
            dst = random()%nodes;
            insert_recv(app, task_events, src, dst, tag_r++, size);
        }
	setstate(stateold);
    }
}

