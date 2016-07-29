/** @mainpage
  Copyright (2014)

  @author J. Navaridas.

  @section gnu License

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
  */

#ifndef _main
#define _main

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "../knkstar/knkstar.h"
#include "../ficonn/ficonn.h"
#include "../gdcficonn/gdcficonn.h"
#include "../swcube/swcube.h"
#include "../bcube/bcube.h"
#include "../hcnbcn/hcnbcn.h"
#include "../dpillar/dpillar.h"
#include "../fattree/fattree.h"
#include "../thintree/thintree.h"
#include "../jellyfish/jellyfish.h"
#include "../torus/torus.h"

//AE: if we include the above, which also include node.h, are we not
//including node.h multiple times?
#include "node.h"
#include "misc.h"
#include "dynamic_engine.h"
#include "traffic.h"
#include "placement.h"
#include "globals.h"
#include "reporting.h"
#include "applications.h"
#include "scheduling.h"
#include "list.h"

//#define SHOWCONNECTIONS
//#define DEBUG

/* Global variables - parameters */
long r_seed;    ///< Random seed

// Could use directly the defined values but will be useful in the future when accepting them as a parameter.
topo_t topo;    ///< the topology we are modeling
long topo_nparam;   ///< Number of parameters passed to the topology
long topo_params[MAX_TOPO_PARAMS];  ///< parameters passed to the topology

list_t workload;
list_t running_applications;
applications_t applications_type;
char applications_file[100];

tpattern_t pattern; ///< the traffic pattern we are using in the simulation
char* traffic_name; ///< the name of the used traffic pattern as a string
long traffic_nparam;    ///< Number of parameters passed to the traffic generator
long traffic_params[MAX_TRAFFIC_PARAMS];    ///< Parameters passed to the traffic generator

scheduling_t scheduling; ///< the scheduling policy we are using in the simulation
char* scheduling_name; ///< the name of the used scheduling policy as a string
long scheduling_nparam;    ///< Number of parameters passed to the scheduling policy
long scheduling_params[MAX_SCHEDULING_PARAMS];    ///< Parameters passed to the scheduling policy

allocation_t allocation; ///< the allocation strategy we are using in the simulation
char* allocation_name; ///< the name of the used allocation strategy as a string
long allocation_nparam;    ///< Number of parameters passed to the allocation strategy
long allocation_params[MAX_SCHEDULING_PARAMS];    ///< Parameters passed to the allocation strategy

routing_t routing;  ///< The routing function we are using
long routing_nparam;    ///< Number of parameters passed to the routing
long routing_params[MAX_ROUTING_PARAMS];    ///< Parameters passed to the routing

placement_t placement;
long placement_nparam;    ///< Number of parameters passed to the placement
long placement_params[MAX_PLACEMENT_PARAMS];    ///< Parameters passed to the placement
char placement_file[100];

float failure_rate;  ///< rate of the network which is faulty updates n_failures
long n_failures;  ///< number of failures

long servers; ///< The total number of servers
long switches;///< The total number of switches
long radix;	  ///< radix of the switches - assuming all switches have the same
long ports;	///< The total number of ports (i.e.links) in the topology

long long connected;///< number of connected pairs (server-to-server)
long long traffic_npairs;

node_t* network;    /// Data structure containing the model of the network.

scheduling_info *sched_info;

int bfs_output; ///< Generate a bfs file with the topology? 0:no, other:yes
// These functions below are the virtual functions to
// support multiple topologies

int compile_latex;///< Compile latex (to pdf) 0:no, other:yes
int mode; ///< Runtime mode: static or dynamic.
int flow_inj_mode; ///< Flow injection mode
int dmetrics_time;
int verbose;

//Servers definition
long server_cores;
long server_memory;
long server_capacity;
long switch_capacity;

/** Initializes the virtual functions to point to the ones needed, based on the input parameters.
 */
