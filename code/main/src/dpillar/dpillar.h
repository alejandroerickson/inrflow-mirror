#ifndef _dpillar
#define _dpillar

#include "../inrflow/node.h"
#include "../inrflow/misc.h"

// Initializes the topology and sets the parameters.
// #params + array_of_params (long).
long init_topo_dpillar(long nparam, long* params);

// Release the resources.
void finish_topo_dpillar();

// Get the number of components of the network
// nodes in [0, #servers) should be the servers
// nodes in [#servers, #servers+#switches) should be the switches
long get_servers_dpillar();
long get_switches_dpillar();
long get_ports_dpillar();

long is_server_dpillar(long i);
long get_server_i_dpillar(long i);
long get_switch_i_dpillar(long i);
long node_to_server_dpillar(long i);
long node_to_switch_dpillar(long i);

char* get_network_token_dpillar();
char* get_routing_token_dpillar();
char* get_topo_version_dpillar();
char* get_topo_param_tokens_dpillar(long i);
char* get_filename_params_dpillar();

long det_dir_dpillar(long source, long dest, long rout_alg);

// Get the number of ports of a given node (either a server or a switch, see above)
long get_radix_dpillar(long n);

// Calculates where to connect a port
// node_port <--> tuple.node_tuple.port
//
// Note that links are bidirectional, so when connecting a port the destination port
// will be connected to the original port as well.
// This may lead to undefined behaviour if the connections are not done properly
// (e.g if n0_p0 <-> n1_p1, and then n1_p1 <-> n2_p2: n0_p0 -> n1_p1 and n1_p1 <-> n2_p2, which is probably erroneous.
//
// disconnected nodes can be represented with tuple <-1, -1>
tuple_t connection_dpillar(long node, long port);

// Initializes the routing for a given path. Can be used for source routing.
// In local-routing this might do nothing
// returns -1 if there is no route to the destination.
long init_routing_dpillar(long src, long dst);

// Finishes a route, checks route is complete and frees stuff if needed.
void finish_route_dpillar();

// Select the next port towards a destination
// Select the output port for a packet in node 'current' addressed to 'destination'
long route_dpillar(long current, long destination);

long get_radix_dpillar(long n);
void create_nodes(void);
tuple_t connection_dpillar(long node, long port);

// Initializes the routing for a given path. Can be used for source routing.
// In local-routing this might do nothing
// returns -1 if there is no route to the destination.
//long init_routing_dpillar(long src, long dst);

// Finishes a route, checks route is complete and frees stuff if needed.
//void finish_route_dpillar();

// Select the next port towards a destination
// Select the output port for a packet in node 'current' addressed to 'destination'
//long route_dpillar(long current, long destination);



#endif //_topo
