/**
* @file
* @brief	Declaration of all global variables & functions.

@author

Adapted from:
FSIN Functional Simulator of Interconnection Networks
Copyright (2003-2011) J. Miguel-Alonso, J. Navaridas

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or (at
your option) any later version.  This program is distributed in the
hope that it will be useful, but WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the GNU General Public License for more details.  You
should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#ifndef _globals
#define _globals

#include "misc.h"
//#include <math.h>
//#include <time.h>
//#include <limits.h>


#include "topo.h" ///< See this file for topology template and documentation.
#include "literal.h"
#include "traffic.h"
#include "node.h"
#include "list.h"
#include "scheduling.h"
#include "path_list.h"
#include "metrics.h"

extern long traffic_nparam;
extern long traffic_params[];
extern char **traffic_param_tokens;
extern long topo_nparam;
extern long topo_params[];
extern long routing_params[];
extern long routing_nparam;
extern long placement_params[];
extern long placement_nparam;
extern char *placement_name;
extern char placement_file[100];
extern long scheduling_nparam;
extern long scheduling_params[];
extern char* scheduling_name;
extern long allocation_nparam;
extern long allocation_params[];
extern char* allocation_name;
extern long n_failures;
extern float failure_rate;
extern long r_seed;
extern long out_net_struc;
extern topo_t topo;
extern list_t workload;
extern list_t running_applications;
extern applications_t applications_type;
extern char applications_file[100];
extern routing_t routing;
extern tpattern_t pattern;
extern placement_t placement;
extern scheduling_t scheduling;
extern allocation_t allocation;
extern char* traffic_name;
extern metrics_t metrics;

extern long servers; ///< The total number of servers
extern long switches;///< The total number of switches
extern long radix;	  ///< radix of the switches - assuming all switches have the same
extern long ports;	///< The total number of ports in the topology

extern long long connected;///< number of connected pairs (server-to-server)
extern long long traffic_npairs;
extern long long link_hops;///< overall number of links traversed (slightly different from server_hops).
extern long long server_hops;///< overall number of server-to-server hops (slightly different from link_hops)
extern long bottleneck_flow;///<  maximum number of flows in a link. Useful to calculate aggregated bisection throughput

extern long server_capacity;
extern long switch_capacity;

extern long upper_flows;

extern node_t* network;
extern scheduling_info *sched_info;
extern routing_t routing;
extern int bfs_output;
extern int compile_latex;
extern int mode;
extern int flow_inj_mode;
extern int dmetrics_time;
extern int verbose;

extern long server_cores;
extern long server_memory;


/* In get_conf.c */
//extern literal_t vc_l[];
//extern literal_t routing_l[];
//extern literal_t rmode_l[];
//extern literal_t atype_l[];
//extern literal_t ctype_l[];
extern literal_t tpatterns_l[];
extern literal_t placement_l[];
extern literal_t allocation_l[];
//extern literal_t injmode_l[];
//extern literal_t placement_l[];


#ifdef MEASURE_ROUTING_TIME
extern long long total_prerouting_time;  ///< stores the aggregated prerouting time.
extern long prerouting_count;              ///< stores how many times we have prerouted.

extern long long total_hoprouting_time;  ///< stores the aggregated hoprouting time.
extern long hoprouting_count;              ///< stores how many times we have hoprouted.
#endif

bool_t are_there_failures(); ///< Return TRUE iff there are network failures.

void get_conf(long, char **);

/**
* Initializes a traffic pattern by giving the number of servers and a list of parameters for that pattern.
*/
void (*init_pattern)(long servers, long nparam, long *params);

/**
* Gets the next flow of the current traffic pattern
*/
flow_t (*next_flow)();

void print_coordinates(long node, long port);

void run_static();
void run_dynamic();

#endif