void init_functions()
{
	switch(topo) {
	case FICONN:
		init_topo=init_topo_ficonn;
		finish_topo=finish_topo_ficonn;
		get_servers=get_servers_ficonn;
		get_switches=get_switches_ficonn;
		get_ports=get_ports_ficonn;
		is_server=is_server_ficonn;
		get_server_i=get_server_i_ficonn;
		get_switch_i=get_switch_i_ficonn;
		node_to_server=node_to_server_ficonn;
		node_to_switch=node_to_switch_ficonn;
		get_radix=get_radix_ficonn;
		connection=connection_ficonn;

		init_routing=init_routing_ficonn;
		finish_route=finish_route_ficonn;
		route=route_ficonn;

		get_network_token=get_network_token_ficonn;
		get_routing_token=get_routing_token_ficonn;
		get_topo_version=get_topo_version_ficonn;
		get_topo_param_tokens=get_topo_param_tokens_ficonn;
		get_filename_params=get_filename_params_ficonn;
		break;
	case HCNBCN:
		init_topo=init_topo_hcnbcn;
		finish_topo=finish_topo_hcnbcn;
		get_servers=get_servers_hcnbcn;
		get_switches=get_switches_hcnbcn;
		get_ports=get_ports_hcnbcn;
		is_server=is_server_hcnbcn;
		get_server_i=get_server_i_hcnbcn;
		get_switch_i=get_switch_i_hcnbcn;
		node_to_server=node_to_server_hcnbcn;
		node_to_switch=node_to_switch_hcnbcn;
		get_radix=get_radix_hcnbcn;
		connection=connection_hcnbcn;

		init_routing=init_routing_hcnbcn;
		finish_route=finish_route_hcnbcn;
		route=route_hcnbcn;

		get_network_token=get_network_token_hcnbcn;
		get_routing_token=get_routing_token_hcnbcn;
		get_topo_version=get_topo_version_hcnbcn;
		get_topo_param_tokens=get_topo_param_tokens_hcnbcn;
		get_filename_params=get_filename_params_hcnbcn;
		get_routing_param_tokens=get_routing_param_tokens_hcnbcn;

		get_topo_nstats=get_topo_nstats_hcnbcn;
		get_topo_key_value=get_topo_key_value_hcnbcn;
		get_topo_nhists=get_topo_nhists_hcnbcn;
		get_topo_hist_prefix=get_topo_hist_prefix_hcnbcn;
		get_topo_hist_doc=get_topo_hist_doc_hcnbcn;
		get_topo_hist_max=get_topo_hist_max_hcnbcn;
		get_topo_hist=get_topo_hist_hcnbcn;
		break;
	case GDCFICONN:
		init_topo=init_topo_gdcficonn;
		finish_topo=finish_topo_gdcficonn;
		get_servers=get_servers_gdcficonn;
		get_switches=get_switches_gdcficonn;
		get_ports=get_ports_gdcficonn;
		is_server=is_server_gdcficonn;
		get_server_i=get_server_i_gdcficonn;
		get_switch_i=get_switch_i_gdcficonn;
		node_to_server=node_to_server_gdcficonn;
		node_to_switch=node_to_switch_gdcficonn;
		get_radix=get_radix_gdcficonn;
		connection=connection_gdcficonn;

		init_routing=init_routing_gdcficonn;
		finish_route=finish_route_gdcficonn;
		route=route_gdcficonn;

		get_network_token=get_network_token_gdcficonn;
		get_routing_token=get_routing_token_gdcficonn;
		get_topo_version=get_topo_version_gdcficonn;
		get_topo_param_tokens=get_topo_param_tokens_gdcficonn;
		get_filename_params=get_filename_params_gdcficonn;
		get_routing_param_tokens=get_routing_param_tokens_gdcficonn;
		get_topo_nstats=get_topo_nstats_gdcficonn;
		get_topo_key_value=get_topo_key_value_gdcficonn;
		get_topo_nhists=get_topo_nhists_gdcficonn;
		get_topo_hist_prefix=get_topo_hist_prefix_gdcficonn;
		get_topo_hist_doc=get_topo_hist_doc_gdcficonn;
		get_topo_hist_max=get_topo_hist_max_gdcficonn;
		get_topo_hist=get_topo_hist_gdcficonn;

		break;
	case KNKSTAR:
		init_topo=init_topo_knkstar;
		finish_topo=finish_topo_knkstar;
		get_servers=get_servers_knkstar;
		get_switches=get_switches_knkstar;
		get_ports=get_ports_knkstar;
		is_server=is_server_knkstar;
		get_server_i=get_server_i_knkstar;
		get_switch_i=get_switch_i_knkstar;
		node_to_server=node_to_server_knkstar;
		node_to_switch=node_to_switch_knkstar;
		get_radix=get_radix_knkstar;
		connection=connection_knkstar;
		init_routing=init_routing_knkstar;
		finish_route=finish_route_knkstar;
		route=route_knkstar;
		get_network_token=get_network_token_knkstar;
		get_routing_token=get_routing_token_knkstar;
		get_topo_version=get_topo_version_knkstar;
		get_topo_param_tokens=get_topo_param_tokens_knkstar;
		get_filename_params=get_filename_params_knkstar;
		break;
	case DPILLAR:
		init_topo=init_topo_dpillar;
		finish_topo=finish_topo_dpillar;
		get_servers=get_servers_dpillar;
		get_switches=get_switches_dpillar;
		get_ports=get_ports_dpillar;
		is_server=is_server_dpillar;
		get_server_i=get_server_i_dpillar;
		get_switch_i=get_switch_i_dpillar;
		node_to_server=node_to_server_dpillar;
		node_to_switch=node_to_switch_dpillar;
		get_radix=get_radix_dpillar;
		connection=connection_dpillar;
		init_routing=init_routing_dpillar;
		finish_route=finish_route_dpillar;
		route=route_dpillar;
		get_network_token=get_network_token_dpillar;
		get_routing_token=get_routing_token_dpillar;
		get_topo_version=get_topo_version_dpillar;
		get_topo_param_tokens=get_topo_param_tokens_dpillar;
		get_filename_params=get_filename_params_dpillar;
		break;
	case SWCUBE:
		init_topo=init_topo_swcube;
		finish_topo=finish_topo_swcube;
		get_servers=get_servers_swcube;
		get_switches=get_switches_swcube;
		get_ports=get_ports_swcube;
		is_server=is_server_swcube;
		get_server_i=get_server_i_swcube;
		get_switch_i=get_switch_i_swcube;
		node_to_server=node_to_server_swcube;
		node_to_switch=node_to_switch_swcube;
		get_radix=get_radix_swcube;
		connection=connection_swcube;
		init_routing=init_routing_swcube;
		finish_route=finish_route_swcube;
		route=route_swcube;
		get_network_token=get_network_token_swcube;
		get_routing_token=get_routing_token_swcube;
		get_topo_version=get_topo_version_swcube;
		get_topo_param_tokens=get_topo_param_tokens_swcube;
		get_filename_params=get_filename_params_swcube;
		break;
	case BCUBE:
		init_topo=init_topo_bcube;
		finish_topo=finish_topo_bcube;
		get_servers=get_servers_bcube;
		get_switches=get_switches_bcube;
		get_ports=get_ports_bcube;
		is_server=is_server_bcube;
		get_server_i=get_server_i_bcube;
		get_switch_i=get_switch_i_bcube;
		node_to_server=node_to_server_bcube;
		node_to_switch=node_to_switch_bcube;
		get_radix=get_radix_bcube;
		connection=connection_bcube;
		init_routing=init_routing_bcube;
		finish_route=finish_route_bcube;
		route=route_bcube;
		get_network_token=get_network_token_bcube;
		get_routing_token=get_routing_token_bcube;
		get_topo_version=get_topo_version_bcube;
		get_topo_param_tokens=get_topo_param_tokens_bcube;
		get_filename_params=get_filename_params_bcube;
		break;
	case FATTREE:
		init_topo=init_topo_fattree;
		finish_topo=finish_topo_fattree;
		get_servers=get_servers_fattree;
		get_switches=get_switches_fattree;
		get_ports=get_ports_fattree;
		is_server=is_server_fattree;
		get_server_i=get_server_i_fattree;
		get_switch_i=get_switch_i_fattree;
		node_to_server=node_to_server_fattree;
		node_to_switch=node_to_switch_fattree;
		get_radix=get_radix_fattree;
		connection=connection_fattree;
		init_routing=init_routing_fattree;
		finish_route=finish_route_fattree;
		route=route_fattree;
		get_network_token=get_network_token_fattree;
		get_routing_token=get_routing_token_fattree;
		get_topo_version=get_topo_version_fattree;
		get_topo_param_tokens=get_topo_param_tokens_fattree;
		get_filename_params=get_filename_params_fattree;
		break;
case TORUS:
		init_topo=init_topo_torus;
		finish_topo=finish_topo_torus;
		get_servers=get_servers_torus;
		get_switches=get_switches_torus;
		get_ports=get_ports_torus;
		is_server=is_server_torus;
		get_server_i=get_server_i_torus;
		get_switch_i=get_switch_i_torus;
		node_to_server=node_to_server_torus;
		node_to_switch=node_to_switch_torus;
		get_radix=get_radix_torus;
		connection=connection_torus;
		init_routing=init_routing_torus;
		finish_route=finish_route_torus;
		route=route_torus;
		get_network_token=get_network_token_torus;
		get_routing_token=get_routing_token_torus;
		get_topo_version=get_topo_version_torus;
		get_topo_param_tokens=get_topo_param_tokens_torus;
		get_filename_params=get_filename_params_torus;
		break;

	case THINTREE:
		init_topo=init_topo_thintree;
		finish_topo=finish_topo_thintree;
		get_servers=get_servers_thintree;
		get_switches=get_switches_thintree;
		get_ports=get_ports_thintree;
		is_server=is_server_thintree;
		get_server_i=get_server_i_thintree;
		get_switch_i=get_switch_i_thintree;
		node_to_server=node_to_server_thintree;
		node_to_switch=node_to_switch_thintree;
		get_radix=get_radix_thintree;
		connection=connection_thintree;
		init_routing=init_routing_thintree;
		finish_route=finish_route_thintree;
		route=route_thintree;
		get_network_token=get_network_token_thintree;
		get_routing_token=get_routing_token_thintree;
		get_topo_version=get_topo_version_thintree;
		get_topo_param_tokens=get_topo_param_tokens_thintree;
		get_filename_params=get_filename_params_thintree;
		break;
	case JELLYFISH:
		init_topo=init_topo_jellyfish;
		finish_topo=finish_topo_jellyfish;
		get_servers=get_servers_jellyfish;
		get_switches=get_switches_jellyfish;
		get_ports=get_ports_jellyfish;
		is_server=is_server_jellyfish;
		get_server_i=get_server_i_jellyfish;
		get_switch_i=get_switch_i_jellyfish;
		node_to_server=node_to_server_jellyfish;
		node_to_switch=node_to_switch_jellyfish;
		get_radix=get_radix_jellyfish;
		connection=connection_jellyfish;
		init_routing=init_routing_jellyfish;
		finish_route=finish_route_jellyfish;
		route=route_jellyfish;
		get_network_token=get_network_token_jellyfish;
		get_routing_token=get_routing_token_jellyfish;
		get_topo_version=get_topo_version_jellyfish;
		get_topo_param_tokens=get_topo_param_tokens_jellyfish;
		get_routing_param_tokens=get_routing_param_tokens_jellyfish;
		get_filename_params=get_filename_params_jellyfish;
		break;
	default:
		break;
	}

	switch (pattern) {
	case ALL2ALL:
		init_pattern=init_pattern_all2all;
		next_flow=next_flow_all2all;
		break;
	case MANYALL2ALL:
		init_pattern=init_pattern_many_all2all;
		next_flow=next_flow_many_all2all;
		break;
	case MANYALL2ALLRND:
		init_pattern=init_pattern_many_all2all_rnd;
		next_flow=next_flow_many_all2all_rnd;
		break;
	case BUTTERFLY:
		init_pattern=init_pattern_butterfly;
		next_flow=next_flow_butterfly;
		break;
	case ALL2ONE:
		init_pattern=init_pattern_all2one;
		next_flow=next_flow_all2one;
		break;
	case ONE2ALL:
		init_pattern=init_pattern_one2all;
		next_flow=next_flow_one2all;
		break;
	case RANDOM:
		init_pattern=init_pattern_rnd;
		next_flow=next_flow_rnd;
		break;
	case SHIFT:
        init_pattern=init_pattern_shift;
		next_flow=next_flow_shift;
		break;
	case BISECTION:
	    init_pattern=init_pattern_bisection;
		next_flow=next_flow_bisection;
		break;
	case HOTREGION:
		init_pattern=init_pattern_hotregion;
		next_flow=next_flow_hotregion;
		break;
        default:
            printf("Error: Unknown traffic pattern.\n");
            exit(-1);
    }
}

