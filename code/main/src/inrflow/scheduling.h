#ifndef _scheduling
#define _scheduling

/**
 * Structure that defines a physical node.
 */
typedef struct server_t{
    
    long n_cores;
    long busy_cores;
    long free_cores;
    long *cores;
    int cont;
    long memory;
    long mem_used;
} server_t;

typedef struct scheduling_info{

    unsigned long long makespan;
    long total_servers;
    long cores_server;
    long total_cores;
    long busy_cores;
    long free_cores;
    server_t *servers;

} scheduling_info;

void init_scheduling(long nservers);

unsigned long long schedule_next_application();

void finish_scheduling(long nservers);

unsigned long long fcfs();

long available_servers(long switch_id, long *av_servers, long ports_servers);

long available_cores(long server_id, long *available_cores, long ports_servers);

long are_available_servers(long switch_id, long ports_servers);
#endif
