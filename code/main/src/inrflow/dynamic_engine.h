#ifndef _dynamic_engine
#define _dynamic_engine

#include "applications.h"
#include "list.h"

#include "node.h"
void update_events(unsigned long long time_next_app);

long time_next_event(list_t *list_cpus, list_t *list_flows);

long insert_new_events(application *app, long ntask);

void update_cpus(list_t *list_cpus, long t_next);

void update_flows(list_t *list_flows, long t_next);

long mark_route_dynamic(long id, dflow_t **flow, long src, long dst);

void remove_flow(dflow_t *flow);

long remove_send_event(list_t *events, long id, long pid, long pid2, long length);

long remove_reception_event(list_t *events, long id, long pid, long pid2, long length);

void min_links_bandwidth();

#endif
