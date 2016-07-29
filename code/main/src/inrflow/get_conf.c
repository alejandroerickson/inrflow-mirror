/**
* @file
* @brief	Gets the configuration of the Simulation.
*
* First take default values for the simulation.
* Second take the values from the file 'fsin.conf'.
* Finally take the command line arguments.

FSIN Functional Simulator of Interconnection Networks
Copyright (2003-2011) J. Miguel-Alonso, A. Gonzalez, J. Navaridas

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "misc.h"
#include "globals.h"
#include "literal.h"

#ifdef WIN32
#include <windows.h>
#endif

/** The Configuration file. */
#define DEFAULT_CONF_FILE "inrflow.conf"

/**
* Default values for options are specified here.
* @see literal.c
*/
static literal_t options_l[] = {
	{ 1,  "rnd_seed"},
	{ 1,  "rseed"},
	{ 1,  "rnds"},
	{ 4,  "failure_rate"},
	{ 4,  "PF"},
	{ 5,  "num_failures"},
	{ 6,  "topo"}, 
        { 6,  "topology"},
	{ 7,  "tpattern"},
	{ 7,  "traffic"},
	{ 8,  "routing"},
	{ 9,  "generate_bfs"},
	{ 9,  "gen_bfs"},
	{ 9,  "bfs"},
	{10, "compile_latex"},
	{10, "compile_pdf"},
	{10, "pdf"},
        {11, "placement"},
        {12, "placement_file"},
        {13, "workload"},
        {14, "scheduling"},
        {15, "allocation"},
        {16, "capacity"},
        {17, "mode"},
        {18, "injectionmode"},
        {18, "injmode"},
        {19, "metricsinterval"},
        {19, "metricsint"},
        {20, "verbose"},
        {21, "servers"},
	LITERAL_END
};

/**
* Boolean values to be read from config file are defined here.
* @see literal.c
*/
static literal_t bool_l[] = {
	{ 1,	"yes"},
	{ 1,	"true"},
	{ 1,	"y"},
	{ 0,	"no"},
	{ 0,	"false"},
	{ 0,    "n"},
	LITERAL_END
};

/**
* All the topologies allowed are specified here.
* @see literal.c
*/
literal_t topology_l[] = {
	{ FICONN,	"ficonn"},
	{ GDCFICONN,	"gdcficonn"},
	{ GDCFICONN,	"generalized-ficonn"},
	{ GDCFICONN,	"generalised-ficonn"},
	{ GDCFICONN,	"genficonn"},
	{ GDCFICONN,	"generalized-dcell"},
	{ GDCFICONN,	"generalised-dcell"},
	{ GDCFICONN,	"gendcell"},
	{ HCNBCN, "hcnbcn"},
	{ HCNBCN, "bcn"},
	{ HCNBCN, "hcn"},
	{ KNKSTAR,	"knkstar"},
	{ KNKSTAR,	"knk*"},
	{ SWCUBE,	"swcube"},
	{ DPILLAR,	"dpillar"},
	{ BCUBE,    "bcube"},
	{ FATTREE,  "fattree"},
	{ THINTREE, "thintree"},
	{ TORUS,  "torus"},
	{ JELLYFISH,"jellyfish"},
	LITERAL_END
};

literal_t routing_l[] = {
	{ DPILLAR_SINGLE_PATH_ROUTING,	 "dpillar-sp"},
	{ DPILLAR_RANDOM_DIRECTION_SP,	 "dpillar-sp-rnd"},
	{ DPILLAR_RANDOM_DIRECTION_SP,	 "dpillar-rnd-sp"},
	{ DPILLAR_SHORTER_DIRECTION_SP,	 "dpillar-sp-shd"},
	{ DPILLAR_SHORTER_DIRECTION_SP,	 "dpillar-shd-sp"},
	{ DPILLAR_MULTI_PATH_ROUTING,	 "dpillar-mp"},
	{ DPILLAR_RANDOM_DIRECTION_MP,	 "dpillar-mp-rnd"},
	{ DPILLAR_RANDOM_DIRECTION_MP,	 "dpillar-rnd-mp"},
	{ DPILLAR_SHORTER_DIRECTION_MP,	 "dpillar-mp-shd"},
	{ DPILLAR_SHORTER_DIRECTION_MP,	 "dpillar-shd-mp"},
	{ DPILLAR_MINIMAL_ROUTING,	 "dpillar-min"},
	{ DPILLAR_MINIMAL_ROUTING,	 "dpillar-minimal"},
	{ JELLYFISH_SHORTEST_PATH_ROUTING,	 "jellyfish-sp"},
	{ JELLYFISH_K_SHORTEST_PATHS_ROUTING,	 "jellyfish-ksp"},
	{ JELLYFISH_ECMP_ROUTING,	 "jellyfish-ecmp"},
	{ JELLYFISH_LLSKR_ROUTING,	 "jellyfish-llskr"},
	{GDCFICONN_DIMENSIONAL, "gdcficonn-dimensional"},
	{GDCFICONN_PROXY, "gdcficonn-proxy"},
	{HCNBCN_FDIM,"hcnbcn-fdim"},
	{HCNBCN_NEWFDIM,"hcnbcn-newfdim"},
	{HCNBCN_BDIM,"hcnbcn-bdim"},
	{HCNBCN_NEWBDIM,"hcnbcn-newbdim"},
	LITERAL_END
};

