#ifndef _topo
#define _topo

#include "node.h"


// Initializes the topology and sets the parameters.
// #params + array_of_params (long).
long init_topo(long nparam, long* params);

// Get the number of components of the network
// nodes in [0, #servers) should be the servers
// nodes in [#servers, #servers+#switches) should be the switches
long get_servers();
long get_switches();
long get_ports();

long is_server(long i);
long get_server_i(long i);
long get_switch_i(long i);

// Get the number of ports of a given node (either a server or a switch, see above)
long get_radix(long n);

// Calculates where to connect a port
// node_port <--> tuple.node_tuple.port
//
// Note that links are bidirectional, so when connecting a port the destination port
// will be connected to the original port as well.
// This may lead to undefined behaviour if the connections are not done properly
// (e.g if n0_p0 <-> n1_p1, and then n1_p1 <-> n2_p2: n0_p0 -> n1_p1 and n1_p1 <-> n2_p2, which is probably erroneous.
//
// disconnected nodes can be represented with tuple <-1, -1>
tuple_t connection(long node, long port);

// Initializes the routing for a given path. Can be used for source routing.
// In local-routing this might do nothing
// returns -1 if there is no route to the destination.
long init_routing(long src, long dst);

// Finishes a route, checks route is complete and frees stuff if needed.
void finish_route();

// Select the next port towards a destination
// Select the output port for a packet in node 'current' addressed to 'destination'
long route (long current, long destination);

#endif //_topo
