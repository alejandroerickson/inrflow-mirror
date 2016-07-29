#ifndef _allocation_jellyfish
#define _allocation_jellyfish

#include "../inrflow/applications.h"

long jellyfish_allocation_spread(application *next_app, long *cores);

long jellyfish_allocation_random(application *app, long *cores);

long jellyfish_allocation_locality(application *app, long *cores);

long jellyfish_allocation_quasicontiguous(application *app, long *cores);

long jellyfish_allocation_contiguous(application *next_app, long *cores);

void print_vector_aux(long size, long *vec);
#endif
