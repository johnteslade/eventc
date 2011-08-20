/* Functions for handling the calling of functions */

#include "eventc_funcqueue.h"
#include "eventc.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"
#include "unistd.h"

void eventc_funcqueue_add(
	int function_id,
	int comp_id,
	comp_t * sender_details, 
	void * data,
	int data_len
)
{

	eventc_call_t * call_struct = NULL; /* Message structure */
	int ret = -1; 
	mqd_t recv_queue; 

	/* Create the call struct */
	call_struct = malloc(sizeof(*call_struct));
	assert(call_struct != NULL);
	call_struct->function_id = function_id;
	call_struct->comp_id = comp_id;

	/* Copy across the input data into a new struct */
	call_struct->data = malloc(data_len); 
	assert(call_struct->data != NULL); 
	memset(call_struct->data, 0x00, data_len); 
	memcpy(call_struct->data, data, data_len); 

	/* Find a receiver */
	recv_queue = eventc_connections_find_receiver(sender_details, call_struct->comp_id); 

	/* Send the message */
	ret = mq_send(recv_queue, (const char *)&call_struct, sizeof(call_struct), 0); 
	assert(ret == 0); 

}



