#ifndef _node
#define _node

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

#endif //_node
