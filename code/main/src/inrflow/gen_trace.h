#ifndef _gen_trace
#define _gen_trace

#include "applications.h"

void insert_computation(application *app, list_t **task_events, long from, long computation);

void insert_send(application *app, list_t **task_events, long from, long to, long tag, long size);

void insert_recv(application *app, list_t **task_events, long to, long from, long tag, long size);

void gen_trace(application *app);

#endif
