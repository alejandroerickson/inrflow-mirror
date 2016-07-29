/** @mainpage
thintree topology
*/

#include <stdlib.h>
#include <stdio.h>

#include "../inrflow/node.h"
#include "../inrflow/misc.h"
#include "thintree.h"

long param_k;	///< parameter k of the topology, typically number of stages
long param_down;	///< Number of downwards ports
long param_up;  ///< number of upwards ports

long *down_pow;	///< an array with the down^i, useful for doing some calculations.
long *up_pow;	///< an array with the up^i, useful for doing some calculations.
long servers; 	///< The total number of servers : n^k
long switches;	///< The total number of switches : k*n^(k-1)
long ports;		///< The total number of links
long *sw_per_stage;  ///< Switches per stage
long last_stage;    ///< id of the first switch in the last stage

long *cur_route;    ///< Array to store the current route
long cur_hop;   ///< Current hop in this route

static char* network_token="thintree";
static char* routing_token="static";
static char* topo_version="v0.1";
static char* topo_param_tokens[3]= {"down","up", "stages"};
//AE: make error checking so that we don't overrun this buffer
extern char filename_params[100];

/**
* Initializes the topology and sets the parameters k & n.
*/
long init_topo_thintree(long np, long* par)
{
	long i;

	if (np<2) {
		printf("3 parameters are needed for thintree <down ports, up ports, stages>\n");
		exit(-1);
	}
	param_down=par[0];
	param_up=par[1];
	param_k=par[2];
	sprintf(filename_params,"down%ldup%ldk%ld",param_down,param_up,param_k);
	up_pow=malloc((param_k+1)*sizeof(long));
	down_pow=malloc((param_k+1)*sizeof(long));
	cur_route=malloc(2*param_k*sizeof(long));   // UP*/DOWN routes cannot be longer than 2*k
	sw_per_stage=malloc(param_k*sizeof(long));
	down_pow[0]=1;
    up_pow[0]=1;

	for (i=1; i<=param_k; i++) {
		down_pow[i]=down_pow[i-1]*param_down;
		up_pow[i]=up_pow[i-1]*param_up;
	} // powers of up and down will be useful throughout,so let's compute them just once.

    switches=0;
	for (i=0; i<param_k; i++) {
		sw_per_stage[i]=down_pow[param_k-i-1]*up_pow[i];
		switches+=sw_per_stage[i];
	}

	servers=down_pow[param_k];
	ports=(switches*(param_down+param_up))-(sw_per_stage[param_k-1]*param_up)+(servers);

    last_stage=servers+switches-sw_per_stage[param_k-1];

	return 1; //Return status, not used here.
}

/**
* Release the resources used by the topology.
**/
void finish_topo_thintree()
{
	free(cur_route);
	free(up_pow);
	free(down_pow);
	free(sw_per_stage);
}

/**
* Get the number of servers of the network
*/
long get_servers_thintree()
{
	return servers;
}

/**
* Get the number of switches of the network
*/
long get_switches_thintree()
{
	return switches;
}

/**
* Get the number of ports of a given node (either a server or a switch, see above)
*/
long get_radix_thintree(long n)
{

	if (n<servers)
		return 1;	// If this is a server it has 1 port
	else if (n>=last_stage) // this is a switch in the last stage (so upwards ports are disconnected.
		return param_down;
	else
		return param_down+param_up; // If this is a switch it has n ports in each direction
}

/**
* Calculates connections
*/
tuple_t connection_thintree(long node, long port)
{
	tuple_t res;
	long lvl;
	long pos;
	long nl_first; // id of the NEIGHBOURING level first switch

	if (node<servers) {
		if (port!=0) {
			//printf("thintree servers only have 1 port!!!");
			res.node=-1; // switch 'sw' in level 'port'
			res.port=-1;
		} else {
			res.node=servers+(node/param_down);
			res.port=(node%param_down);
		}
	} else {
	    pos=node-servers;
	    lvl=0;
        nl_first=servers;
	    while (pos>=sw_per_stage[lvl]){
            pos-=sw_per_stage[lvl];
            nl_first+=sw_per_stage[lvl];
            lvl++;
	    }
		if (lvl==param_k-1 && port>=param_down) {   //disconnected links in the last stage of the thintree (for regularity, could be connected to other subtrees)
			res.node=-1;
			res.port=-1;
		} else if (port<param_down && lvl==0) { // connected to server
			res.node=port+(pos*down_pow[1]);
			res.port=0;
		} else if (port>=param_down) { //upward port
			//sg=pos/n_pow[lvl+1];
			long p=port-param_down;
            nl_first=nl_first+sw_per_stage[lvl]; // we are connecting up, so the neighbouring level is above, we need to add the number of switches in this level

			res.node = (p*up_pow[lvl]) + (mod(pos,up_pow[lvl])) + ((pos/(param_down*up_pow[lvl]))*up_pow[lvl+1]) + nl_first;
			res.port = (mod(pos/up_pow[lvl],param_down));
		} else { //downwards port
			nl_first-=sw_per_stage[lvl-1]; // we are connecting down, so the neighbouring level is below, we need to substract the number of switches in the previous level

            //printf("[%ld.%ld] :: %ld + %ld + %ld + %ld = %ld\n", node,port,(port*up_pow[lvl-1]), (mod(pos,up_pow[lvl-1])), ((pos/up_pow[lvl])*up_pow[lvl+1]), nl_first, (port*up_pow[lvl-1]) + (mod(pos,up_pow[lvl-1])) + ((pos/up_pow[lvl])*up_pow[lvl-1]*param_down) + nl_first);
			res.node = (port*up_pow[lvl-1]) + (mod(pos,up_pow[lvl-1])) + ((pos/up_pow[lvl])*up_pow[lvl-1]*param_down)  + nl_first;
			res.port = param_down + mod(pos/up_pow[lvl-1], param_up);
        }
	}

	return res;
}

long is_server_thintree(long i)
{
	return (i<servers);
}


char * get_network_token_thintree()
{
	return network_token;
}

char * get_routing_token_thintree()
{
	return routing_token;
}

char * get_topo_version_thintree()
{
	return topo_version;
}


char * get_topo_param_tokens_thintree(long i)
{
	return topo_param_tokens[i];
}

char * get_filename_params_thintree()
{
	return filename_params;
}

long get_server_i_thintree(long i)
{
	return i;
}

long get_switch_i_thintree(long i)
{
	return servers+i;
}


long node_to_server_thintree(long i)
{
	return i;
}

long node_to_switch_thintree(long i)
{
	return i-servers;
}

long get_ports_thintree()
{
	return ports;
}

/**
* Simple oblivious UP/DOWN. Others can be implemented.
*/
long init_routing_thintree(long src, long dst)
{
	long mca=0; // minimum common ancestor (levels)
	long i;

	while (src/down_pow[mca]!=dst/down_pow[mca]) {
		mca++;
	}

	cur_route[0]=0; //first hop is away from server, so no option to choose.
	for (i=1; i<mca; i++) { // Choose option based on source server, ensures load balancing.
		cur_route[i]=param_down+((src/down_pow[i-1]) % param_up);
	}

	for (i=0; i<mca; i++) {
		cur_route[mca+i]=(dst/down_pow[mca-1-i]) % param_down;
	}

	cur_hop=0;
	return 0;
}

void finish_route_thintree()
{

}
/**
* Return current hop. Calculated in init_routing
*/
long route_thintree(long current, long destination)
{
	return cur_route[cur_hop++];
}

