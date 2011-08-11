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

	/* Malloc space for component details */
	new_self = malloc(sizeof(thread_1_comp_t));
	assert(new_self != NULL);

	EVENTC_SET_COMPONENT(new_self, THREAD_1);
	
	EVENTC_SET_START_ROUTINE(new_self, &thread_1);

	/* TODO should also register functions here */

	/* Return thread details */
	return (comp_t *)new_self;

}


void * thread_1(void * start_ptr)
{

	thread_1_comp_t * local_attr = NULL;
	
	assert(start_ptr != NULL);

	local_attr = (thread_1_comp_t*)start_ptr; 

	printf("%s started\n", __FUNCTION__);

	/* Check this is the correct thread data */
	assert(EVENTC_COMPONENT(local_attr) == THREAD_1);

	while (1)
	{

		thread_1_input * recv_data = NULL;
		ssize_t bytes_read;

		/* receive the message */
		bytes_read = mq_receive(local_attr->comp_details.queue_id, (void *)&recv_data, sizeof(recv_data), NULL);
//		printf("%s %d: errno = %d - %s\n", __FUNCTION__, __LINE__, errno, strerror(errno));
		assert(bytes_read >= 0);

		printf("%s: mq recv\n", __FUNCTION__);

		/* TODO use func pointers here or perhaps we can autogen this */

		thread_1_function(local_attr, recv_data);
		
		free(recv_data);
	
	}

	free(local_attr);

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

CALL_FUNCTION_IMPLEMENTATION(thread_1_function_call, thread_1_input, THREAD_1)

/*

void thread_1_function_call(comp_t * comp_details, thread_1_input * struct_in)
{

	thread_1_input * tmp_struct;
	int ret;
	mqd_t recv_queue;

	tmp_struct = malloc(sizeof(*tmp_struct));
	memcpy(tmp_struct, struct_in, sizeof(tmp_struct));

	recv_queue = find_receiver_queue(comp_details, THREAD_1);

	ret = mq_send(recv_queue, (const char *)&tmp_struct, sizeof(tmp_struct), 0);
	assert(ret == 0);
	
}

*/

