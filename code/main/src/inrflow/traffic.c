#include "traffic.h"
#include <stdio.h>
#include <stdlib.h>

long servers;   ///< Number of servers in this traffic pattern.
long src;       ///< current source
long dst;       ///< current destination
long curr_flows;	///< The current number of sent flows
long max_flows;		///< The total number of flows in this pattern
long group_size;	///< The number of servers per group in many_*
long curr_group;	///< The current group in many_*
long pn, pn1;		///< 2 ^ n & 2 ^ (n+1) - used for logarithmic patterns.
long *translate;    ///< Used to translate from consecutive to other placement (e.g. random)

/**
* Initializes the all2all pattern. all2all requires no parameter in the list.
* @param nservers The number of communicating servers
* @param nparam The number of parameters in the list of parameters
* @param params A list with the parameters for the traffic pattern
*/
void init_pattern_all2all(long nservers, long nparam, long *params)
{
	servers=nservers;
	src=0;
	dst=0;
}
/**
* Get next flow
* @see init_pattern_all2all
*/
flow_t next_flow_all2all()
{
	flow_t tmp;

	tmp.src=src;
	tmp.dst=dst;
	src++;
	if(src==servers) {
		src=0;
		dst++;
		if(dst==servers) {
			src=-1;
			dst=-1;
		}
	}
	return tmp;
}


/**
* Initializes the butterfly pattern. all2all requires no parameter
* @param nservers The number of communicating servers
* @param nparam The number of parameters in the list of parameters
* @param params A list with the parameters for the traffic pattern
*/
void init_pattern_butterfly(long nservers, long nparam, long *params)
{
	servers=nservers;
	src=0;
	dst=1;
	pn=1;
	pn1=2;
}

/**
* Get next flow
* @see init_pattern_butterfly
*/
flow_t next_flow_butterfly()
{
	flow_t tmp;


	tmp.src=src;
	tmp.dst=dst;
	do {
		src++;
		if (src%pn1<pn)
			dst=src+pn;
		else
			dst=src-pn;

		if(src==servers) {
			src=0;
			pn=pn1;
			pn1=pn1*2;
			if(pn>servers) {
				src=-1;
				dst=-1;
			} else {
				if (src%pn1<pn)
					dst=src+pn;
				else
					dst=src-pn;
			}
		}
	} while(src>=servers || dst>=servers);
	return tmp;
}

/**
* Initializes the many_all2all pattern. It requires 1 parameter: the group size. all2all groups are sequential.
* @param nservers The number of communicating servers
* @param nparam The number of parameters in the list of parameters
* @param params A list with the parameters for the traffic pattern
*/
void init_pattern_many_all2all(long nservers, long nparam, long *params)
{
	servers=nservers;
	src=0;
	dst=0;
	if(nparam<1) {
		printf("1 parameter is needed for MANY_ALL2ALL <#group_size>\n");
		exit(-1);
	}
	curr_flows=0;
	group_size=params[0];
	curr_group=0;
}

/**
* Get next flow
* @see init_pattern_many_all2all
*/
flow_t next_flow_many_all2all()
{
	flow_t tmp;

	tmp.src=src;
	tmp.dst=dst;

	src++;
	if(src%group_size==0 || src==servers ) {
		src=curr_group*group_size;
		dst++;
		if(dst==servers) {
			src=-1;
			dst=-1;
		} else if(dst%group_size==0) {
			curr_group++;
			src=curr_group*group_size;
		}
	}
	return tmp;
}

/**
* Initializes the many_all2all_rnd pattern. It requires 1 parameter: the group size. all2all groups are chosen randomly.
* @param nservers The number of communicating servers
* @param nparam The number of parameters in the list of parameters
* @param params A list with the parameters for the traffic pattern
*/
void init_pattern_many_all2all_rnd(long nservers, long nparam, long *params)
{
	long i,r,tmp;

	servers=nservers;
	src=0;
	dst=0;

	translate=malloc(nservers*sizeof(long));
	for (i=0; i<nservers; i++) {
		translate[i]=i;
	}
	for (i=0; i<nservers; i++) {
		r=rand()%nservers;
		tmp=translate[i];
		translate[i]=translate[r];
		translate[r]=tmp;
	}

	if(nparam<1) {
		printf("1 parameter is needed for MANY_ALL2ALL_RND <#group_size>\n");
		exit(-1);
	}
//    else
//        printf("Running with MANY_ALL2ALL_RND %ld\n",params[0]);
	curr_flows=0;
	group_size=params[0];
	curr_group=0;
}

