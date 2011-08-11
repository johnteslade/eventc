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

typedef struct 
{
	EVENTC_COMP_STRUCT_START
	int static_val_2;
} thread_2_comp_t;

void * thread_2(void * start_ptr);
comp_t * thread_2_new(void);
void thread_2_start(thread_2_comp_t * self);
void thread_2_function(thread_2_comp_t * self, thread_2_input * struct_in);
CALL_FUNCTION_FUNC_PROTO(thread_2_function_call, thread_2_input);

#endif /* thread2_h */

