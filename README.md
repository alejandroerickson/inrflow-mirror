# Interconnection Networks Research Flow-level (INRFlow) Extensible Framework

### License Information ###

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.  This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
details.  You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation, Inc., 51
Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

#### Release Version ####

v0.3.0

## Contributors 

In alphabetical order:

* Alejandro Erickson
* Abbas Kiasari
* Javier Navaridas
* Jose A. Pascual Saiz
* Iain A. Stewart

### Notice to users ###

Please contribute any developments you make on this software back to
its original home.

## Quick Start ##

From the code/main/ directory, you can compile INRFlow using 'make all' and then
execute the binary with './build/bin/inrflow'.  Modify inrflow.conf to change
simulation parameters or override inrflow.conf from the command line.  Example
execution:

	$ ./build/bin/inrflow topo=bcube_3_3 failure_rate=0.0  tpattern=alltoall generate_bfs=false compile_latex=false

The default configuration can be found in `get_conf.c`, at `set_default_conf()`.
Output is written to .dat and .tex files in the directory the tool is executed,
as well as to stdout.

## Overview of INRFlow ##

INRFLow takes a network topology, routing algorithm(s), traffic
patterns, and a number of link faults as input, and then outputs
flow-level data for each link.  It is currently optimised for research
on large server-centric datacenter networks (DCNs) and relevant
traffic patterns.

INRFlow can be extended by defining new families of topologies and their routing
algorithms (e.g., src/knkstar/knkstar.c), and then making minimal changes to the
main INRFlow file (src/inrflow/main.c) to let it know about the new topology.

See inrflow.conf for examples of accepted input formats, as well as the source
code for the topology of you are interested in.

Outputs are written to column separated text files, as well as stdout
 and a latex file for convenience.

## Release Version Information ##

  * V0.3.0 First release!
  * V0.4.0 
	* Dynamic engine added.
	* INRFlow refactored
	* Optional stats and histograms, topology-side
	* New topologies HCN BCN (hcnbcn) and Generalized DCell and FiConn (gdcficonn)
	* New features in Jellyfish topology
	* Shift and Bisection traffic patterns
	* Time measuring infrastructure
	
### INRFlow's Outputs Explained ###

Stats and histograms are written to `.dat` files, stamped with identifying
information and the time of execution (so you are unlikely to overwrite
anything).  Both of these files are meant to be used with pgfplots (see
`pretty_plot.tex` for examples.)

The same output, stats transposed, is also sent to stdout.  Find some commented
example stdout output below.

	network                  
	routing.algorithm        
	traffic.pattern          
	r.seed                 //random seed
	tf.version             //version of inrflow framework
	topo.version           //version of the current topology
	p.cable.failures       //proportion of bidirectional link failures
	                       //(there are no unidirectional link failures)
	n.cable.failures       //number of bidirectional link failures
	n.servers              //number of servers
	n.switches             //number of switches
	switch.radix           //switch radix (non-regular switch degrees to be supported in future)
	n.cables               //number of bidirectional links (failed or otherwise)
	n.server.pairs.NN      //number of pairs of servers
	n.server.pairs.NN-1    //number of pairs of DISTINCT servers
	n.routes.connected     //number of routes succesfully completed (including self-sent)
	p.routes.connected.NN  //proportion of routes succesfully completed (including self-sent)
	min.link.path.length   //stats on path length where EVERY link is counted
	max.link.path.length             
	mean.link.path.length     
	median.link.path.length   
	var.link.path.length      
	std.link.path.length      
	min.server.hop.length   //stats on hop length of paths, where only SERVER HOPS are counted 
	max.server.hop.length            
	mean.server.hop.length    
	median.server.hop.length  
	var.server.hop.length     
	std.server.hop.length     
	//The following items are currently only reported for fault-free networks
	//and are computed from the flow histogram
	n.links                 //number of fault-free links
	min.flows.per.link               
	max.flows.per.link              
	mean.flows.per.link      
	median.flows.per.link    
	var.flows.per.link       
	std.flows.per.link                   
	connected.nonzero.flows.over.bottleneck.flow  //aggregate bottleneck throughput (ABT) with no self-sent flows: N\*(N-1)/max.flows.per.link
	connected.flows.over.bottleneck.flow          //ABT including self-sent flows: N*N/max.flows.per.link
	connected.nonzero.flows.over.mean.flow        //As above, computed with mean.flows.per.link
	connected.flows.over.mean.flow               
	runtime                                       //number of seconds the whole process took
	started.at.y.m.d.h:m                          //the time the process was started
	n                                             //parameters relating to the topology (there may be others).
	k                                   

