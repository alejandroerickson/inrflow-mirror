#ifndef _knkstar
#define _knkstar

#include "../inrflow/node.h"
#include "../inrflow/misc.h"
#include "../inrflow/globals.h"

// Initializes the topology and sets the parameters.
// #params + array_of_params (long).
long init_topo_knkstar(long nparam, long* params);

// Release the resources.
void finish_topo_knkstar();

// Get the number of components of the network
long get_servers_knkstar();
long get_switches_knkstar();
long get_ports_knkstar();

long is_server_knkstar(long i);
long get_server_i_knkstar(long i);
long get_switch_i_knkstar(long i);
long node_to_server_knkstar(long i);
long node_to_switch_knkstar(long i);


char * get_network_token_knkstar(); //"%s " //"network " //single-token name
char * get_routing_token_knkstar();  //"%s " //"routing_algorithm " //single-token name
char * get_topo_version_knkstar(); //"%s " //"topo_version " //version of the topology
char * get_topo_param_tokens_knkstar(long i);
char* get_filename_params_knkstar();

// Get the number of ports of a given node (either a server or a switch, see above)
long get_radix_knkstar(long n);

// Calculates where to connect a port
// node_port <--> tuple.node_tuple.port
//
// Note that links are bidirectional, so when connecting a port the destination port
// will be connected to the original port as well.
// This may lead to undefined behaviour if the connections are not done properly
// (e.g if n0_p0 <-> n1_p1, and then n1_p1 <-> n2_p2: n0_p0 -> n1_p1 and n1_p1 <-> n2_p2, which is probably erroneous.
//
// disconnected nodes can be represented with tuple <-1, -1>
tuple_t connection_knkstar(long node, long port);

// Initializes the routing for a given path. Can be used for source routing.
// In local-routing this might do nothing
// returns -1 if there is no route to the destination.
long init_routing_knkstar(long src, long dst);

// Finishes a route, checks route is complete and frees stuff if needed.
void finish_route_knkstar();

// Select the next port towards a destination
// Select the output port for a packet in node 'current' addressed to 'destination'
long route_knkstar(long current, long destination);

#endif //_topo
