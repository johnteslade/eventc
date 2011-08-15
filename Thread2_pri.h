/*

Private header file for thread 2

*/

#ifndef thread2_pri_h
#define thread2_pri_h

// EVENTC: comp=THREAD_2
// EVENTC: comp_type=thread_2_comp_t

#include "eventc.h"
#include "Thread2.h"

typedef struct 
{
	EVENTC_COMP_STRUCT_START
	int static_val_2;
} thread_2_comp_t;

// Start

EVENTC_COMP_START thread_2_start(thread_2_comp_t * self);

// Main

EVENTC_COMP_MAIN thread_2_main(void * start_ptr);

// Call funcs

EVENTC_FUNCTION_RET thread_2_function(thread_2_comp_t * self, thread_2_input * struct_in);

#endif /* thread2_pri_h */

