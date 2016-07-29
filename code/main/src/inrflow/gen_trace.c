#include "applications.h"
#include "../kernels/trace.h"
#include "../kernels/collectives.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>

#define op_per_cycle 50         ///< balances the computation time (cpu time/op_per_cycle)==fsin cycles.
#define cpuspeed  1e6           ///< The cpu speed in Mhz.

/* *
 * * Print a send line in the standard output.
 * *
 * *@param from    The source node.
 * *@param to      The destination node.
 * *@param tag     The MPI tag for this message.
 * *@param size    The size in bytes of the message.
 * */
void insert_computation(application *app, list_t **task_events, long from, long computation)
{
    event ev;
    
    ev.type = COMPUTATION;
    ev.id = 0;
    ev.count = computation;
    ev.length = computation;
    ev.pid = from;
    ev.pid2 = -1;
    ev.app = app;
    list_append(task_events[from], &ev);
}

/* *
 * * Print a send line in the standard output.
 * *
 * *@param from    The source node.
 * *@param to      The destination node.
 * *@param tag     The MPI tag for this message.
 * *@param size    The size in bytes of the message.
 * */
void insert_send(application *app, list_t **task_events, long from, long to, long tag, long size)
{
    event ev;
    
    ev.type = SENDING;
    ev.id = tag;
    ev.pid = from;
    ev.length = size;
    ev.count = size;
    ev.pid2 = to;
    ev.app = app;
    ev.dflow.speed = FLT_MAX;
    list_initialize(&ev.dflow.path, sizeof(route_t));
    list_append(task_events[from], &ev);
}

void insert_recv(application *app, list_t **task_events, long to, long from, long tag, long size)
{
    event ev;
    
    ev.type = RECEPTION;
    ev.id = tag;
    ev.pid = from;
    ev.length = size;
    ev.count = size;
    ev.pid2 = to;
    ev.app = app;
    ev.dflow.speed = FLT_MAX;
    list_append(task_events[from], &ev);
}

void gen_trace(application *app){

    switch(app->pattern){
        case ALL2ALL:
            all2all(app, app->task_events, app->size, app->packets, app->comp_time, app->phases);
            break;
        case MANYALL2ALL:
            many_all2all(app, app->task_events, app->size, app->packets, app->comp_time, app->phases);
            break;
        case MANYALL2ALLRND:
            many_all2all_rnd(app, app->task_events, app->size, app->packets, app->comp_time, app->phases);
            break;
        case ONE2ALL:
            one2all(app, app->task_events, app->size, app->packets, app->comp_time, app->phases);
            break;
        case ONE2ALLRND:
            one2all_rnd(app, app->task_events, app->size, app->packets, app->comp_time, app->phases);
            break;
        case ALL2ONE:
            all2one(app, app->task_events, app->size, app->packets, app->comp_time, app->phases);
            break;
        case ALL2ONERND:
            all2one_rnd(app, app->task_events, app->size, app->packets, app->comp_time, app->phases);
            break;
    	case PTP:
            ptp(app, app->task_events, app->size, app->packets, app->comp_time, app->phases);
            break;
        case RANDOM:
            randomapp(app, app->task_events, app->size, app->packets, app->comp_time, app->phases);
            break;
        case FILE_PATTERN:
            read_trc(app, app->task_events, app->size);
            break;

    /* 
        case MESH2D:
              case ALLTOALL:
            alltoall(nprocs, packets, comp_time,phases);
            break;
        case ONETOALL:
            onetoall(nprocs, packets, comp_time,phases);
            break;
        case ALLTOONE:
            alltoone(nprocs, packets, comp_time,phases);
            break;
        case ALLTOALL:
            alltoall(nprocs, packets, comp_time,phases);
            break;
        case ONETOALL:
            onetoall(nprocs, packets, comp_time,phases);
            break;
        case ALLTOONE:
            alltoone(nprocs, packets, comp_time,phases);
            break;
        mesh2d(nprocs, packets, comp_time,phases);
            break;			
        case TORUS2D:
            torus2d(nprocs, packets, comp_time,phases);
            break;
        case SWEEP2D:
            sweep2d(nprocs, packets, comp_time,phases);
            break;
        case RTORUS2D:
            break;
        case RSWEEP2D:
            break;
        case RMESH2D:
            break;
        case MESH3D:
            mesh3d(nprocs, packets, comp_time,phases);
            break;
        case TORUS3D:
            torus3d(nprocs, packets, comp_time,phases);
            break;
        case SWEEP3D:
            sweep3d(nprocs, packets, comp_time,phases);
            break;
        case RTORUS3D:
            break;
        case RWATERFALL:
            break;
        case WATERFALL:
            waterfall(nprocs, packets, comp_time,phases);
            break;
        case BINARYTREE:
            binaryTree(nprocs, packets, comp_time,phases);
            break;
        case BUTTERF:
            butterf(nprocs, packets, comp_time,phases);
            break;
        case GUPS:
            gups(nprocs, packets, comp_time,phases);
            break;
        case NBODIES:
            nbodies(nprocs, packets, comp_time,phases);
            break;
        */
       default:
            printf("Undefined application.\n");
	    exit(-1);
            break;
    }
}
