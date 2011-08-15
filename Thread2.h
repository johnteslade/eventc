/*

Public header file for thread 2 

*/

#ifndef thread2_h
#define thread2_h

typedef struct {
	char char1;
	int int1;
	int int2;
} thread_2_input;

/* Include the auto gen code */
#include "Thread2_autogen.h"

comp_t * thread_2_new(void);

#endif /* thread2_h */

