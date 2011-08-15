/* Main thread 1 code */

#include "eventc.h"
#include "model.h"
#include "Thread1.h"
#include "Thread2.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>



comp_t * thread_1_new(void)
{

	/* TODO this can be broken apart so it just does component specific items */

	thread_1_comp_t * new_self = NULL;
	
	printf("%s: top\n", __FUNCTION__);

	/* Malloc space for component details */
	new_self = malloc(sizeof(thread_1_comp_t));
	assert(new_self != NULL);

	EVENTC_SET_COMPONENT(new_self, THREAD_1);
	
	EVENTC_SET_MAIN_ROUTINE(new_self, &thread_1_main);

	/* Return thread details */
	return (comp_t *)new_self;

}

void thread_1_start(thread_1_comp_t * self)
{
	printf("%s: start\n", __FUNCTION__);
}

void thread_1_function(thread_1_comp_t * self, thread_1_input * struct_in)
{

	printf("%s: got %d %d\n", __FUNCTION__, struct_in->int1, struct_in->int2);
	
	// Slow it down slightly
	sleep(1);

	// Send message 
	{
		thread_2_input send_struct = {0};
		send_struct.int1 = struct_in->int1 + 1;
		send_struct.int2 = struct_in->int2 + 1;
		thread_2_function_call(&(self->comp_details), &send_struct);
	}	

}


