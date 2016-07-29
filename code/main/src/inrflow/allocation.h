#ifndef _allocation
#define _allocation

#include "applications.h"

long allocate_application(application *next_app);

void release_application(application *app);

#endif

