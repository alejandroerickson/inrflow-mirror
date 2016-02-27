#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "reporting.h"
#include "misc.h"
#include "globals.h"

#define BROKEN_FLOWS_NOT_DELETED
long *server_hop_histogram;
long *path_length_histogram;
long *flows_histogram;

static time_t start_time;	///< Simulation start time / date.
static time_t end_time;		///< Simulation finish time / date.

char time_started[26];
struct tm* tm_info;

FILE *stats_file, *hist_file;
char stats_filename[100], hist_filename[100];
char filename_params[100];

char * inrflow_version="v0.3.0";

void add_key_value(struct key_value ** stats_head, char* key,const char* value_format, ...)
{
	char value[50];
	va_list arg;
	va_start(arg,value_format);
	vsprintf(value,value_format,arg);
	va_end(arg);

	if(stats_head==NULL) {
		*stats_head=malloc(sizeof(struct key_value));
		strcpy((*stats_head)->key,key);
		strcpy((*stats_head)->value,value);
	} else {
		struct key_value* new_node;
		new_node=malloc(sizeof(struct key_value));
		strcpy(new_node->key,key);
		strcpy(new_node->value,value);
		new_node->next=*stats_head;
		*stats_head=new_node;
	}
}

void reverse_list(struct key_value ** stats_head)
{
	struct key_value* curr_node=NULL, *prev_node=NULL;
	curr_node=*stats_head;
	while(curr_node!=NULL) {
		curr_node=curr_node->next;
		(*stats_head)->next=prev_node;
		prev_node=*stats_head;
		*stats_head=curr_node;
	}
	*stats_head=prev_node;
}

void print_keys(struct key_value* stats_head, FILE * output)
{
	struct key_value* curr_node;
	curr_node=stats_head;
	while(curr_node!=NULL) {
		fprintf(output,"%s ",curr_node->key);
		curr_node=curr_node->next;
	}
	fprintf(output,"\n");
}

void print_values(struct key_value* stats_head, FILE * output)
{
	struct key_value* curr_node;
	curr_node=stats_head;
	while(curr_node!=NULL) {
		fprintf(output,"%s ",curr_node->value);
		curr_node=curr_node->next;
	}
	fprintf(output,"\n");
}

void print_key_value_pairs(struct key_value* stats_head, FILE * output)
{
	struct key_value* curr_node;
	curr_node=stats_head;
	while(curr_node!=NULL) {
		fprintf(output,"%-35s ",curr_node->key);
		fprintf(output,"%-20s\n",curr_node->value);
		curr_node=curr_node->next;
	}
}

long print_hist_array(long *H, long n,char prefix)
{
	long i;
	for (i=0; i<n; i++) {
		if(H[i]>0)
			printf("%c %4ld %9ld\n",prefix,i,H[i]);
	}
	return 1;
}

long print_hist_next(FILE * output, long *H, long upper, long index, double normalise)
{
	while(index<upper && H[index]==0)index++;
	if(index<upper)
		fprintf(output,"%9ld %9ld %1.5f ",index,H[index],H[index]/normalise);
	else
		fprintf(output,"%9s %9s %7s ","nan", "nan", "nan");
	return ++index;
}


/**
* Various utilities for collecting and computing stats
*
*/

long init_hist_array(long ** H, long n)
{
	long i;
	*H = malloc(n*(sizeof(long)));
	for (i=0; i<n; i++) {
		(*H)[i] = 0;
	}
	return 1;

}

long update_hist_array(long hops, long ** H, long n)
{
	if(hops>=n) {
		printf("ERROR:  Found a link path (or server hops) of length %ld, when upper bound was thought to be %ld", hops, n);
		exit(-1);
	}
	(*H)[hops]++;
	return 1;
}


