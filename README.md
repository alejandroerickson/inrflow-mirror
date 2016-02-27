# Interconnection Networks Research Flow-level (INRFlow) Extensible Framework

### License Information ###

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or (at
your option) any later version.  This program is distributed in the
hope that it will be useful, but WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the GNU General Public License for more details.  You
should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

#### Release Version ####

v0.3.0

## Contributors 

In alphabetical order:

* Alejandro Erickson
* Abbas Kiasari
* Javier Navaridas
* Jose Pascual Saiz
* Iain A. Stewart

### Notice to users ###

Please contribute any developments you make on this software back to
its original home.

## Quick Start ##

From the code/main/ directory, you can compile INRFlow using 'make
all' and then execute the binary with './build/bin/inrflow'.  Modify
inrflow.conf to change simulation parameters or override inrflow.conf
from the command line.  Example execution:

	$ ./build/bin/inrflow topo=bcube_3_3 failure_rate=0.0  tpattern=alltoall generate_bfs=false compile_latex=false

Output is written to .dat and .tex files in the directory the tool is
executed.

## Overview of INRFlow ##

INRFLow takes a network topology, routing algorithm(s), traffic
patterns, and a number of link faults as input, and then outputs
flow-level data for each link.  It is currently optimised for research
on large server-centric datacenter networks (DCNs) and relevant
traffic patterns.

INRFlow can be extended by defining new families of topologies and
their routing algorithms (e.g., src/knkstar/knkstar.c), and then
making minimal changes to the main INRFlow file (src/inrflow/main.c) to
let it know about the new topology.

See inrflow.conf for accepted input formats

Outputs are written to column separated text files, as well as stdout
 and a latex file for convenience.
 
 
### INRFlow's Outputs Explained ###

Stats and histograms are written to `.dat` files, stamped with
identifying information and the time of execution (so you are unlikely
to overwrite anything).  Both of these files are meant to be used with
pgfplots (see `pretty_plot.tex` for examples.)

The same output, stats transposed, is also sent to stdout.
Find some commented example stdout output below.

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

INRFlow is designed so that new topologies and routing algorithms can
be added.  We describe how the topology `ficonn` integrates with
INRFlow. Code for `ficonn` resides in `code/main/src/ficonn/`.  In
order for `ficonn` to interface with INRFlow we (1) assigned virtual
functions and (2) defined configuration parameters.

(1) We included `ficonn.h` in `main.c`, and assigned its
functions to virtual functions in `init_functions()`.

(2) We added `"ficonn"` to the list of allowed topologies in
`get_conf.c` at `literal_t topology_l[] = {...}`.

A topology may be implemented with more than one routing algorithm,
e.g., `dpillar`.  These options are added in `get_conf.c` at
`literal_t routing_l[] = {...}`.

Similarly, new traffic pattern tests can be added to INRFlow.  See
`traffic.h|c`, and in `get_conf.c` at `literal_t tpatterns_l[] =
{...}`.

Note that `bcube` and `fattree` are the simplest topologies and serve
as templates for implementing new topologies.
