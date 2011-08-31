/* Main thread 1 code */

#include "eventc.h"
#include "model.h"
#include "Thread2.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "Thread1.h"
#include "Thread1_pri.h"

comp_t * thread_1_new(void)
{

	/* TODO this can be broken apart so it just does component specific items */

	thread_1_comp_t * new_self = NULL;
	
	printf("%s: top\n", __FUNCTION__);

	EVENTC_NEW_COMPONENT(new_self, THREAD_1, "Thread_1", &thread_1_main);

	/* Return thread details */
	return (comp_t *)new_self;

}

void thread_1_start(thread_1_comp_t * self)
{
	printf("%s: start\n", __FUNCTION__);
}

void thread_1_end(thread_1_comp_t * self)
{
	printf("%s: end\n", __FUNCTION__);
}

void thread_1_function(thread_1_comp_t * self, thread_1_input * struct_in)
{

	printf("%s: got %d %d\n", __FUNCTION__, struct_in->int1, struct_in->int2);
	
	// Send message 
	{
		thread_2_input send_struct = {0};
		send_struct.int1 = struct_in->int1 + 1;
		send_struct.int2 = struct_in->int2 + 1;
		CALL_TIMED_thread_2_function(&(self->comp_details), &send_struct, 5, 0);
	}	
	
	// Send message to self
	{
		thread_1_input send_struct = {0};
		send_struct.int1 = struct_in->int1 + 1;
		send_struct.int2 = struct_in->int2 + 1;
		CALL_TIMED_thread_1_function_1(&(self->comp_details), &send_struct, 1, 0);
	}

}

void thread_1_function_1(thread_1_comp_t * self, thread_1_input * struct_in)
{

	printf("%s: LOOOOPBACK  got %d %d\n", __FUNCTION__, struct_in->int1, struct_in->int2);

}
	
void thread_1_function_2(thread_1_comp_t * self, thread_1_input * struct_in)
{

	printf("%s: got %d %d\n", __FUNCTION__, struct_in->int1, struct_in->int2);
	
}
