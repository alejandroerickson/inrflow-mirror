/**
* @file
* @brief	Some miscellaneus tools & definitions.
*
* Tools for memory allocation & error printing.
* Definition of some useful macros.
* Definition of some enumerations.
*/

#ifndef _misc
#define _misc

//#include "constants.h"
//#include <errno.h>
//#include <stdio.h>
//#include <stdlib.h>

/**
* Choose a random number in [ 0, m ).
*
* @param m maximum.
* @return A random number.
*/
#define ztm(m) (long) (m * ( (1.0*rand() ) / (RAND_MAX+1.0)))

/**
* Return the absolute value
*
* @param m The number.
* @return The absolute value of m.
*/
#define abs(m) ((m<0) ? (-m) : (m))

/**
* The sign of a numeric value.
*
* For zero, the return value is 1
*/
#define sign(x) (((x)<0 )? -1 : 1)

#define P_NULL (-1) ///< Definition of a NULL value.

/**
* Definition of the maximum chooser
*
* @param a One number.
* @param b Another One.
* @return The maximum of both.
*/
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

/**
* Definition of the minimum chooser
*
* @param a One number.
* @param b Another One.
* @return The minimum of both.
*/
#ifndef min
#define min(a,b)            (((a) > (b)) ? (b) : (a))
#endif

/**
* Definition of the module of a division
*
* @param a One number.
* @param b Another One.
* @return a mod b.
*/
#ifndef mod
#define mod(a,b)            ((((a)%(b)) <  0 ) ? ((a)%(b)) + (b) : ((a)%(b)) )
#endif

/**
* Definition of boolean values.
*/
typedef enum bool_t {
	FALSE = 0, TRUE = 1
} bool_t;

/**
* Definition of all accepted topologies.
*/
typedef enum topo_t {
	FICONN, KNKSTAR, DPILLAR, BCUBE, SWCUBE, SERVERCENTRIC, FATTREE, JELLYFISH
} topo_t;

typedef enum routing_t {
	DPILLAR_SINGLE_PATH_ROUTING,
	DPILLAR_RANDOM_DIRECTION_SP,
	DPILLAR_SHORTER_DIRECTION_SP,
	DPILLAR_MULTI_PATH_ROUTING,
	DPILLAR_RANDOM_DIRECTION_MP,
	DPILLAR_SHORTER_DIRECTION_MP,
	DPILLAR_MINIMAL_ROUTING,
	JELLYFISH_SHORTEST_PATH_ROUTING,
	JELLYFISH_K_SHORTEST_PATHS_ROUTING
} routing_t;

/**
* Classes of traffic sources.
*/
typedef enum tpattern_t {
	ALL2ALL,	// All-to-all
	ALL2ONE,     // All-to-one
	ONE2ALL,
	MANYALL2ALL,
	MANYALL2ALLRND,
	BUTTERFLY,
	HOTREGION,
	RANDOM
} tpattern_t;

#ifndef MAX_TRAFFIC_PARAMS
#define MAX_TRAFFIC_PARAMS 8
#endif

#ifndef MAX_TOPO_PARAMS
#define MAX_TOPO_PARAMS 8
#endif

#ifndef MAX_ROUTING_PARAMS
#define MAX_ROUTING_PARAMS 8
#endif

//AE: I think these values might be too low to be safe.
#ifndef UPPER_PATH_LENGTH
#define UPPER_PATH_LENGTH 100
#endif

#ifndef UPPER_SERVER_HOPS
#define UPPER_SERVER_HOPS 100
#endif

#endif /* _misc */

