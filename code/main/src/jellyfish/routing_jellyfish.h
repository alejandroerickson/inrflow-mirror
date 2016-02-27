#ifndef _routing_jellyfish
#define _routing_jellyfish

#include "rg_gen.h"

// Generate routing table
void generate_routing_table();

//Destroy the routing table
void destroy_routing_table();

//Print the routing table
void print_routing_table();

typedef struct k_paths {
	long last_used;
	long *next;
} k_paths;

void find_shortest_path(graph_t *rg, long switches, long node_src, k_paths **routing_table);

void mark_shortest_route(long start, long end, long *parents, k_paths **routing_table, long current);

void print_routing_table();

void destroy_routing_table();

void generate_routing_table();

#endif
