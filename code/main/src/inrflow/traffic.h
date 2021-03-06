#ifndef _traffic
#define _traffic

typedef struct flow_t {
	long src;
	long dst;
} flow_t;

char **traffic_param_tokens;


/**
 * @brief Free resources from traffic pattern.
 *
 * In future this can be a specific virtual function
 *
 */
void finish_traffic(long nparam);

void init_pattern_all2all(long servers, long nparam, long *params);
flow_t next_flow_all2all();

void init_pattern_all2one(long servers, long nparam, long *params);
flow_t next_flow_all2one();

void init_pattern_one2all(long servers, long nparam, long *params);
flow_t next_flow_one2all();

void init_pattern_rnd(long servers, long nparam, long *params);
flow_t next_flow_rnd();

void init_pattern_shift(long servers, long nparam, long *params);
flow_t next_flow_shift();

void init_pattern_bisection(long servers, long nparam, long *params);
flow_t next_flow_bisection();

void init_pattern_hotregion(long servers, long nparam, long *params);
flow_t next_flow_hotregion();

void init_pattern_butterfly(long nservers, long nparam, long *params);
flow_t next_flow_butterfly();

void init_pattern_many_all2all(long nservers, long nparam, long *params);
flow_t next_flow_many_all2all();

void init_pattern_many_all2all_rnd(long nservers, long nparam, long *params);
flow_t next_flow_many_all2all_rnd();

#endif // _traffic
