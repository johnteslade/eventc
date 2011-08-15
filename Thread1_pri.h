/*

Private header file for thread 1

*/

#ifndef thread1_pri_h
#define thread1_pri_h

// EVENTC: comp=THREAD_1
// EVENTC: comp_type=thread_1_comp_t

 /* Structures used as inputs */
 
typedef struct 
{
	EVENTC_COMP_STRUCT_START
	int static_val_1;
} thread_1_comp_t;

// Start

EVENTC_COMP_START thread_1_start(thread_1_comp_t * self);

// Main

EVENTC_COMP_MAIN thread_1_main(void * start_ptr);

// Call funcs

EVENTC_FUNCTION_RET thread_1_function(thread_1_comp_t * self, thread_1_input * struct_in);

EVENTC_FUNCTION_RET thread_1_function_1(
	thread_1_comp_t * self, 
	thread_1_input * struct_in
);

EVENTC_FUNCTION_RET thread_1_function_2(thread_1_comp_t *self, thread_1_input *struct_in);


#endif /* thread1_pri_h */

