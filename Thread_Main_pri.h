/*

Private header file for thread main

*/

#ifndef thread_main_pri_h
#define thread_main_pri_h

// EVENTC: comp=EVENTC_MAIN_COMPONENT_ID
// EVENTC: comp_type=thread_main_comp_t

#include "eventc.h"
#include "Thread_Main.h"

 /* Structures used as inputs */
 
typedef struct 
{
	EVENTC_COMP_STRUCT_START
} thread_main_comp_t;

// Start

EVENTC_COMP_START thread_main_start(thread_main_comp_t * self);

// End

EVENTC_COMP_END thread_main_end(thread_main_comp_t * self);

// Main

EVENTC_COMP_MAIN thread_main_main(void * start_ptr);

#endif /* thread_main_pri_h */