/**
* Get next flow
* @see init_pattern_many_all2all_rnd
*/
flow_t next_flow_many_all2all_rnd()
{
	flow_t tmp;

	if (src==-1 || dst==-1) {
		tmp.src=-1;
		tmp.dst=-1;
	} else {
		tmp.src=translate[src];
		tmp.dst=translate[dst];
	}

	src++;
	if(src%group_size==0 || src==servers ) {
		src=curr_group*group_size;
		dst++;
		if(dst==servers) {
			src=-1;
			dst=-1;
		} else if(dst%group_size==0) {
			curr_group++;
			src=curr_group*group_size;
		}
	}
	return tmp;
}

/**
* Initializes the all2one pattern. It requires no parameter.
* @param nservers The number of communicating servers
* @param nparam The number of parameters in the list of parameters
* @param params A list with the parameters for the traffic pattern
*/
void init_pattern_all2one(long nservers, long nparam, long *params)
{
	servers=nservers;
	src=0;
	dst=0;
}

/**
* Get next flow
* @see init_pattern_all2one
*/
flow_t next_flow_all2one()
{
	flow_t tmp;

	tmp.src=src;
	tmp.dst=dst;
	src++;
	if(src==servers) {
		src=-1;
		dst=-1;
	}
	return tmp;
}

/**
* Initializes the one2all pattern. It requires no parameter.
* @param nservers The number of communicating servers
* @param nparam The number of parameters in the list of parameters
* @param params A list with the parameters for the traffic pattern
*/
void init_pattern_one2all(long nservers, long nparam, long *params)
{
	servers=nservers;
	src=0;
	dst=0;
}

/**
* Get next flow
* @see init_pattern_one2all
*/
flow_t next_flow_one2all()
{
	flow_t tmp;

	tmp.src=src;
	tmp.dst=dst;
	dst++;
	if(dst==servers) {
		src=-1;
		dst=-1;
	}
	return tmp;
}

/**
* Initializes the random pattern. It requires 1 parameter: the total number of flows. Flows are generated independently: source and destination are selected randomly using a uniform distribution.
* @param nservers The number of communicating servers
* @param nparam The number of parameters in the list of parameters
* @param params A list with the parameters for the traffic pattern
*/
void init_pattern_rnd(long nservers, long nparam, long *params)
{
	servers=nservers;
	if(nparam<1) {
		printf("1 parameter is needed for RANDOM <#flows>\n");
		exit(-1);
	}
	curr_flows=0;
	max_flows=params[0];
}

/**
* Get next flow
* @see init_pattern_rnd
*/
flow_t next_flow_rnd()
{
	flow_t tmp;
	if (curr_flows++ == max_flows) {
		tmp.src = -1;
		tmp.dst = -1;
	} else {
		tmp.src = rand()%servers;
		tmp.dst = rand()%servers;
	}
	return tmp;
}

/**
* Initializes the random pattern. It requires 1 parameter: the total number of flows. Flows are generated independently: source is selected randomly using a uniform distribution whereas destination follows a hot region distribution (the first 12.5% of the servers have 25% chances of being selected, the rest is uniform).
* @param nservers The number of communicating servers
* @param nparam The number of parameters in the list of parameters
* @param params A list with the parameters for the traffic pattern
*/
void init_pattern_hotregion(long nservers, long nparam, long *params)
{
	servers=nservers;
	if(nparam<1) {
		printf("1 parameter is needed for HOTREGION <#flows>\n");
		exit(-1);
	}
	curr_flows=0;
	max_flows=params[0];
}

/**
* Get next flow
* @see next_flow_hotregion
*/
flow_t next_flow_hotregion()
{
	flow_t tmp;
	if (curr_flows++ == max_flows) {
		tmp.src = -1;
		tmp.dst = -1;
	} else {
		tmp.src=rand()%servers;
		if (rand()%100 < 25)
			tmp.dst=rand()%(servers/8);
		else
			tmp.dst=rand()%servers;
	}
	return tmp;
}