### Extending INRFlow ###

#### Adding a new topology ####

INRFlow is designed so that new topologies and routing algorithms can
be added.  We describe how the topology `ficonn` integrates with
INRFlow. Code for `ficonn` resides in `code/main/src/ficonn/`.  In
order for `ficonn` to interface with INRFlow we (1) assigned virtual
functions and (2) defined configuration parameters.

To begin, we copy the **template topology header**, `src/inrflow/topo.h`, to
`src/ficonn/ficonn.h` and implement the functions declared there, renamed as
explained below.  We also remove any optional declarations we will not use (like
extra stat reporting or routing parameters).  Any other functions in ficonn.c
should not be declared in `ficonn.h` and, to be safe, it's best to declare them
as `static` which will avoid any accidental namespace clashes.

(1) We included `ficonn.h` in `main.c`, and assigned its
functions to virtual functions in `init_functions()`.

(2) We added `"ficonn"` to the list of allowed topologies in
`get_conf.c` at `literal_t topology_l[] = {...}`.  We added the
constant `FICONN` to `typedef enum topo_t {...}` in `misc.h`.

A topology may be implemented with more than one routing algorithm,
e.g., `dpillar`.  These options are added in `get_conf.c` at
`literal_t routing_l[] = {...}`.

Similarly, new traffic pattern tests can be added to INRFlow. See `traffic.h|c`,
and in `get_conf.c` at `literal_t tpatterns_l[] = {...}` (see below).

Note that `bcube` and `fattree` are the simplest topologies and serve as
templates for implementing new topologies, but comprehensive documentation of
the required functions is provided in the template header file `topo.h`.

##### Topologies with optional stats and histograms #####

Topologies from INRFlow V0.4.x onward can report their own statistics and
histograms to INRflow after the experiment/simulation has ended, which are then
integrated into the normal output.  Examples are hcnbcn and gdcficonn.  These
functions (below) are a bit delicate, so we have documented them further here.

	// [OPTIONAL] Functions to report custom status from the topology. See, e.g.,  gdcficonn
	long (*get_topo_nstats)();///< number of stats to be reported.
	struct key_value (*get_topo_key_value)(long i);///< ith key value to be used in reporting.c
	long (*get_topo_nhists)();///< number of histograms to be reported
	char (*get_topo_hist_prefix)(long i);///< ith histogram prefix, must avoid 'h' 'p' 'f'
	const char* (*get_topo_hist_doc)(long i);///< ith histogram documentation, latex friendly and not contain '\n'
	long (*get_topo_hist_max)(long i);///< ith histogram length
	void (*get_topo_hist)(long *topo_hist, long i);///< ith histogram (memcpy to topo_hist)

A histogram `H` records the frequency of occurrence of the value at it's index.
That is, `i` happens `H[i]` times.  INRFlow needs:
  * an upper bound on `i` in order to initialise `H`.
  * a doc-string for `H` that does not contain `\n`.  This
    doc-string will appear with `H` in the output.  No limit on length
  * a unique "prefix" character for each histogram, that is different from the
    ones used in INRFlow, namely, `'h'`, `'p'`, and `'f'`.

Histograms are updated using the function `update_hist_array()`, provided by
`reporting.c`.  Typically we want to glean stats from the histograms, and these
are also provided by `reporting.c`: `X_hist_array()`, where `X` is
`min|max|mean|median|var|std`.  Of course, stats can be generated any which way,
but they are reported using a function from `reporting.c`.  Namely
`make_key_value_node()`.  Return the output of this function in
`get_topo_key_value_yourtopo()`.

#### Adding a new traffic pattern ####

Traffic patterns are declared/defined in `traffic.h`/`traffic.c` with an "init"
function and a "next" function. The init function interprets parameters,
precomputes values like the total number of flows that will be generated, and
makes other preparations such as memory allocation. Each flow is generated based
on the previous flow that was generated.

Note that INRFlow provides the freedom to generate flows any which way, such as
pre-populating an array of flows with an arbitrary combinatorial generation
algorithm. Assuming that some care is taken to avoid a time-consuming algorithm,
the developer should note that the memory requirements for storing all flows
might be the bottleneck. INRFlow does not currently do this in the static
engine, where the number of flows might potentially be very large.

Traffic patterns must be added to `get_conf.c` in order that they, and their
parameters, be understood by INRFlow. Add all possible spellings to `literal_t
tpatterns_l[]`. Finally, the virtual functions `init_pattern()` and `next_flow()`
must assigned to the corresponding functions in `traffic.c`; this is done in
`init_functions()` of `main.c`.

