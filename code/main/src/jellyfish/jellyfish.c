/** @mainpage
 * Jellyfish topology
 */

#include <stdio.h>
#include <stdlib.h>
#include "jellyfish.h"
#include "rg_gen.h"
#include "routing_jellyfish.h"
#include "../inrflow/globals.h"

long param_n;
long param_k;
long param_r;
long param_seed;
long param_routing_type;
long param_routing_k;

static long src_aux;
static long dst_aux;
static long n_path;
static long c_path;
long n_kpaths;

long servers;   ///< The total number of servers
long switches;  ///< The total number of switches
long ports_network;	///< The total number of links
long total_ports;
long ports_switches;
long ports_servers;

static char *network_token="jellyfish";
static char routing_token[50];
static char *topo_version="v0.2";
static char *topo_param_tokens[4]= {"n", "k", "r", "seed"};
static char *routing_param_tokens[1]= {"k"};

char filename_params[100];

graph_t *random_graph;
k_paths **routing_table;

/**
 * Initializes the topology and sets the parameters n & k & r.
 */
long init_topo_jellyfish(long np, long *par)
{

    long i,j;

    if (np < 4) {
        printf("4 parameters are needed for JELLYFISH <n, k, r, seed>.\n");
        exit(-1);
    }
    param_n = par[0];
    param_k = par[1];
    param_r = par[2];
    param_seed = par[3];
    if (param_r < 3) {
        printf("The number of ports must be higher than 2.\n");
        exit(-1);
    }
    sprintf(filename_params,"n%ldk%ldr%lds%ld",param_n,param_k, param_r, param_seed);
    servers = param_n * (param_k - param_r);
    switches = param_n;
    ports_network = (param_n * param_k) + (param_n * param_r);
    total_ports = param_k;
    ports_switches = param_r;
    ports_servers = param_k - param_r;
    random_graph = malloc(sizeof(graph_t) * switches);

    for(i = 0; i < switches; i++) {
        random_graph[i].nedges = get_radix_jellyfish(i + servers) - ports_servers;
        random_graph[i].fedges = random_graph[i].nedges;
        random_graph[i].active = 1;
        random_graph[i].edge = malloc(sizeof(edge_t) * random_graph[i].nedges);
        for(j = 0; j < random_graph[i].nedges; j++) {
            random_graph[i].edge[j].neighbour.node = -1;
            random_graph[i].edge[j].neighbour.edge = -1;
            random_graph[i].edge[j].active = 1;
        }
    }
    generate(random_graph, switches, ports_switches, param_seed);
    //export_graph(random_graph,switches);
    generate_routing_table(ports_switches, ports_servers);
    return(1);
}

/**
 * Releases the resources used by jellyfish (random graph and routing table).
 */
void finish_topo_jellyfish()
{

    long i;

    destroy_routing_table();

    for(i = 0; i < switches; i++) {
        free(random_graph[i].edge);
    }
    free(random_graph);
}


/**
 * Get the number of servers of the network.
 */
long get_servers_jellyfish(){

    return servers;
}

/**
 * Get the number of switches of the network
 */
long get_switches_jellyfish(){

    return switches;
}

/**
 * Get the number of ports of the network
 */
long get_ports_jellyfish(){

    return ports_network;
}

/**
 * Get the number of ports of a given node (either a server or a switch, see above)
 */
long get_radix_jellyfish(long i){

    if(i < servers)
        return 1;
    else
        return param_k;
}

/**
 * Checks if a node is a server.
 */
long is_server_jellyfish(long i)
{
    if(i < servers)
        return 1;
    else
        return 0;
}

/**
 * Get the id of a server.
 */
long get_server_i_jellyfish(long i)
{

    return i;
}

/**
 * Get the id of a switch.
 */
long get_switch_i_jellyfish(long i)
{

    return i + servers;
}

long node_to_server_jellyfish(long i)
{
    return(i);
}

long node_to_switch_jellyfish(long i)
{
    return(i-servers);
}

/**
 * Calculates connections of the network.
 */
tuple_t connection_jellyfish(long node, long port)
{

    long node_aux, port_aux;
    tuple_t res;

    if(node < servers) {
        res.node = (node / ports_servers) + servers;
        res.port = node % ports_servers;
    } else if(port < ports_servers) {
        res.node = ((node - servers) * ports_servers) + port;
        res.port = 0;
    } else {
        node_aux = random_graph[node - servers].edge[port-ports_servers].neighbour.node;
        port_aux = random_graph[node - servers].edge[port-ports_servers].neighbour.edge;
        if(node_aux != -1 || port_aux != -1) {
            res.node = node_aux + servers;
            res.port = port_aux + ports_servers;
        } else {
            res.node = node_aux;
            res.port = port_aux;
        }
    }

    return(res);
}

/**
 * Get the network type.
 */
char * get_network_token_jellyfish()
{

    return(network_token);
}

/**
 * Get the routing token.
 */
char * get_routing_token_jellyfish(){

    switch(routing) {
        case JELLYFISH_SHORTEST_PATH_ROUTING:
            snprintf(routing_token, 50, "sp");
            break;
        case JELLYFISH_K_SHORTEST_PATHS_ROUTING:
            snprintf(routing_token, 50, "ksp%ld",routing_params[0]);
            break;
        case JELLYFISH_ECMP_ROUTING:
            snprintf(routing_token, 50, "ecmp%ld",routing_params[0]);
            break;
        case JELLYFISH_LLSKR_ROUTING:
            snprintf(routing_token, 50, "llksr%ld",routing_params[0]);
            break;
        default:
            printf("Unknown routing %ld", param_routing_type);
            exit(-1);
    }

    return(routing_token);
}

