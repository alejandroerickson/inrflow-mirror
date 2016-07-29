#ifndef _collectives
#define _collectives

#include "../inrflow/list.h"

void all2all(application *app, list_t **task_events, long nodes, long size, long computation, long phases);

void many_all2all(application *app, list_t **task_events, long nodes, long size, long computation, long phases);

void many_all2all_rnd(application *app, list_t **task_events, long nodes, long size, long computation, long phases);

void one2all(application *app, list_t **task_events, long nodes, long size, long computation, long phases);

void one2all_rnd(application *app, list_t **task_events, long nodes, long size, long computation, long phases);

void all2one(application *app, list_t **task_events, long nodes, long size, long computation, long phases);

void all2one_rnd(application *app, list_t **task_events, long nodes, long size, long computation, long phases);

void ptp(application *app, list_t **task_events, long nodes, long size, long computation, long phases);

void randomapp(application *app, list_t **task_events, long nodes, long size, long computation, long phases);

#endif