### Testing INRFlow ###

The scripts ./gnu_parallel_test.sh, ./parallel_test.sh, ./serial_test.sh will
execute `inrflow` on the arguments generated by ./gen_test_args.sh, and should be
used in that order of preference.


### Dynamic execution mode ###

Since v0.4.0 a dynamic execution mode has been added. The dynamic engine allows INRFlow to 
simulate real scenarios in which multiple applications share the network resources. It also 
simulates the scheduling process allowing the arrival of new applications when resources freed
by other applications become available.

    - Dynamic engine: This mode considers the bandwidth of the links and the size of the flows 
                      exchanged by the applications. Using this mode, instead of the static, INRFlow
                      is able to estimate the time required to send and receive all the flows 
                      generated by multiple applications executed concurrently.  

    - Scheduling: It is the process that selects the order in which applications will be 
                  executed following a scheduling policy. It is represented as a list of 
                  applications described with multiple parameters.

    ## Example ## 

    inrflow mode=dynamic injmode=x capacity=c1_c2 scheduling=y workload=file_z 

    - injmode (Injection mode): Number of simultaneous flows that a task can inject into the network.
                                If we set this value to 0, all the flows available will be injected.
                                Otherwise, only the number of flows set by this value can be injected.

    - capacity: Speed of the links. c1 corresponds to server links and c2 to switch links.
    
    - scheduling: The policy to be used to establish the order in which applications will be executed. 
                  For now, only the FCFS (Fist Come First Serve) policy is implemented.

    - workload: It is the set of applications to be executed. Using the file option INRFlow will load 
                the list of applications from the file z. For now this is the only way to load the list of 
                applications. The format of the file is as follows:

                * 0 all2all_1 64 sequential
                * 10 manyall2allrnd_1_50 10000 random

                * timestamp application_parameters size allocation
                
                where:
                     - timespamp is the time when the application arrives.
                     - application is the pattern or application to be executed.
                     - parameters are specific to each pattern or application
                     - size is the number of tasks that compose the applications
                     - allocation is the strategy to be used to put the tasks onto the network nodes.
                       For now only the sequential and random allocations is implemented.

### Extending scheduling, allocation and applications in dynamic mode ###

    - The addition of new scheduling policies and allocation strategies must be done in the files get_conf.c where new policies
      and stragies are added and in scheduling.c and allocation.c where the source is included. For detailed information follow the 
      iplemententation of the already included FCFS scheduling policy and sequential allocation strategy. These strategies are topology agnostic. If we want to add specific policies or strategies the procedure is the same but the recommendation is to put the implementation inside the topology directory as can be seen in jellyfish with the spread allocation strategy.

    - The inclusion of new applications (patterns) must be performed in the files get_conf.c and gen_trace.c. The former is used  
      to read the application (pattern) from the configuration file, the latter contains the code to select the  application (pattern). 
      The actual code to generate the flows of the application is located in the kernels directory. More details can be 
      found in the file kernels/collectives.c where several applications (patterns) are already implemented.

### Output of the dynamic mode ###

The output of the dynamic mode is organized in four files.

    - .scheduling: Metrics related to the scheduling.
        + Makespan: Time required to process all the applications. 

    - .execution: Metrics related to the execution of all the application in the workload.
        + Runtime: Time required to execute all the application. 
        + Average latency: Average latency of the flows sent by all the applications.
        + Aggregated bandwidth: Bandwitdth used by the flows. 
        + Number of links shared: This metric measures how many links are used by 0 applications (not used), by 1 application, by 2 applications and so on.
  
    - .applications: Metrics related to the applications.
        + Number of the application: The number of applications executed.

    - .list_applications: Contains the metrics for each application (not averaged as in .applications). 
        + Id: Id of the application.          
        + Arrive time: Time when the application arrived.           
        + Start time: Time when the applicaton started.            
        + Finish time: Time when the application finished.
        + Runtime: Execution time of the application.    
        + Waiting time: Time that the application waited in the queue.   
        + Number of flows: Number of flows exchanged among the tasks of the applications.
        + Total distance: Total distance traveled by the flows.       
        + Average flows distance: Average distance traveled by the flows.
        + Total flows latency: Total latency of all the flows.  
        + Average flows latency: Average latency of the flows.

There is also a verbose mode that will show information about the amount of flows that are in the network. 
It can be activated using the option verbose=1. The option metricsint determines when this information 
is shown. It is recommended to use the value 0 that selects the interval automatically.

    * inrflow mode=dynamic verbose=1 metricsint=0  
     

















    
