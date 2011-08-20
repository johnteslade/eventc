/* Functions for handling the calling of functions */

#include "eventc_funcqueue.h"
#include "eventc_timed.h"
#include "eventc.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"
#include "unistd.h"

static void create_call_structure(
	eventc_call_t ** call_struct,
	int function_id,
	int comp_id,
	void * data,
	int data_len
);

void eventc_funcqueue_add(
	int function_id,
	char * function_name,
	int comp_id,
	comp_t * sender, 
	void * data,
	int data_len
)
{

	eventc_call_t * call_struct = NULL; /* Message structure */
	int ret = -1; 
	mqd_t recv_queue; 

	printf("%s: %s inst:%d calling %s\n", __FUNCTION__, sender->comp_name, sender->instance_id, function_name);

	create_call_structure(&call_struct,	function_id, comp_id, data, data_len);

	/* Find a receiver - prevent loopback calls */
	recv_queue = eventc_connections_find_receiver(sender, call_struct->comp_id, false); 

	/* Send the message */
	ret = mq_send(recv_queue, (const char *)&call_struct, sizeof(call_struct), 0); 
	assert(ret == 0); 

}

/* Create a timed event - times relative to now */
void eventc_funcqueue_timed(
	int function_id,
	char * function_name,
	int comp_id,
	comp_t * sender, 
	int secs,
	long nsecs,
	void * data,
	int data_len
)
{

	eventc_call_t * call_struct = NULL; /* Message structure */
	int ret = -1; 
	mqd_t recv_queue;
	timed_event_call_t * timed_call_event = NULL; /* Time call struct */
	struct timespec current_time = {0};

	printf("%s: %s inst:%d timed call to %s - s:%d ns:%ld\n", __FUNCTION__, sender->comp_name, sender->instance_id, function_name, secs, nsecs);

	create_call_structure(&call_struct,	function_id, comp_id, data, data_len);

	/* Find a receiver */
	recv_queue = eventc_connections_find_receiver(sender, call_struct->comp_id, true); 

	/* Find out current time */
	ret = clock_gettime(CLOCK_REALTIME, &current_time);
	assert(ret == 0);

	/* Created time struct */
	timed_call_event = malloc(sizeof(*timed_call_event));
	assert(timed_call_event != NULL);
	timed_call_event->secs = current_time.tv_sec + secs;
	timed_call_event->nsecs = current_time.tv_nsec + nsecs;
	timed_call_event->dest_queue_id = recv_queue;
	timed_call_event->event_call = call_struct;

	/* Send the message */
	ret = mq_send(eventc_timed_get_q(), (const char *)&timed_call_event, sizeof(timed_call_event), 0); 
	assert(ret == 0); 

}

/* Creates and setup the call structure */
static void create_call_structure(
	eventc_call_t ** call_struct, /* Struct created */
	int function_id,
	int comp_id,
	void * data,
	int data_len
)
{
	/* Create the call struct */
	(*call_struct) = malloc(sizeof(*call_struct));
	assert((*call_struct) != NULL);
	(*call_struct)->function_id = function_id;
	(*call_struct)->comp_id = comp_id;

	/* TODO save the sender details here */

	/* Copy across the input data into a new struct */
	(*call_struct)->data = malloc(data_len); 
	assert((*call_struct)->data != NULL); 
	memset((*call_struct)->data, 0x00, data_len); 
	memcpy((*call_struct)->data, data, data_len); 
}