/**
 * Check whether there are any nodes that are expected to be neighbours
 * but aren't - possibly due to errors in the connection function for
* the topology.
*/
void check_consistency()
{
    long halt=0;
    long nnode;
    long nport;
    long i,j;

    for(i=0; i<servers+switches; i++) {
        for(j=0; j<network[i].nports; j++) {
            if(network[i].port[j].neighbour.node!=-1) {
                nnode=network[i].port[j].neighbour.node;
                nport=network[i].port[j].neighbour.port;

                if (network[nnode].port[nport].neighbour.node !=i || network[nnode].port[nport].neighbour.port !=j ) {
                    printf("Inconsistency!, %ld.%ld <-> %ld.%ld but %ld.%ld <-> %ld.%ld\n",
                            i, j,
                            network[i].port[j].neighbour.node, network[i].port[j].neighbour.port,
                            network[i].port[j].neighbour.node, network[i].port[j].neighbour.port,
                            network[nnode].port[nport].neighbour.node,
                            network[nnode].port[nport].neighbour.port);
                    halt=1;
                }
            } else if (network[i].port[j].neighbour.port!=-1) {
                printf("Warning!, %ld.%ld neighbour node is -1, but ports is not -1\n",i, j);
            }
        }
    }
    if (halt)
        exit(-1);
}

