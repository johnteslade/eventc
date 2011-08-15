/*

Public header file for thread 1

*/

#ifndef thread1_h
#define thread1_h

// EVENTC: comp=THREAD_1
// EVENTC: comp_type=thread_1_comp_t

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

EVENTC_COMP_START thread_1_start(thread_1_comp_t * self);

EVENTC_COMP_MAIN thread_1_main(void * start_ptr);

EVENTC_FUNCTION_RET thread_1_function(thread_1_comp_t * self, thread_1_input * struct_in);

EVENTC_FUNCTION_RET thread_1_function_1(
	thread_1_comp_t * self, 
	thread_1_input * struct_in
);

EVENTC_FUNCTION_RET thread_1_function_2(thread_1_comp_t *self, thread_1_input *struct_in);


#endif /* thread1_h */


