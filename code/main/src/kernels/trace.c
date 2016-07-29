#include "../inrflow/globals.h"
#include "../inrflow/applications.h"
#include "../inrflow/gen_trace.h"
#include "trace.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
* The trace reader dispatcher selects the format type and calls to the correct trace read.
*
* The selection reads the first character in the file. This could be: '#' for dimemas, 
* 'c', 's' or 'r' for fsin trc, and '-' for alog (in complete trace the header is "-1",
* or in filtered trace could be "-101" / "-102"). This is a very naive decision, so we
* probably have to change this, but for the moment it works.
* 
*@see read_dimemas
*@see read_fsin_trc
*@see read_alog
*/
void read_trc(application *app, list_t **task_events, long size) {
	FILE * ftrc;
	char c;

	if((ftrc = fopen(app->pattern_file, "r")) == NULL){
		printf("Trace file not found in current directory");
                exit(-1);
	}
	c=(char)fgetc(ftrc);
	fclose(ftrc);

	switch (c){
		case '-':
			read_alog_trc(app, task_events, size);
			break;
		case 'c':
		case 's':
		case 'r':
			read_fsin_trc(app, task_events, size);
			break;
		case -1:
			printf("Reading empty file\n");
			break;
		default:
			printf("Cannot understand this trace format");
                        exit(-1);
			break;
	}
}

/**
 * Reads a trace from a file.
 *
 * Read a trace from a fsin trc file whose name is in global variable #trcfile
 * This format only takes in account 'c' CPU, 's' SEND, 'r' RECV, events.
 */
void read_fsin_trc(application *app, list_t **task_events, long size) {

    FILE * ftrc;
    char buffer[512];
    char * tok;
    char sep[]=" \t";
    long to, from, tag, length;

    if((ftrc = fopen(app->pattern_file, "r")) == NULL){
        printf("Trace file not found in current directory\n");
        exit(-1);
    }
    while(fgets(buffer, 512, ftrc) != NULL) {
        if(buffer[0] != '\n' && buffer[0] != '#') {
            if(buffer[strlen(buffer) - 1] == '\n')
                buffer[strlen(buffer) - 1] = '\0';

            tok = strtok( buffer, sep);

            if (strcmp(tok, "s") == 0) { // Communication.
                tok = strtok(NULL, sep); // from
                from = atol(tok); // Node to add event
                tok=strtok(NULL, sep);
                to = atol(tok); // event's PID: destiny when we are sending
                if (to != from) {
                    tok = strtok(NULL, sep);
                    tag = atol(tok); // Type of message (tag)
                    tok = strtok(NULL, sep);
                    length = atol(tok); // Length of message
                    if(length == 0)
                        length = 1;
                    insert_send(app, task_events, to, from, tag, length);
                } 
            }
            else if(strcmp(tok, "r") == 0) { // Communication.
                tok = strtok(NULL, sep); // from
                from = atol(tok); // Node to add event
                tok=strtok(NULL, sep);
                to = atol(tok); // event's PID: destiny when we are sending
                if (to != from) {
                    tok = strtok(NULL, sep);
                    tag = atol(tok); // Type of message (tag)
                    tok = strtok(NULL, sep);
                    length = atol(tok); // Length of message
                    if(length == 0)
                        length = 1;
                    insert_recv(app, task_events, to, from, tag, length);
                } 
            }
            else if (strcmp(tok, "c")==0){ // Computation.
                tok = strtok(NULL, sep);	
                to = atol(tok); // nodeId.
                tok = strtok(NULL, sep);
                length=atol(tok); // Computation time.
                insert_computation(app, task_events, to, length);
            }
        }
    }
    fclose(ftrc);
}

/**
 * Reads a trace from a file.
 *
 * Read a trace from a fsin trc file whose name is in global variable #trcfile
 * This format only takes in account 'c' CPU, 's' SEND, 'r' RECV, events.
 */
void read_alog_trc(application *app, list_t **task_events, long size) {

    FILE * ftrc;
    char buffer[512];
    char * tok;
    char sep[]=" \t";
    long to, from, tag, length;

    if((ftrc = fopen(app->pattern_file, "r")) == NULL){
        printf("Trace file not found in current directory\n");
        exit(-1);
    }
    while(fgets(buffer, 512, ftrc) != NULL) {
        if(buffer[0] != '\n' && buffer[0] != '#') {
            if(buffer[strlen(buffer) - 1] == '\n')
                buffer[strlen(buffer) - 1] = '\0';

            tok = strtok( buffer, sep);

            if (strcmp(tok, "-101") == 0) { // Communication.
                tok = strtok(NULL, sep); // from
                from = atol(tok); // Node to add event
                tok=strtok(NULL, sep);
                tok=strtok(NULL, sep);
                to = atol(tok); // event's PID: destiny when we are sending
                if (to != from) {
                    tok = strtok(NULL, sep);
                    tok = strtok(NULL, sep);
                    tok = strtok(NULL, sep);
                    tag = atol(tok); // Type of message (tag)
                    tok = strtok(NULL, sep);
                    length = atol(tok); // Length of message
                    if(length == 0)
                        length = 1;
                    insert_send(app, task_events, to, from, tag, length);
                } 
            }
            else if(strcmp(tok, "-102") == 0) { // Communication.
                tok = strtok(NULL, sep); // from
                to = atol(tok); // Node to add event
                tok=strtok(NULL, sep);
                tok=strtok(NULL, sep);
                from = atol(tok); // event's PID: destiny when we are sending
                if (to != from) {
                    tok = strtok(NULL, sep);
                    tok = strtok(NULL, sep);
                    tok = strtok(NULL, sep);
                    tag = atol(tok); // Type of message (tag)
                    tok = strtok(NULL, sep);
                    length = atol(tok); // Length of message
                    if(length == 0)
                        length = 1;
                    insert_recv(app, task_events, to, from, tag, length);
                } 
            }
        }
    }
    fclose(ftrc);
}