long obtain_flows()
{
	long i,j;
	for (i=0; i<servers+switches; i++) {
		for (j=0; j<network[i].nports; j++) {
			//ensure that faulty and open links are not counted,
			//since, we do not want to skew things by these
			//0-flows (or worse, garbage-flows).
			if(!network[i].port[j].faulty &&
			        !(network[i].port[j].neighbour.node==-1))
				flows_histogram[network[i].port[j].flows]++;
		}
	}
	return 1;
}

double mean_hist_array(long * H, long n)
{
	long i,m,total;
	m=0;
	total=0;
	for (i=0; i<n; i++) {
		m+=H[i]*i;
		total+=H[i];
	}
	return ((double)m)/((double)total);
}

long max_hist_array(long *H,long n)
{
	long i;
	for (i=n-1; i>=0; i--) {
		if(H[i]>0)
			return i;
	}
	return 0;
}

long min_hist_array(long *H,long n)
{
	long i;
	for (i=0; i<n; i++) {
		if(H[i]>0)
			return i;
	}
	return 0;
}

long long size_hist_array(long *H,long n)
{
	long i;
	long long total;
	total=0;
	for (i=0; i<n; i++) {
		total+=(long long)H[i];
	}
	return total;
}

//This is the mean of the list: 0{repeated H[0] times}, 1{repeated H[1] times}, etc
//H[0]=2
//H[3]=2
//median is 1.5, but the old function would have output 0

//H[0]=1
//H[1]=1
//H[2]=1
//median is 1 but old function would have output 0
// AE: I think I fixed the bias here
double median_hist_array(long * H, long n)
{
	long i,cum,j;
	long long N2=size_hist_array(H,n)/2;//floor of half the number of things recorded in histogram
	cum=0;
	for (i=0; i<n; i++) {
		cum+=H[i];
		if(cum==N2) { //floor of half the number of things in histogram
			//are in H[0] to H[i].
			//find the next non-zero index j
			j=i+1;
			while(H[j]==0 && j<(n-1)) j++;
			if(N2%2==0) //even number of things, return average of these two values.
				return (double)(i+j)/2;
			else //odd number of things, return j
				return (double)j;
		} else if(cum>N2)
			return (double)i;
	}
	return 0;
}

//variance
double var_hist_array(long *H, long n)
{
	long i,total;
	long long sum_of_squares, sum_squared;
	total = size_hist_array(H,n);
	sum_of_squares=0;
	sum_squared=0;
	for (i=0; i<n; i++) {
		sum_of_squares+=H[i]*i*i;
		sum_squared+=H[i]*i;
	}
	sum_squared *=sum_squared;
	return (double)((long double)sum_of_squares-(long double)sum_squared/total)/(total-1);
}

double std_dev_hist_array(long *H, long n)
{
	return (double)sqrt(var_hist_array(H,n));
}


void init_stats()
{
	time(&start_time);
	tm_info = localtime(&start_time);
	strftime(time_started, 26, "%Y.%m.%d.%H:%M", tm_info);

	init_hist_array(&path_length_histogram,UPPER_PATH_LENGTH);
	init_hist_array(&server_hop_histogram,UPPER_SERVER_HOPS);
}

/**
* Updates path length statistics after a flow.
* @param sh path length in terms of server hops
* @param lh path length in terms of link hops
*/
void update_statistics(long sh, long lh)
{
	update_hist_array(sh, &server_hop_histogram, UPPER_SERVER_HOPS);
	update_hist_array(lh, &path_length_histogram, UPPER_PATH_LENGTH);
	link_hops+=lh;
	server_hops+=sh;
}

#ifdef DEBUG
/**
 * Checks consistency between various statistics.
 *
 */
