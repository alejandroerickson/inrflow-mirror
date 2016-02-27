#include <stdio.h>
#include <stdlib.h>
#include "rg_gen.h"
#include "routing_jellyfish.h"
#include "../inrflow/globals.h"

extern graph_t *random_graph;
extern k_paths ** routing_table;

/**
 * Auxiliar funtion to generate the routing table.
 */
void mark_shortest_route(long start, long end, long *parents, k_paths **routing_table, long current)
{

	if ((start == current) || (current == -1)) {
	} else {
		mark_shortest_route(start,end, parents, routing_table, parents[current]);
		routing_table[parents[current]][end].next[0] = current;
	}
}

/**
 * Find the shortest path between all pairs of nodes.
 */
void find_shortest_path(graph_t *rg, long switches, long node_src, k_paths **routing_table)
{

	long i;
	long *queue;
	long *discovered;
	long *parent;
	long *processed;
	long queue_insert, queue_extract;
	long v;

	queue = malloc(switches * sizeof(long));
	discovered = malloc(switches * sizeof(long));
	parent = malloc(switches * sizeof(long));
	processed = malloc(switches * sizeof(long));

	for(i = 0; i < switches; i++) {
		queue[i] = -1;
		parent[i] = -1;
		discovered[i] = 0;
		processed[i] = 0;
	}
	queue_insert = 0;
	queue_extract = 0;

	queue[queue_insert] = node_src;
	queue_insert++;
	discovered[node_src] = 1;

	while(queue_insert != queue_extract) {
		v = queue[queue_extract];
		queue_extract++;
		processed[v] = 1;
		for(i = 0; i < rg[v].nedges; i++) {
			if(rg[v].edge[i].neighbour.edge != -1) {
				if(discovered[rg[v].edge[i].neighbour.node] == 0) {
					queue[queue_insert] = rg[v].edge[i].neighbour.node;
					queue_insert++;
					discovered[rg[v].edge[i].neighbour.node] = 1;
					parent[rg[v].edge[i].neighbour.node] = v;
				}
			}
		}

	}

	for (i = 0; i< switches; i++)
		mark_shortest_route(node_src, i, parent, routing_table, i);

	free(queue);
	free(discovered);
	free(parent);
	free(processed);
}

/**
 * Generates the routing table.
 */
void generate_routing_table()
{

	long i, j, k;

	switch(routing) {
	case JELLYFISH_SHORTEST_PATH_ROUTING:
		routing_table = malloc(switches * sizeof(k_paths*));
		for(i = 0; i < switches; i++) {
			routing_table[i] = malloc(switches * sizeof(k_paths));
			for(j = 0; j < switches; j++) {
				routing_table[i][j].next = malloc(1 * sizeof(long));
				routing_table[i][j].last_used = 0;
				for(k = 0; k < 1; k++) {
					routing_table[i][j].next[k] = -1;
				}
			}
		}
		for(i = 0; i < switches; i++) {
			find_shortest_path(random_graph, switches, i, routing_table);
		}
		break;
	default:
		printf("Unknown routing.\n");
		exit(-1);
	}
}

/**
 * Releases the resources reserved to store the routing table.
 */
void destroy_routing_table()
{
	long i, j;

	switch(routing) {
	case JELLYFISH_SHORTEST_PATH_ROUTING:
		for(i = 0; i < switches; i++) {
			for(j = 0; j < switches; j++) {
				free(routing_table[i][j].next);
			}
			free(routing_table[i]);
		}
		free(routing_table);
		break;
	default:
		printf("Unknown routing.\n");
		exit(-1);
	}
}

/**
 * Print the routing table.
 */
void print_routing_table()
{
	long i, j, k;

	switch(routing) {
	case JELLYFISH_SHORTEST_PATH_ROUTING:
		for(i = 0; i < switches; i++) {
			for(j = 0; j < switches; j++) {
				for(k = 0; k < 1; k++) {
					printf("%ld ",routing_table[i][j].next[k]);
				}
			}
			printf("\n");
		}
		break;
	default:
		printf("Unknown routing \n.");
		exit(-1);
	}
}


