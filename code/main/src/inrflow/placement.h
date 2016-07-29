#ifndef _placement
#define _placement

void init_placement(long ntasks);

long task_to_server(long task_id);

void finish_placement();
#endif