void stats_consistency_check()
{
	if(n_failures==0 &&
	        ports!=size_hist_array(flows_histogram,upper_flows)) {
		printf("Flow histogram size and number of ports are inconsistent (%ld vs %ld).\n",size_hist_array(flows_histogram,upper_flows),ports);
//   exit(-1);
	}

	if(n_failures==0 && pattern == ALL2ALL &&
	        connected != ((long long)servers)*((long long)servers)) {
		printf("FAIL CONSISTENCY CHECK: n_failures==0"
		       " && pattern == ALL2ALL && connected != "
		       "((long long)servers)*((long long)servers.\n");
//   exit(-1);
	}

	if(n_failures==0 &&
	        ((long)(1000*mean_hist_array(flows_histogram, upper_flows))) !=
	        ((long)((1000*connected *
	                 mean_hist_array(path_length_histogram, UPPER_PATH_LENGTH) /
	                 ports)))) {
		printf("Mean number of flows per link computed with "
		       "n.connected x mean.path.length / ports "
		       "inconsistent with the flow histogram, to three decimal places.\n"
		       "Reported:\n%f\n%f\n",
		       connected *
		       mean_hist_array(path_length_histogram, UPPER_PATH_LENGTH) /
		       ports,
		       mean_hist_array(flows_histogram, upper_flows));
//   exit(-1);
	}
}
#endif

