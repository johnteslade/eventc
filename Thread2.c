/* Main thread 2 code */

#include "eventc.h"
#include "model.h"
#include "Thread1.h"
#include "Thread2.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>


comp_t * thread_2_new(void)
{

	/* TODO this can be broken apart so it just does component specific items */

	thread_2_comp_t * new_self = NULL;
	
	printf("%s: top\n", __FUNCTION__);

	/* Malloc space for component details */
	new_self = malloc(sizeof(thread_2_comp_t));
	assert(new_self != NULL);

	EVENTC_SET_COMPONENT(new_self, THREAD_2);
	
	EVENTC_SET_START_ROUTINE(new_self, &thread_2);

	/* TODO should also register functions here */

	/* Return thread details */
	return (comp_t *)new_self;

}

void * thread_2(void * start_ptr)
{

	thread_2_comp_t * local_attr = NULL;
	
	printf("%s: top\n", __FUNCTION__);
	
	assert(start_ptr != NULL);

	local_attr = (thread_2_comp_t*)start_ptr; 

	printf("%s started\n", __FUNCTION__);

	/* Check this is the correct thread data */
	assert(EVENTC_COMPONENT(local_attr) == THREAD_2);

	while (1)
	{

		thread_2_input * recv_data = NULL;
		ssize_t bytes_read;

		/* receive the message */
		bytes_read = mq_receive(local_attr->comp_details.queue_id, (void *)&recv_data, sizeof(recv_data), NULL);
//		printf("%s %d: errno = %d - %s\n", __FUNCTION__, __LINE__, errno, strerror(errno));
		assert(bytes_read >= 0);

		printf("%s: mq recv\n", __FUNCTION__);

		if (recv_data == NULL)
		{
			thread_2_start(local_attr);
		}
		else
		{

			/* TODO use func pointers here or perhaps we can autogen this */
			thread_2_function(local_attr, recv_data);

			free(recv_data);
		}
	
	}

	free(local_attr);

}

void thread_2_start(thread_2_comp_t * self)
{
	printf("%s: start\n", __FUNCTION__);

	// Send start message 
	{
		thread_1_input send_struct;
		send_struct.int1 = 1;
		send_struct.int2 = 2;
		thread_1_function_call(&(self->comp_details), &send_struct);
	}	

}

void thread_2_function(thread_2_comp_t * self, thread_2_input * struct_in)
{

	printf("%s: got %d %d %d\n", __FUNCTION__, struct_in->char1, struct_in->int1, struct_in->int2);
	
	// Send message 
	{
		thread_2_input send_struct;
		send_struct.int1 = struct_in->int1 + 1;
		send_struct.int2 = struct_in->int2 - 1;
		thread_2_function_call(&(self->comp_details), &send_struct);
	}

}

CALL_FUNCTION_IMPLEMENTATION(thread_2_function_call, thread_2_input, THREAD_2)