/**
* All the supported traffic patterns are specified here.
* @see literal.c
*/
literal_t tpatterns_l[] = {
	{ ALL2ALL,	"alltoall"},
	{ ALL2ALL,	"all2all"},
	{ ALL2ALL,  "a2a"},
	{ ALL2ONE,	"alltoone"},
	{ ALL2ONE,	"all2one"},
	{ ALL2ONE,  "a2o"},
	{ ALL2ONERND,  "all2onernd"},
	{ ONE2ALL,	"onetoall"},
	{ ONE2ALL,	"one2all"},
	{ ONE2ALL,  "o2a"},
	{ ONE2ALLRND,  "one2allrnd"},
	{ RANDOM,	"random"},
	{ RANDOM,   "rnd"},
	{ HOTREGION,	"hotregion"},
	{ HOTREGION,	"hot"},
	{ HOTREGION,	"hr"},
	{ MANYALL2ALL,	"manyalltoall"},
	{ MANYALL2ALL,	"manyall2all"},
	{ MANYALL2ALL,  "ma2a"},
	{ MANYALL2ALLRND,	"manyalltoallrnd"},
	{ MANYALL2ALLRND,	"manyall2allrnd"},
	{ MANYALL2ALLRND,  "ma2ar"},
	{ SHIFT,    "shift"},
	{ BISECTION,    "bisection"},
    	{ BUTTERFLY,	"butterfly"},
	{ BUTTERFLY,	"bf"},
        { PTP,         "ptp"},
        { FILE_PATTERN,         "file"},
	LITERAL_END
};

/**
* All the supported placement strategies are specified here.
* @see literal.c
*/

literal_t placement_l[] = {
    { SEQUENTIAL_PLC,  "sequential"},
    { RANDOM_PLC,   "random"},
    { PATTERN_PLC, "pattern"},
    { FILE_PLC, "file"},
    LITERAL_END
};

literal_t applications_l[] = {
    { NONE_APP,  "none"},
    { FILE_APP,   "file"},
    LITERAL_END
};

literal_t scheduling_l[] = {
    { FCFS,  "fcfs"},
    LITERAL_END
};

literal_t allocation_l[] = {
    { SEQUENTIAL_ALLOC,  "sequential"},
    { RANDOM_ALLOC,   "random"},
    { JELLYFISH_SPREAD_ALLOC, "jellyfish-spread"},
    { JELLYFISH_RANDOM_ALLOC, "jellyfish-random"},
    { JELLYFISH_CONTIGUITY_ALLOC, "jellyfish-cont"},
    { JELLYFISH_LOCALITY_ALLOC, "jellyfish-local"},
    { JELLYFISH_QUASICONTIGUITY_ALLOC, "jellyfish-quasicont"},
    LITERAL_END
};

literal_t rt_mode_l[] = {
    { STATIC,  "static"},
    { DYNAMIC,  "dynamic"},
    LITERAL_END
};

// some declarations.
void set_default_conf(void);
void get_option(char *);
void get_conf_file(char *);
void verify_conf(void);

/**
* Gets the configuration defined into a file.
* @param fname The name of the file containing the configuration.
*/
void get_conf_file(char * fname)
{
	FILE * fdesc;
	char buffer[1024];
	long l;

	if((fdesc = fopen(fname, "r")) == NULL) {
		fprintf(stderr, "WARNING: config file %s cannot be opened for read\n", fname);
		return;
	}

	while(fgets(buffer, 1024, fdesc) != NULL) {
		if((buffer[0] != '\n') && (buffer[0] != '#') && (buffer[0] != '\r')) {
			l=strlen(buffer);
			if(buffer[l - 1] == '\n') // linux EOL (\n)
				buffer[l - 1] = '\0';
			if(buffer[l - 2] == '\r') // DOS EOL (\r\n)
				buffer[l - 2] = '\0';
			get_option(buffer);
		}
	}
	fclose(fdesc);
}

