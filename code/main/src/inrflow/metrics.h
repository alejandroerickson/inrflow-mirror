#ifndef _metrics
#define _metrics

#include "list.h"

typedef struct scheduling_metrics{
    unsigned long long makespan;
    double avg_waiting_time;

} scheduling_metric;

typedef struct app_metrics{
    long id;
    unsigned long long arrive_time;
    unsigned long long start_time;
    unsigned long long end_time;
    unsigned long long runtime;
    unsigned long long waiting_time;
    long n_flows;
    long flows_distance;
    long flows_latency;
    float avg_flows_distance;
    float avg_flows_latency;
} app_metrics;

typedef struct applications_metrics{
    long n_apps;
    list_t apps;
} applications_metrics;

typedef struct execution_metrics{
    unsigned long long runtime;
    double avg_latency;
    float avg_agg_bw;
    long n_steps;
    list_t agg_bw;
    long *links_info;
} execution_metrics;

typedef struct metrics_t{
    struct scheduling_metrics scheduling;
    struct applications_metrics applications;
    struct execution_metrics execution;
} metrics_t;

void init_metrics(struct metrics_t *metrics);

unsigned long long get_makespan(struct metrics_t *metrics);

void set_makespan(struct metrics_t *metrics, unsigned long long makespan);

void update_metrics(struct metrics_t *metrics);

void report_metrics(struct metrics_t *metrics);
#endif
