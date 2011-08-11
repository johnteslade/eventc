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

typedef struct 
{
	EVENTC_COMP_STRUCT_START
	int static_val_1;
} thread_1_comp_t;

comp_t * thread_1_new(void);
void * thread_1(void * start_ptr);
void thread_1_function(thread_1_comp_t * self, thread_1_input * struct_in);
void thread_1_function_call(comp_t * comp_details, thread_1_input * struct_in);

#endif /* thread1_h */


