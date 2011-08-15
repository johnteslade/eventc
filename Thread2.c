/* Main thread 2 code */

#include "eventc.h"
#include "model.h"
#include "Thread1.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "Thread2.h"
#include "Thread2_pri.h"

comp_t * thread_2_new(void)
{

	/* TODO this can be broken apart so it just does component specific items */

	thread_2_comp_t * new_self = NULL;
	
	printf("%s: top\n", __FUNCTION__);

	/* Malloc space for component details */
	new_self = malloc(sizeof(thread_2_comp_t));
	assert(new_self != NULL);

	EVENTC_SET_COMPONENT(new_self, THREAD_2);
	
	EVENTC_SET_MAIN_ROUTINE(new_self, &thread_2_main);

	/* Return thread details */
	return (comp_t *)new_self;

}

void thread_2_start(thread_2_comp_t * self)
{
	printf("%s: start\n", __FUNCTION__);

	// Send start message 
	{
		thread_1_input send_struct = {0};
		send_struct.int1 = 1;
		send_struct.int2 = 2;
		CALL_thread_1_function(&(self->comp_details), &send_struct);
	}	

}

void thread_2_function(thread_2_comp_t * self, thread_2_input * struct_in)
{

	printf("%s: got %d %d %d\n", __FUNCTION__, struct_in->char1, struct_in->int1, struct_in->int2);
	
	// Send message 
	{
		thread_1_input send_struct = {0};
		send_struct.int1 = struct_in->int1;
		send_struct.int2 = struct_in->int2;
		CALL_thread_1_function(&(self->comp_details), &send_struct);
	}

}


