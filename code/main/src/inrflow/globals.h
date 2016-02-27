/**
* @file
* @brief	Declaration of all global variables & functions.

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

#include "node.h"
#include "path_list.h"

extern long traffic_nparam;
extern long traffic_params[];
extern long topo_nparam;
extern long topo_params[];
extern long routing_params[];
extern long routing_nparam;
extern long n_failures;
extern float failure_rate;
extern long r_seed;
extern long out_net_struc;
extern topo_t topo;
extern routing_t routing;
extern tpattern_t pattern;
extern char* traffic_name;

extern long servers; ///< The total number of servers
extern long switches;///< The total number of switches
extern long radix;	  ///< radix of the switches - assuming all switches have the same
extern long ports;	///< The total number of ports in the topology

extern long long connected;///< number of connected pairs (server-to-server)
extern long long link_hops;///< overall number of links traversed (slightly different from server_hops).
extern long long server_hops;///< overall number of server-to-server hops (slightly different from link_hops)
extern long bottleneck_flow;///<  maximum number of flows in a link. Useful to calculate aggregated bisection throughput

extern long upper_flows;

extern node_t* network;
extern routing_t routing;
extern int bfs_output;
extern int compile_latex;

/* In get_conf.c */
//extern literal_t vc_l[];
//extern literal_t routing_l[];
//extern literal_t rmode_l[];
//extern literal_t atype_l[];
//extern literal_t ctype_l[];
//extern literal_t pattern_l[];
//extern literal_t topology_l[];
//extern literal_t injmode_l[];
//extern literal_t placement_l[];

void get_conf(long, char **);

long (*init_topo)(long nparam, long* params);
void (*finish_topo)();

// Get the number of components of the network
long (*get_servers)();
long (*get_switches)();
long (*get_ports)();

// Functions to deal with server/switch differences
long (*is_server)(long i);
long (*get_server_i)(long i);
long (*get_switch_i)(long i);
long (*node_to_server)(long i);
long (*node_to_switch)(long i);

char* (*get_network_token)(); //"%s " //"network " //single-token name
char* (*get_routing_token)();  //"%s " //"routing_algorithm " //single-token name
char* (*get_topo_version)(); //"%s " //"topo_version " //version of the topology
char* (*get_topo_param_tokens)(long i);
char* (*get_filename_params)();
// Get the number of ports of a given node (either a server or a switch, see above)
long (*get_radix)(long n);
#endif
