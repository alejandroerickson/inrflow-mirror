#include "globals.h"
#include "misc.h"
#include "topo.h"
#include "placement.h"
#include "reporting.h"

#include <time.h>
#include <stdlib.h>
#include <unistd.h>

long* partial_path=NULL;	///< store the path before updating the
long max_path_length=16;    ///< the maximum length of the path. will need updating depending on the topology
long current_path_length=0; ///< current hop

#ifdef MEASURE_ROUTING_TIME
struct timespec prerouting_start;   ///< stores the starting time for prerouting @see init_routing. To be used later on to measure routing computation time.
struct timespec prerouting_end;     ///< stores the starting time for prerouting @see init_routing. To be used later on to measure routing computation time.
long long total_prerouting_time=0;  ///< stores the aggregated prerouting time.
long prerouting_count=0;              ///< stores how many times we have prerouted.

struct timespec hoprouting_start;   ///< stores the starting time for per hop routing @see route. To be used later on to measure routing computation time.
struct timespec hoprouting_end;     ///< stores the starting time for per hop routing @see route. To be used later on to measure routing computation time.
long long total_hoprouting_time=0;  ///< stores the aggregated hoprouting time.
long hoprouting_count=0;              ///< stores how many times we have hoprouted.

bool_t measuring;   ///< 0 if any error occurred.
long measured;      ///< measured time in nsecs.
#endif

/**
 * Follows the route between servers i & j and update flow counts.
 * Side effects: If a path exists, update link_hops and server_hops
 *
 * @return 1 if there is a path between them, 0 otherwise.
 */
long mark_route(long src, long dst)
{
	long current=src;
	long next_port;
	long lh=0, sh=0; ///< link and server hops
	long i;

#ifdef MEASURE_ROUTING_TIME
    measuring=1;
    if(clock_gettime(CLOCK_MODE, &prerouting_start)!=0){
        perror("Error Measuring routing time.");
        measuring=0;
    }
#endif

	if (init_routing(src, dst)==-1){
		finish_route();
		return 0; // this pair of servers are disconnected;
	}
#ifdef MEASURE_ROUTING_TIME
    if(clock_gettime(CLOCK_MODE , &prerouting_end)!=0){
        perror("Error Measuring routing time.");
        measuring=0;
    }

    if (measuring!=0){
        measured=((prerouting_end.tv_sec-prerouting_start.tv_sec)*1e9L) + (prerouting_end.tv_nsec-prerouting_start.tv_nsec);
        if (measured<0)
            printf("negative prerouting time read! (%ld) %d:%d - %d:%d\n", measured, prerouting_end.tv_sec,prerouting_end.tv_nsec, prerouting_start.tv_sec,prerouting_start.tv_nsec);
        else {
            total_prerouting_time+=measured;
            prerouting_count++;
            if (total_prerouting_time<0)
                printf("possible prerouting overflow (%lld)\n",total_prerouting_time);
        }
    }
#endif

	current_path_length=0;
	while(current!=dst) {	// not at destination yet
#ifdef MEASURE_ROUTING_TIME
        measuring=1;
        if(clock_gettime(CLOCK_MODE , &hoprouting_start)!=0){
            perror("Error Measuring routing time.");
            measuring=0;
        }
#endif
		next_port=route(current,dst);

#ifdef MEASURE_ROUTING_TIME
		if(clock_gettime(CLOCK_MODE , &hoprouting_end)!=0){
            perror("Error Measuring routing time.");
            measuring=0;
        }

        if (measuring!=0){
            measured=(hoprouting_end.tv_sec-hoprouting_start.tv_sec)*1e9L + (hoprouting_end.tv_nsec-hoprouting_start.tv_nsec);
            if (measured<0)
                printf("negative reading (%ld) %d:%d - %d:%d\n", measured, hoprouting_end.tv_sec,hoprouting_end.tv_nsec, hoprouting_start.tv_sec,hoprouting_start.tv_nsec);
            else{
                total_hoprouting_time+=measured;
                hoprouting_count++;
                if (total_hoprouting_time<0)
                    printf("possible overflow (%lld)\n",total_hoprouting_time);

            }
        }
#endif

		if (next_port==-1 ||
            network[current].port[next_port].faulty ||
            network[current].port[next_port].neighbour.node==-1) { // routing function aborted, i.e. no suitable path
			finish_route();
			return 0; // this pair of servers are disconnected;
		}

		if (current_path_length==max_path_length){ // this is not too pretty but shouldn't be called to often and I rather not use dynamic lists for this (for performance)
			max_path_length*=2;
			partial_path=realloc(partial_path, max_path_length*sizeof(long)); // we double the max path length
		}

		partial_path[current_path_length++]=next_port;

#ifdef DEBUG
		if(network[current].port[next_port].faulty==1){
			printf("mark_route should not use faulty ports.\n");
			exit(-1);
		}
#endif
		current=network[current].port[next_port].neighbour.node;
#ifdef DEBUG
		if (current==-1) { //disconnected
			printf("Should not have arrived here!!!!\ncurrent node is -1\n");
			exit(-1);
		}
#endif // DEBUG
	}

	current=src;
	for(i=0; i<current_path_length; i++){
		next_port=partial_path[i];
		network[current].port[next_port].flows++;	// update flows
		lh++;	// update link hops
		if (is_server(current)) // update server hops
			sh++;
		current=network[current].port[next_port].neighbour.node;
	}
	finish_route();

	update_statistics(sh, lh);

	return 1;
}

/**
* Performs a static run in which all of the flows are placed in the network and use statistics are captured.
* Dynamic evolution of the system is not considered.
*/
void run_static()
{
	long done=0;
	flow_t flow;

        init_stats();
	init_placement(servers);
	init_pattern(servers,traffic_nparam,traffic_params);

	partial_path=malloc(max_path_length*sizeof(long));
	while (!done) {
		flow=next_flow();
		if(flow.src==-1 || flow.dst==-1)
			done=1;
		else {
			traffic_npairs++;
			connected += mark_route(get_server_i(task_to_server(flow.src)), get_server_i(task_to_server(flow.dst)));
		}
	}
        free(partial_path);
	capture_statistics();

	create_latex_output();

        finish_traffic(traffic_nparam);

        finish_placement();
}


