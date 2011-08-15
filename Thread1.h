/*

Public header file for thread 1

*/

#ifndef thread1_h
#define thread1_h

/* Structures used as inputs */

typedef struct {
	int int1;
	int int2;
} thread_1_input;

/* Function to create this component */

comp_t * thread_1_new(void);

#endif /* thread1_h */