char *get_routing_param_tokens_jellyfish(long i){

    return routing_param_tokens[i];
}

/**
 * Get the version of the topology.
 */
char * get_topo_version_jellyfish(){

    return(topo_version);
}

/**
 * Get the parameters of the topology.
 */
char * get_topo_param_tokens_jellyfish(long i)
{
    return(topo_param_tokens[i]);
}

/**
 * Get the filename in which return the results..
 */
char * get_filename_params_jellyfish()
{
    return(filename_params);
}

/**
 * Init the routing each time a flow has to be send (Not required).
 */
long init_routing_jellyfish(long src, long dst){

    long dst2_aux;

    src_aux = (src / ports_servers);
    dst_aux = (dst / ports_servers);
    dst2_aux = (dst % ports_servers);
    c_path = 1;

    switch(routing) {
        case JELLYFISH_SHORTEST_PATH_ROUTING:
        case JELLYFISH_K_SHORTEST_PATHS_ROUTING:
        case JELLYFISH_ECMP_ROUTING:
            break;
            n_path = routing_table[src_aux][dst_aux].last_used[0];
            routing_table[src_aux][dst_aux].last_used[0] = ((routing_table[src_aux][dst_aux].last_used[0] + 1) % routing_table[src_aux][dst_aux].n_paths);
        case JELLYFISH_LLSKR_ROUTING:
            n_path = ((dst_aux * n_kpaths) + routing_table[src_aux][dst_aux].last_used[dst2_aux]) % routing_table[src_aux][dst_aux].n_paths;

            routing_table[src_aux][dst_aux].last_used[dst2_aux] = ((routing_table[src_aux][dst_aux].last_used[dst2_aux] + 1) % n_kpaths);
            break;
        default:
            printf("Unknown routing.\n");
            exit(-1);
    }

    //exit(0);
    return(0);
}

/**
 * Finish the routing (Not required).
 */
void finish_route_jellyfish(){
}

/**
 * Route a flow from a source to a destination.
 */
long route_jellyfish(long current, long destination)
{

    long i;
    long next = -1;
    long port = -1;

    switch(routing) {
        case JELLYFISH_SHORTEST_PATH_ROUTING:
            if(current < servers) {
                port = 0;
                c_path++;
            } else {
                next = routing_table[src_aux][dst_aux].paths[n_path][c_path++];
                if((current- servers) == dst_aux) {
                    port = (destination % ports_servers);
                } else {
                    for(i = 0; i < random_graph[current - servers].nedges; i++) {
                        if(random_graph[current - servers].edge[i].neighbour.node == next) {
                            port = i + ports_servers;
                            break;
                        }
                    }
                }
            }
            break;
        case JELLYFISH_K_SHORTEST_PATHS_ROUTING:
            if(current < servers) {
                port = 0;
                c_path++;
            } 
            else {
                next = routing_table[src_aux][dst_aux].paths[n_path][c_path++];
                if((current - servers) == dst_aux) {
                    port = (destination % ports_servers);
                } else {
                    for(i = 0; i < random_graph[current - servers].nedges; i++) {
                        if(random_graph[current - servers].edge[i].neighbour.node == next) {
                            port = i + ports_servers;
                            break;
                        }
                    }
                }
            }
            break;
        case JELLYFISH_ECMP_ROUTING:
            if(current < servers) {
                port = 0;
                c_path++;
            } else {
                next = routing_table[src_aux][dst_aux].paths[n_path][c_path++];
                if((current- servers) == dst_aux) {
                    port = (destination % ports_servers);
                } else {
                    for(i = 0; i < random_graph[current - servers].nedges; i++) {
                        if(random_graph[current - servers].edge[i].neighbour.node == next) {
                            port = i + ports_servers;
                            break;
                        }
                    }
                }
            }
            break;
        case JELLYFISH_LLSKR_ROUTING:
            if(current < servers) {
                port = 0;
                c_path++;
            } else {
                next = routing_table[src_aux][dst_aux].paths[n_path][c_path++];
                if((current- servers) == dst_aux) {
                    port = (destination % ports_servers);
                } else {
                    for(i = 0; i < random_graph[current - servers].nedges; i++) {
                        if(random_graph[current - servers].edge[i].neighbour.node == next) {
                            port = i + ports_servers;
                            break;
                        }
                    }
                }
            }
            break;

            break;
        default:
            printf("Unknown routing %ld", param_routing_type);
            exit(-1);
    }
    return(port);
}

/**
 * Search the set of switches at a given distance dist from switch src.
 * Returns the set of switches in dsts and the size of the set.
 */
long nodes_distance_jellyfish(long src, long *dsts, long dist){

    long i, j;
    long num = 0;

    if(dist == 0){
        dsts[num++] = src;
    }
    else{
        for(i = 0; i < switches; i++){
            for(j = 0; j < routing_table[src][i].n_paths; j++){
                if(i != src && ((dist + 1) == routing_table[src][i].paths[j][0])){
                    dsts[num++] = i;
                    break;
                }           
            }
        }
    }
    return(num);
}

long *get_path(long src, long dst, long n_path){
    
    return(routing_table[src][dst].paths[n_path]);    
}


