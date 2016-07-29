#ifndef _trace
#define _trace

void read_trc(application *app, list_t **task_events, long size);

void read_fsin_trc(application *app, list_t **task_events, long size);

void read_alog_trc(application *app, list_t **task_events, long size);

#endif