/**
* Get the configuration for the simulation.
*
* First take default values for the simulation.
* Second take the values from the file 'fsin.conf'.
* Finally take the command line arguments.
* @param argn The number of arguments.
* @param args The arguments taken from the command line.
*/
void get_conf(long argn, char ** args)
{
	long i;

	set_default_conf();
	get_conf_file(DEFAULT_CONF_FILE);
	for(i = 0; i < argn; ++i)
		get_option(args[i]);
	verify_conf();
}

/**
* Gets an option & its value.
*
* Put the value to the FSIN variable.
* @param option The string which contains an option=value
*/
void get_option(char * option)
{
	int opt;
	char * name;
	char * value;
	char * param;
	char * sep=" _";
	long i;

	name = strtok(option, "=");
	if(!literal_value(options_l, name, &opt)) {
		printf("Unknown parameter - %s\n", name);
		exit(-1);
	}
	value = strtok(NULL, "=");
	switch(opt) {
	case 1:
		sscanf(value, "%ld", &r_seed);
		break;
	case 4:
		sscanf(value, "%f", &failure_rate);
		n_failures=-1; // undefines num_failures. if it is -1 we need to recalculate it from failure_rate
		break;
	case 5:
		sscanf(value, "%ld", &n_failures);
		break;
	case 6:
		param = strtok(value, sep);
		if(!literal_value(topology_l, param, (int *) &topo)) {
			printf("Error: Unknown topology - %s.  Be sure that you have spelt it correctly, and that it is defined in get_conf.c's literal_t topology_l[].\n",value);
			exit(-1);
		}
		topo_nparam=0;

		while ((param = strtok(NULL, sep)) && topo_nparam<MAX_TOPO_PARAMS)
			topo_params[topo_nparam++] = atoi(param);

		for (i=topo_nparam; i<MAX_TOPO_PARAMS; i++)
			topo_params[i] = -1; // undefine values, should not be checking these anyway.

		break;
	case 7:
		param = strtok(value, sep);
		if(!literal_value(tpatterns_l, value, (int *) &pattern)) {
			printf("Error: Unknown traffic pattern - %s\n",value);
			exit(-1);
		}
		if (!literal_name (tpatterns_l, &traffic_name, pattern)) {
			printf("Error: fetching traffic pattern name - %d\n",pattern);
			exit(-1);
		}
		traffic_nparam=0;

		while ((param = strtok(NULL, sep)) && traffic_nparam<MAX_TRAFFIC_PARAMS)
			traffic_params[traffic_nparam++] = atoi(param);

		for (i=traffic_nparam; i<MAX_TRAFFIC_PARAMS; i++)
			traffic_params[i] = -1; // undefine values, should not be checking these anyway.

		break;
	case 8:
		param = strtok(value, sep);
		if(!literal_value(routing_l, value, (int *) &routing)) {
			printf("Error: Unknown routing algorithm - %s.  Be sure that you have spelt it correctly, and that it is defined in get_conf.c's literal_t routing_l[].\n",value);
			exit(-1);
		}
		routing_nparam=0;

		while ((param = strtok(NULL, sep)) && routing_nparam<MAX_ROUTING_PARAMS)
			routing_params[routing_nparam++] = atoi(param);

		for (i=routing_nparam; i<MAX_ROUTING_PARAMS; i++)
			routing_params[i] = -1; // undefine values, should not be checking these anyway.
		break;
	case 9:
		if(!literal_value(bool_l, value, (int *) &bfs_output)) {
			printf("Error: generate_bfs should be yes/no (or true/false). value:%s\n",value);
			exit(-1);
		}
		break;
	case 10:
		if(!literal_value(bool_l,value,(int *) &compile_latex)){
			printf("Error: compile_latex should be yes/no (or true/false). value:%s\n",value);
			exit(-1);
		}
		break;
	case 11:
		param = strtok(value, sep);
		if(!literal_value(placement_l, value, (int *) &placement)) {
			printf("Error: Unknown placement strategy - %s.  Be sure that you have spelt it correctly, and that it is defined in get_conf.c's literal_t placement_l[].\n",value);
			exit(-1);
		}
		if (!literal_name (placement_l, &placement_name, placement)) {
			printf("Error: fetching placement strategy - %d\n",placement);
			exit(-1);
		}

		placement_nparam=0;

		while ((param = strtok(NULL, sep)) && placement_nparam<MAX_PLACEMENT_PARAMS)
			placement_params[placement_nparam++] = atoi(param);

		for (i=placement_nparam; i<MAX_PLACEMENT_PARAMS; i++)
			placement_params[i] = -1; // undefine values, should not be checking these anyway.
		break;
	case 12:
		sscanf(value, "%s", placement_file);
		break;
	case 13:
		param = strtok(value, sep);
		if(!literal_value(applications_l, value, (int *) &applications_type)) {
			printf("Error: Unknown option to generate applications - %s.\n",value);
			exit(-1);
		}
		if(applications_type == FILE_APP){
			param = strtok(NULL, sep);
			if(param != NULL){
				sscanf(param, "%s", applications_file);
			}
			else{
				printf("A file name must be provided.\n");
				exit(-1);
			}
		}
		break;
	case 14:
		param = strtok(value, sep);
		if(!literal_value(scheduling_l, value, (int *) &scheduling)) {
			printf("Error: Unknown scheduling policy - %s.  Be sure that you have spelt it correctly, and that it is defined in get_conf.c's literal_t scheduling_l[].\n",value);
			exit(-1);
		}
		if (!literal_name (scheduling_l, &scheduling_name, scheduling)) {
			printf("Error: fetching scheduling policy - %d\n", scheduling);
			exit(-1);
		}

		scheduling_nparam=0;

		while ((param = strtok(NULL, sep)) && scheduling_nparam < MAX_SCHEDULING_PARAMS)
			scheduling_params[scheduling_nparam++] = atoi(param);

		for (i = scheduling_nparam; i < MAX_SCHEDULING_PARAMS; i++)
			scheduling_params[i] = -1; // undefine values, should not be checking these anyway.
		break;
	case 15:
		param = strtok(value, sep);
		if(!literal_value(allocation_l, value, (int *) &allocation)) {
			printf("Error: Unknown allocation strategy - %s.  Be sure that you have spelt it correctly, and that it is defined in get_conf.c's literal_t allocation_l[].\n",value);
			exit(-1);
		}
		if (!literal_name (allocation_l, &allocation_name, allocation)) {
			printf("Error: fetching allocation - %d\n", allocation);
			exit(-1);
		}

		allocation_nparam=0;

		while ((param = strtok(NULL, sep)) && allocation_nparam < MAX_ALLOCATION_PARAMS)
			allocation_params[allocation_nparam++] = atoi(param);

		for (i = allocation_nparam; i < MAX_ALLOCATION_PARAMS; i++)
			allocation_params[i] = -1; // undefine values, should not be checking these anyway.
		break;
        case 16:
                param = strtok(value, sep);
                if(param != NULL){
                    sscanf(param, "%ld", &server_capacity);
                }
                else{
                    printf("Servers bandwidth capacity must be provided.\n");
                    exit(-1);
                }
                 param = strtok(NULL, sep);
                if(param != NULL){
                    sscanf(param, "%ld", &switch_capacity);
                }
                else{
                    printf("Switches bandwidth capacity must be provided.\n");
                    exit(-1);
                }
                break;
        case 17:
		param = strtok(value, sep);
		if(!literal_value(rt_mode_l, param, (int *) &mode)) {
			printf("Error: Unknown mode - %s.\n",value);
			exit(-1);
		}
                break;
        case 18:
		sscanf(value, "%d", &flow_inj_mode);
                break;
        case 19:
		sscanf(value, "%d", &dmetrics_time);
                break;
        case 20:
		sscanf(value, "%d", &verbose);
                break;
        case 21:
                param = strtok(value, sep);
                if(param != NULL){
                    sscanf(param, "%ld", &server_cores);
                }
                else{
                    printf("Servers bandwidth capacity must be provided.\n");
                    exit(-1);
                }
                 param = strtok(NULL, sep);
                if(param != NULL){
                    sscanf(param, "%ld", &server_memory);
                }
                else{
                    printf("Number of cores and memory of the servers must be provided.\n");
                    exit(-1);
                }
                break;

                break;
	default:
		printf("Unknown parameter - %s\n", value);
	}
}

/**
* Verifies the simulation configuration.
*
* Looks for illegal values of the variables or not allowed combinations.
*/
void verify_conf(void)
{

}

/**
* The default configuration is set here.
*/
void set_default_conf (void)
{
	long i;

	r_seed=13;
        mode=STATIC;
	topo=BCUBE;
	pattern=ALL2ALL;
	placement=SEQUENTIAL_PLC;
	failure_rate=0;
	n_failures=0;
        flow_inj_mode = 0;
        dmetrics_time = 0;
        verbose=0;
        server_cores = 1;
        server_memory = 1000;
	topo_nparam=2;
	topo_params[0]=16;
	topo_params[1]=2;
	for (i=2; i<MAX_TOPO_PARAMS; i++)
		topo_params[i]=-1;


}
