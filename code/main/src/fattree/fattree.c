/** @mainpage
Fattree topology
*/

#include <stdlib.h>
#include <stdio.h>

#include "../inrflow/node.h"
#include "../inrflow/misc.h"
#include "fattree.h"

long param_k;	///< parameter k of the topology, typically number of levels/dimensions
long param_n;	///< parameter n of the topology, switch radix

long *n_pow;	///< an array with the n^i, useful for doing some calculations.
long servers; 	///< The total number of servers : n^k
long switches;	///< The total number of switches : k*n^(k-1)
long ports;		///< The total number of links
long sw_per_stage;  ///< Switches per stage

long *cur_route;    ///< Array to store the current route
long cur_hop;   ///< Current hop in this route

static char* network_token="fattree";
static char* routing_token="static";
static char* topo_version="v0.1";
static char* topo_param_tokens[2]= {"n","k"};
//AE: make error checking so that we don't overrun this buffer
extern char filename_params[100];

/**
* Initializes the topology and sets the parameters k & n.
*/
long init_topo_fattree(long np, long* par)
{
	long i;

	if (np<2) {
		printf("2 parameters are needed for Fattree <n, k>\n");
		exit(-1);
	}
	param_n=par[0];
	param_k=par[1];
	sprintf(filename_params,"k%ldn%ld",param_n,param_k);
	n_pow=malloc((param_k+1)*sizeof(long));
	cur_route=malloc(2*param_k*sizeof(long));   // UP*/DOWN routes cannot be longer than 2*k
	n_pow[0]=1;

	for (i=1; i<=param_k; i++) {
		n_pow[i]=n_pow[i-1]*param_n;
	} // powers of n will be useful throughout,so let's compute them just once.

	servers=n_pow[param_k];
	sw_per_stage=n_pow[param_k-1];
	switches=sw_per_stage*param_k;
	ports=sw_per_stage*param_n*param_k*2;

	return 1; //Return status, not used here.
}

/**
* Release the resources used by the topology.
**/
void finish_topo_fattree()
{
	free(n_pow);
	free(cur_route);
}

/**
* Get the number of servers of the network
*/
long get_servers_fattree()
{
	return servers;
}

/**
* Get the number of switches of the network
*/
long get_switches_fattree()
{
	return switches;
}

/**
* Get the number of ports of a given node (either a server or a switch, see above)
*/
long get_radix_fattree(long n)
{

	if (n<servers)
		return 1;	// If this is a server it has 1 port
	else if (n>=servers+((param_k-1)*sw_per_stage)) // this is a switch in the last stage (so upwards ports are disconnected.
		return param_n;
	else
		return 2*param_n; // If this is a switch it has n ports in each direction
}

/**
* Calculates connections
*/
tuple_t connection_fattree(long node, long port)
{
	tuple_t res;
	long sg; //subgroup
	long lvl;
	long pos;

	if (node<servers) {
		if (port!=0) {
			printf("Fattree servers only have 1 port!!!");
			res.node=-1; // switch 'sw' in level 'port'
			res.port=-1;
		} else {
			res.node=servers+(node/param_n);
			res.port=(node%param_n);
		}
	} else {
		lvl=(node-servers)/sw_per_stage; // level of the switch
		pos=(node-servers)%sw_per_stage; // position of the switch in that level
		if (lvl==param_k-1 && port>=param_n) {   //disconnected links in the last stage of the fattree (for regularity, could be connected to other subtrees)
			res.node=-1;
			res.port=-1;
		} else if (port<param_n && lvl==0) { // connected to server
			res.node=port+(pos*n_pow[1]);
			res.port=0;
		} else if (port>=param_n) { //upward port
			sg=pos/n_pow[lvl+1];
			res.node = mod((((port-param_n)*n_pow[lvl]) + (n_pow[lvl+1]*sg)+(pos%n_pow[lvl])), sw_per_stage) + servers+((lvl+1)*sw_per_stage);
			res.port = mod(((pos/n_pow[lvl])-(n_pow[lvl+1]*sg)), param_n);
		} else { //downwards port
			sg=pos/n_pow[lvl];
			res.node = mod(((port*n_pow[lvl-1]) + (n_pow[lvl]*sg)+(pos%n_pow[lvl-1])), sw_per_stage) + servers+((lvl-1)*sw_per_stage);
			res.port = param_n+mod(((pos/n_pow[lvl-1])-(n_pow[lvl]*sg)), param_n);
		}
	}
	return res;
}

long is_server_fattree(long i)
{
	return (i<servers);
}


char * get_network_token_fattree()
{
	return network_token;
}

char * get_routing_token_fattree()
{
	return routing_token;
}

char * get_topo_version_fattree()
{
	return topo_version;
}


char * get_topo_param_tokens_fattree(long i)
{
	return topo_param_tokens[i];
}

char * get_filename_params_fattree()
{
	return filename_params;
}

long get_server_i_fattree(long i)
{
	return i;
}

long get_switch_i_fattree(long i)
{
	return servers+i;
}


long node_to_server_fattree(long i)
{
	return i;
}

long node_to_switch_fattree(long i)
{
	return i-servers;
}

long get_ports_fattree()
{
	return ports;
}

/**
* Simple oblivious UP/DOWN. Others can be implemented.
*/
long init_routing_fattree(long src, long dst)
{
	long mca=0; // minimum common ancestor (levels)
	long i;

	while (src/n_pow[mca]!=dst/n_pow[mca]) {
		mca++;
	}

	cur_route[0]=0; //first hop is away from server, so no option to choose.

	for (i=1; i<mca; i++) { // Choose option based on source server, ensures load balancing.
		cur_route[i]=param_n+((src/n_pow[i-1]) % param_n);
	}

	for (i=0; i<mca; i++) {
		cur_route[mca+i]=(dst/n_pow[mca-1-i]) % param_n;
	}

	cur_hop=0;
	return 0;
}

void finish_route_fattree()
{

}
/**
* Return current hop. Calculated in init_routing
*/
long route_fattree(long current, long destination)
{
	return cur_route[cur_hop++];
}

