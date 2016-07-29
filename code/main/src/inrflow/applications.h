#ifndef _applications
#define _applications

#include "metrics.h"
#include "misc.h"
#include "list.h"

typedef struct application{

    app_metrics info;

    long size;
    long tasks_finished;
    long packets;
    long comp_time;
    long phases;
    long running;
    tpattern_t pattern; ///< the traffic pattern we are using in the simulation
    char pattern_type[50];
    allocation_t allocation;
    long pattern_nparam;
    long pattern_params[MAX_TRAFFIC_PARAMS];
    char pattern_file[50];
    char allocation_type[50];
    long *translation;
    long *remaining_flows;
    node_list *node;
    list_t **task_events;
    list_t **task_events_occurred;

} application;

/** 
 * Types of event.
 * 
 * It should be 'r' for a reception, 's' for a sent or 'c' for a computation event.
 */
typedef enum event_t {
    RECEPTION = 'r',        ///< Reception
    SENDING = 's',          ///< Sent
    COMPUTATION = 'c'       ///< Computation
} event_t;

typedef struct route_t{
    int node;
    short port;
    struct node_list *n_l;
} route_t;

typedef struct dflow_t{
    long start_time;
    float speed;
    list_t path;
    
}dflow_t;

/**
 * Structure that defines an event.
 *
 * It contains all the needed atributes for distinguish it:
 * type (S/R/C), the second node id (destination/source), a task id,
 * the length in packets, and the packets sent/arrived.
 */
typedef struct event {
    event_t type;   ///< Type of the event (Reception / Sent / Computation).
    int id;
    int pid;               ///< The other node (processor id).
    int pid2;               ///< The other node (processor id).
    int length;    ///< Length of the message in packets. Number of cycles in computation.
    int count;             ///< The number of packets sent/arrived. Number of elapsed cycles when running.
    struct dflow_t dflow;
    struct application *app;
} event;

void init_workload(list_t *list);

void init_running_application(application *next_app);

void finish_application(application *app);
#endif

