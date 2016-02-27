typedef struct key_value {
	char key[50];
	char value[50];
	struct key_value * next;

} key_value;

void add_key_value(struct key_value ** stats_head, char* key,const char* value_format, ...);

void reverse_list(struct key_value ** stats_head);
void print_keys(struct key_value* stats_head, FILE * output);
void print_values(struct key_value* stats_head, FILE * output);
void print_key_value_pairs(struct key_value* stats_head, FILE * output);

long print_hist_array(long *H, long n,char prefix);
long print_hist_next(FILE * output, long *H, long upper, long index,double normalise);


/**
* Various utilities for collecting and computing stats
*
*/
long init_hist_array(long ** H, long n);
long update_hist_array(long hops, long ** H, long n);
long obtain_flows();
double mean_hist_array(long * H, long n);
long max_hist_array(long *H,long n);
long min_hist_array(long *H,long n);
long long size_hist_array(long *H,long n);
double median_hist_array(long * H, long n);
double var_hist_array(long *H, long n);
double std_dev_hist_array(long *H, long n);

/**
* Main statistics functions used elsewhere.
*/
void init_stats();
void update_statistics(long sh, long lh);
void capture_statistics();
long create_latex_output();
