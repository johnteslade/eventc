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

void thread_2_function(thread_2_comp_t * self, thread_2_input * struct_in);
void thread_2_function_call(comp_t * comp_details, thread_2_input * struct_in);


#endif /* thread2_h */