/**
 * Initializes data structures and connects them to form the desired topology.
 */
void construct_topology()
{
	long i,j,k; // node, port
	tuple_t dst;

	servers=get_servers();
	switches=get_switches();
	ports=get_ports();
	radix=get_radix(get_switch_i(0));
	// printf("servers %ld, switches %ld\n",servers, switches);

	network=malloc(sizeof(node_t)*(servers+switches));

	for(i=0; i<servers+switches; i++) {
		network[i].nports=get_radix(i);
		network[i].port=malloc(sizeof(port_t)*network[i].nports);
		for(j=0; j<network[i].nports; j++) {
			network[i].port[j].neighbour.node=-1;
			network[i].port[j].neighbour.port=-1;
			network[i].port[j].bandwidth_capacity=0;
                        for(k=0; k<MAX_CONCURRENT_APPS;k++){
                            network[i].port[j].link_info[k] = 0;
                        }

		}
	}

	for(i=0; i<servers+switches; i++) {
		for(j=0; j<network[i].nports; j++) {
			dst=connection(i,j);
			// Assess consistency of connections
			if (dst.node==-1 || dst.port==-1) { //this port is disconnected
				dst.node=-1;
				dst.port=-1;
				network[i].port[j].neighbour=dst;
			} else {
				network[i].port[j].neighbour=dst;
				network[dst.node].port[dst.port].neighbour.node=i;
				network[dst.node].port[dst.port].neighbour.port=j;
                                list_initialize(&network[i].port[j].dflows, sizeof(dflow_t*));
                                if(is_server(i)){
                                    network[i].port[j].bandwidth_capacity=server_capacity;
                                }
                                else{
                                    network[i].port[j].bandwidth_capacity=switch_capacity;
                                }
			}
		}
	}
#ifdef DEBUG
    check_consistency();
#endif

#ifdef SHOWCONNECTIONS
    for(i=0; i<servers+switches; i++)
        for(j=0; j<network[i].nports; j++)
            printf("%ld.%ld <-> %ld.%ld\n", i, j, network[i].port[j].neighbour.node, network[i].port[j].neighbour.port);
#endif

}