void capture_statistics()
{
	long i, j;
	double runtime;


	for(i=0; i<servers+switches; i++)
		for(j=0; j<network[i].nports; j++)
			if(network[i].port[j].flows>bottleneck_flow)
				bottleneck_flow=network[i].port[j].flows;

	upper_flows = bottleneck_flow+1;
	init_hist_array(&flows_histogram,upper_flows);
	obtain_flows();

	//end of experiment.  only printing to do
	time(&end_time);
	runtime=difftime(end_time,start_time);

	sprintf(stats_filename,"%s_%s_%s_fr%.2f_seed%ld.%s.dat",
	        get_network_token(),
	        get_filename_params(),
	        get_routing_token(),
	        failure_rate,
	        r_seed,
	        time_started);
	stats_file = fopen(stats_filename,"w");
	if(stats_file == NULL) {
		perror("Unable to open stats file");
		exit(-1);
	}
	printf("Wrote stats to file: %s\n",stats_filename);

	long max_flow=max_hist_array(flows_histogram,upper_flows);
	long long server_pairs, non_zero_path_server_pairs;
	server_pairs=((long long)servers)*((long long)servers);
	non_zero_path_server_pairs = ((long long)servers)*((long long)(servers-1));

	struct key_value *stats_head=NULL;

	add_key_value(&stats_head,
	              "network","%s",
	              get_network_token() );  //single-token name
	add_key_value(&stats_head,
	              "routing.algorithm","%s",
	              get_routing_token()  );  //single-token name
	add_key_value(&stats_head,
	              "traffic.pattern","%s",
	              traffic_name );  //traffic pattern name
	add_key_value(&stats_head,
	              "r.seed","%ld",
	              r_seed );  //random seed
	add_key_value(&stats_head,
	              "inrflow.version","%s",
	              inrflow_version );  //version of the inrflow code
	add_key_value(&stats_head,
	              "topo.version","%s",
	              get_topo_version() );  //version of the topology code
	add_key_value(&stats_head,
	              "p.cable.failures","%f",
	              failure_rate );  // proportion of failures
	add_key_value(&stats_head,
	              "n.cable.failures","%ld",
	              n_failures );  // proportion of failures
	add_key_value(&stats_head,
	              "n.servers","%ld",
	              servers );  // number of servers
	add_key_value(&stats_head,
	              "n.switches","%ld",
	              switches );  // number of switches
	add_key_value(&stats_head,
	              "switch.radix","%ld",
	              radix );  // radix of switches
	add_key_value(&stats_head,
	              "n.cables","%ld",
	              ports/2); // number of bidirectional links.


	add_key_value(&stats_head,
	              "n.server.pairs.NN","%lld",
	              server_pairs);  //servers*servers
	add_key_value(&stats_head,
	              "n.server.pairs.NN-1","%lld",
	              non_zero_path_server_pairs );   //servers*(servers-1)
	add_key_value(&stats_head,
	              "n.routes.connected","%lld",
	              connected );  //global variable: connected
	add_key_value(&stats_head,
	              "p.routes.connected.NN","%f",
	              (double)(100.0L*connected/server_pairs) );
	//"proportion.connected.to.N.x.N-1" // we don't report
	// this because connected counts trivial paths
	add_key_value(&stats_head,
	              "min.link.path.length","%ld",
	              min_hist_array(path_length_histogram,
	                             UPPER_PATH_LENGTH) );
	add_key_value(&stats_head,
	              "max.link.path.length","%ld",
	              max_hist_array(path_length_histogram,
	                             UPPER_PATH_LENGTH) );
	add_key_value(&stats_head,
	              "mean.link.path.length","%f",
	              mean_hist_array(path_length_histogram,
	                              UPPER_PATH_LENGTH) );
	add_key_value(&stats_head,
	              "median.link.path.length","%f",
	              median_hist_array(path_length_histogram,
	                                UPPER_PATH_LENGTH) );
	//"mode.link.path.length" //we don't report this because
	// it's not meaningful as a single value
	add_key_value(&stats_head,
	              "var.link.path.length","%f",
	              var_hist_array(path_length_histogram,
	                             UPPER_PATH_LENGTH) );
	add_key_value(&stats_head,
	              "std.link.path.length","%f",
	              std_dev_hist_array(path_length_histogram,
	                                 UPPER_PATH_LENGTH) );

	add_key_value(&stats_head,
	              "min.server.hop.length","%ld",
	              min_hist_array(server_hop_histogram,
	                             UPPER_SERVER_HOPS) );
	add_key_value(&stats_head,
	              "max.server.hop.length","%ld",
	              max_hist_array(server_hop_histogram,
	                             UPPER_SERVER_HOPS) );
	add_key_value(&stats_head,
	              "mean.server.hop.length","%f",
	              mean_hist_array(server_hop_histogram,
	                              UPPER_SERVER_HOPS) );
	add_key_value(&stats_head,
	              "median.server.hop.length","%f",
	              median_hist_array(server_hop_histogram,
	                                UPPER_SERVER_HOPS) );
	//"mode.server.hop.length" //we don't report this because
	// it's not meaningful as a single value
	add_key_value(&stats_head,
	              "var.server.hop.length","%f",
	              var_hist_array(server_hop_histogram,
	                             UPPER_SERVER_HOPS) );
	add_key_value(&stats_head,
	              "std.server.hop.length","%f",
	              std_dev_hist_array(server_hop_histogram,
	                                 UPPER_SERVER_HOPS) );


	//stopgap measure to avoid reporting these stats until flow
	//recording is implemented for faulty networks
#ifdef BROKEN_FLOWS_NOT_DELETED
	if(n_failures==0) {
#endif

		add_key_value(&stats_head,
		              "n.links","%lld",
		              size_hist_array(flows_histogram,
		                              upper_flows) );
		add_key_value(&stats_head,
		              "min.flows.per.link","%ld",
		              min_hist_array(flows_histogram,
		                             upper_flows) );
		add_key_value(&stats_head,
		              "max.flows.per.link","%ld",

		              max_flow );
		add_key_value(&stats_head,
		              "mean.flows.per.link","%f",
		              mean_hist_array(flows_histogram,
		                              upper_flows) );
		add_key_value(&stats_head,
		              "median.flows.per.link","%f",
		              median_hist_array(flows_histogram,
		                                upper_flows) );
		//"mode.flows.per.link"//we don't report this because it's not
		// meaningful as a single value
		add_key_value(&stats_head,
		              "var.flows.per.link","%f",
		              var_hist_array(flows_histogram,
		                             upper_flows) );
		add_key_value(&stats_head,
		              "std.flows.per.link","%f",
		              std_dev_hist_array(flows_histogram,
		                                 upper_flows) );
		add_key_value(&stats_head,
		              "connected.nonzero.flows.over.bottleneck.flow","%f",
		              (double)non_zero_path_server_pairs/max_flow );
		add_key_value(&stats_head,
		              "connected.flows.over.bottleneck.flow","%f",
		              (double)server_pairs/max_flow );
		add_key_value(&stats_head,
		              "connected.nonzero.flows.over.mean.flow","%f",
		              (double)non_zero_path_server_pairs /
		              mean_hist_array(flows_histogram,upper_flows));
		add_key_value(&stats_head,
		              "connected.flows.over.mean.flow","%f",
		              (double)server_pairs /
		              mean_hist_array(flows_histogram,upper_flows));
#ifdef BROKEN_FLOWS_NOT_DELETED
	}
#endif

	add_key_value(&stats_head,
	              "runtime","%f",
	              runtime);
	add_key_value(&stats_head,
	              "started.at.y.m.d.h:m","%s",
	              time_started);
	for(i=0; i<topo_nparam; i++) {
		add_key_value(
		    &stats_head,get_topo_param_tokens(i),"%ld",
		    topo_params[i]);
	}

	reverse_list(&stats_head);
	print_keys(stats_head,stats_file);
	print_values(stats_head,stats_file);
	fclose(stats_file);
	print_key_value_pairs(stats_head,stdout);


	//AE: we will send the histograms to another file
	//now we print the three histograms, for flow length, link-path length, and server-hop length

	printf("#Server hop length histogram (server hop path length, number of occurrences)\n");
	print_hist_array(server_hop_histogram,UPPER_SERVER_HOPS,'h');
	printf("#Path length histogram (path length, number of occurrences)\n");
	print_hist_array(path_length_histogram,UPPER_PATH_LENGTH,'p');

	printf("#Flows histogram (number of flows, number of uni-directional links with this many flows)\n");
	print_hist_array(flows_histogram,upper_flows,'f');


	sprintf(hist_filename,"%s_%s_%s_fr%.2f_seed%ld_histograms.%s.dat",
	        get_network_token(),
	        get_filename_params(),
	        get_routing_token(),
	        failure_rate,
	        r_seed,
	        time_started);
	hist_file = fopen(hist_filename,"w");
	if(hist_file == NULL) {
		perror("Unable to open stats file");
		exit(-1);
	}

#ifdef BROKEN_FLOWS_NOT_DELETED
	if(n_failures>0) {
#endif
		fprintf(hist_file, "#WARNING:  Flow-histogram is invalid for n.failures>0 (i.e., this output) in the current version of INRFlow\n");
#ifdef BROKEN_FLOWS_NOT_DELETED
	}
#endif

	fprintf(hist_file,
	        "#Histograms for %s.\n"
	        "#hi,pi,fi are indices for h, p and f, respectively. *n is normalized so that the bars add up to 1.\n"
	        "#h: Server hop length histogram (server hop path length, number of occurrences)\n"
	        "#p: Path length histogram (path length, number of occurrences)\n"
	        "#f: Flows histogram (number of flows, number of uni-directional links with this many flows)\n",
	        //	  "#If needed, omit the length 0 paths to avoid plotting the case where src=dst\n",
	        hist_filename);

	double num_uni_dir_links = (double)size_hist_array(flows_histogram,upper_flows);
	fprintf(hist_file,"%9s %9s %6s %9s %9s %6s %9s %9s %6s\n","hi", "h", "nh" ,"pi" ,"p", "np" ,"fi" ,"f", "nf");
	long hi=0,pi=0,fi=0;
	while(hi<UPPER_SERVER_HOPS || pi < UPPER_PATH_LENGTH || fi < upper_flows) {
		hi=print_hist_next(hist_file,server_hop_histogram,UPPER_SERVER_HOPS,hi,(double)connected);
		pi=print_hist_next(hist_file,path_length_histogram,UPPER_PATH_LENGTH,pi,(double)connected);
		fi=print_hist_next(hist_file,flows_histogram,upper_flows,fi,num_uni_dir_links);
		fprintf(hist_file,"\n");
	}
	fclose(hist_file);

	runtime=difftime(end_time,start_time);
	printf("time %f\n",runtime); // SAme as above

#ifdef DEBUG
	stats_consistency_check();
#endif
}

