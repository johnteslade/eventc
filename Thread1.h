/*

Public header file for thread 1

*/

#ifndef thread1_h
#define thread1_h

#include "eventc.h"

/* Define the name of the private header for this thread - TODO could this all be based off thread name */
#define EVENTC_PRIVATE_HEADER "Thread1_pri.h"

/* Structures used as inputs */

typedef struct {
	int int1;
	int int2;
} thread_1_input;

/* Create prototypes for public functions */
#include "eventc_pub_proto.h"


#endif /* thread1_h */