void release_topology(){

    long i;

    for(i=0; i<servers+switches; i++) {
        free(network[i].port);
    }
    free(network);
}

/**
 * Resets the statistics and failure, useful to run monte carlo experiments.
 */
void reset()
{
    long i,j;// node, port

    for(i=0; i<servers+switches; i++) {
        for(j=0; j<network[i].nports; j++) {
            network[i].port[j].faulty=0;
            network[i].port[j].flows=0;
        }
    }

}

/**
 * Selects at random which links to break
 */
void set_failures(long failures)
{
    long n;
    long i,j; // node, port

    for(n=0; n<failures; n++) {
        do {
            i=rand()%(servers+switches);
            j=rand()%network[i].nports;
            // Choose a different link if the link is already faulty or it is disconnected
        } while (network[i].port[j].faulty==1 ||
                network[i].port[j].neighbour.node==-1);
        network[i].port[j].faulty=1;
        network[network[i].port[j].neighbour.node]
            .port[network[i].port[j].neighbour.port].faulty=1;
#ifdef SHOWCONNECTIONS
        printf("node %ld fails on port %ld\n",i,j);
#endif
    }
}

/**
 * Counts the number of failures in the network.
 */
long check_failures()
{
    long count,i,j;
    count = 0;
    for(i=0; i<servers+switches; i++)
        for(j=0; j<network[i].nports; j++)
            count+=network[i].port[j].faulty;
    return count;
}

/**
 * Outputs the topology in an adjacency list format (for BFS).
 * @return The finalization code. -1 if there was any error, otherwise 0
 */