long create_latex_output()
{
	long exit_code=0;
	printf("Add the file \"pretty_data.tex\" to the output directory and compile it with the following command.\npdflatex  \"\\newcommand\\thetables{\\pgfplotstabletranspose\\statstable{%s} \\pgfplotstableread{%s}\\histogramtable}\\input{pretty_data.tex}\"\n",stats_filename,hist_filename);

#ifndef WIN32
	//AE: dropped .dat from tex_filename
	char tex_filename[100];
	//  char tex_filename_prefix[100];
	char sys_command[1000];
	//AE: Some stuff here is probably OS specific.

	if(compile_latex) {
		sprintf(tex_filename,"%s_%s_%s_fr%.2f_seed%ld.%s",
		        get_network_token(),
		        get_filename_params(),
		        get_routing_token(),
		        failure_rate,
		        r_seed,
		        time_started);
		printf("Attempting to compile a pdf file.\n");
		sprintf(sys_command,"cp pretty_data.tex %s.tex",tex_filename);
		exit_code = system(sys_command);
		sprintf(sys_command,"pdflatex -interaction nonstopmode \"\\newcommand\\thetables{\\pgfplotstabletranspose\\statstable{%s} \\pgfplotstableread{%s}\\histogramtable}\\input{%s.tex}\" > /dev/null 2>&1",stats_filename,hist_filename,tex_filename);
		exit_code = system(sys_command);
		//  printf("exit code of pdflatex %d\n",exit_code);
		if(exit_code==0) {
			printf("pdflatex compiled succesfully.  Find the output in %s.pdf\n",tex_filename);
			exit_code = system("latexmk -c > /dev/null  2>&1");
			//  printf("exit code of latexmk clean %d\n",exit_code);
			sprintf(sys_command, "rm %s.tex",tex_filename);
			exit_code = system(sys_command);
		} else {
			printf("pdflatex failed to compile automatically.\n");
			//  You'll have to do it manually\n");

			/****************/
			printf("Attempting to compile a pdf file without "
			       "the histograms (as the table might be too large).\n");
			sprintf(sys_command,"cp stats_only.tex %s.stats.tex",tex_filename);
			exit_code = system(sys_command);
			sprintf(sys_command,"pdflatex -interaction nonstopmode "
			        "\"\\newcommand\\thetables{"
			        "\\pgfplotstabletranspose\\statstable{%s}}"
			        "\\input{%s.stats.tex}\" > /dev/null 2>&1",stats_filename,tex_filename);
			exit_code = system(sys_command);
			//  printf("exit code of pdflatex %d\n",exit_code);
			if(exit_code==0) {
				printf("pdflatex compiled succesfully."
				       "  Find the output in %s.stats.pdf "
				       "(no histograms)\n",tex_filename);
				exit_code = system("latexmk -c > /dev/null 2>&1 ");
				exit_code = system(sys_command);
				sprintf(sys_command, "rm %s.stats.tex",tex_filename);
				exit_code = system(sys_command);
			} else {
				printf("pdflatex failed to compile automatically.  You'll have to do it manually\n");
			}

			/******************/
		}
	}
#endif
	return exit_code;
}

/*
void print_network_structure(node_t* network, long servers, long switches){

    long i, j, k;
    FILE *net_struct_file;

    if (out_net_struc == 1){
        net_struct_file = fopen(net_struct_filename,"w");
	    for(i = 0;i < servers + switches; i++){
		    for(j = 0;j < servers + switches; j++){
			    if(i == j){
				    fprintf(net_struct_file, "0 ");
			    }
			    else{
				    for(k = 0; k < network[i].nports; k++){
					    if(network[i].port[k].neighbour.node == j){
						    fprintf(net_struct_file, "1 ");
						    break;
					    }
					    if(k == network[i].nports - 1)
						    fprintf(net_struct_file, "0 ");
				    }
			    }
  		    }
		    fprintf(net_struct_file,"\n");
	    }
        fclose(net_struct_file);
    }
}

*/
