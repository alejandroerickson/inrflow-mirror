#ifndef _node
#define _node

#include "list.h"

// Define the maximum number of applications to execute and therefore, the maximum number of concurrent applications.  
#define MAX_CONCURRENT_APPS 1000

/**
 * Structure that defines a port/link location.
 */
typedef struct tuple_t {
    long node;	///< the node it is connected to (-1 means not connected).
    long port;	///< the port is connected to.
} tuple_t;

/**
 * Structure that defines a port/link.
 */
typedef struct port_t {
    long faulty;///< Is there any problem with the link: 0 is ok, anything else is considered a failure.
    long flows;	///< the number of flows that use this specific port.
    long link_info[MAX_CONCURRENT_APPS + 1]; 
    list_t dflows;
    long bandwidth_capacity; ///< Speed of the link in units/time
    tuple_t neighbour;	///< the node & port it is connected to (-1 means not connected).
} port_t;

/**
 * Structure that defines a node.
 * Simple enough to represent both servers and switches.
 */
typedef struct node_t {
    long nports;///< number of ports .
    port_t* port;	///< structure containing all the ports.
} node_t;

void set_bandwidth_capacity(port_t *port, long bandwidth_capacity);

long get_bandwidth_capacity(long node, long port);

#endif //_node