long generate_bfs_file()
{
    long i, j, k;
    long s; ///< server
    long nnode;

    FILE *bfs_file;
    char bfs_filename[100];

    sprintf(bfs_filename,"%s_%s_%s_fr%.2f_seed%ld.bfs",
            get_network_token(),
            get_filename_params(),
            get_routing_token(),
            failure_rate,
            r_seed);

    if((bfs_file = fopen(bfs_filename,"w")) == NULL) {
        perror("Unable to open bfs file for writing");
        return -1;
    }

    if (topo<SERVERCENTRIC) {// For servercentric networks we consider server-to-server adjacency only.
        fprintf(bfs_file,"%ld\n",servers);
        for (i=0; i<servers; i++) {
            s = get_server_i(i);
            fprintf(bfs_file,"%ld",i);
            for(j=0; j<network[s].nports; j++) {
                if (network[s].port[j].neighbour.node!=-1 && network[s].port[j].faulty!=1) { // the server is connected and the link is not faulty
                    nnode=network[s].port[j].neighbour.node;
                    if (is_server(nnode)) { // if the neighbour is a server, add it directly to the adjacency list.
                        fprintf(bfs_file,", %ld",node_to_server(nnode));
                    } else { // this is a switch; check the neighbours at every port
                        for (k=0; k<network[nnode].nports; k++) {
                            if (network[nnode].port[k].neighbour.node!=-1 && network[nnode].port[k].faulty!=1) {
                                if (network[nnode].port[k].neighbour.node!=s ) {// not the original node
                                    if(is_server(network[nnode].port[k].neighbour.node))
                                        fprintf(bfs_file,", %ld",node_to_server(network[nnode].port[k].neighbour.node));
                                    else
                                        printf("Switches (%ld and %ld) interconnected in a server-centric topology. Check topology definition is correct.\n",nnode,network[nnode].port[k].neighbour.node);
                                }
                            }
                        }
                    }
                }
            }
            fprintf(bfs_file,"\n");
        }
    } else {
        fprintf(bfs_file,"%ld\n",servers+switches);
        for (i=0; i<servers+switches; i++) {
            fprintf(bfs_file,"%ld",i);
            for(j=0; j<network[i].nports; j++) {
                if (network[i].port[j].neighbour.node!=-1 && network[i].port[j].faulty!=1) { // the server is connected and the link is not faulty
                    fprintf(bfs_file,", %ld",network[i].port[j].neighbour.node);
                }
            }
            fprintf(bfs_file,"\n");
        }
    }
    fclose(bfs_file);
    printf("wrote topology model for bfs in file %s\n",bfs_filename);
    exit(0);
    return 0; //Everything went ok
}


/**
 * return true if and only if either the number of failures is non-zero or the failure rate is
 * non-zero.  This works before calling init_topo().  We assume get_conf() has been called.
 * declared in globals.h
 */
bool_t are_there_failures(){
	if(n_failures>0) return TRUE;
	if(n_failures==0) return FALSE;
	if(failure_rate>0.0) return TRUE;
	return FALSE;
}


/**
 * Main function.
 *
 * Initializes network (and failures, if any).
 * Then routes among every pair of servers and captures statistics.
 * Finally writes the results to file and stdout.
 *
 * @param argc The number of parameters given in the command line.
 * @param argv Array that contains all the parameters.
 * @return The finalization code. Usually 0.
 */
int main(int argc, char **argv)
{

    //Record start time for pretty output and for actual computation

#ifdef DEBUG
    printf("DEBUG flag is on.\n");
#endif

    get_conf((long)(argc - 1), argv + 1);
    init_functions();
    srand(r_seed);
    init_topo(topo_nparam, topo_params);
    construct_topology();
    reset();

    //use probability to decide number of failures.  cast it to long
    if (n_failures==-1) // undefined
        n_failures = (long)((ports/2)*failure_rate);//ports is the number of ports (twice the number of links)

    set_failures(n_failures);

#ifdef DEBUG
    long c_failures;
    if((c_failures=check_failures()/2)!=n_failures)
        printf("n_failures = %ld but check_failures = %ld",n_failures,c_failures);
#endif //DEBUG

	if (bfs_output!=0)
		generate_bfs_file();

        switch(mode){
            case STATIC:
	        run_static();
                break;
            case DYNAMIC:
	        run_dynamic();
                break;
            default:
                printf("Execution mode not defined.\n");
                exit(-1);
        }

    finish_topo();

	release_topology();
#ifdef DEBUG
    printf("DEBUG flag is on.\n");
#endif

    return 0;
}

#endif //_main
