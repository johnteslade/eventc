#include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
//#include <sys/stat.h>
//#include <sys/types.h>
#include <errno.h>
#include <mqueue.h>


#include "eventc.h"
#include "eventc_connections.h"

#include "model.h"
#include "Thread1.h"
#include "Thread2.h"

/***************************************/
// Static protypes
/***************************************/

static void init_component(comp_t * comp_details);
static void start_thread(comp_t * comp_details);
static mqd_t open_queue(int instance_id);
static mqd_t create_thread_q(char * name);
static void start_component(comp_t * comp_details);
static void wait_thread(comp_t * comp_details);

/***************************************/
// Public
/***************************************/

int main()
{
	
	comp_t * thread_1 = NULL;
	comp_t * thread_2 = NULL;
	
	/* Create thread 1 */
	{
		thread_1 = thread_1_new();
		init_component(thread_1);	
	}

	/* Create thread 2 */
	{
		thread_2 = thread_2_new();
		init_component(thread_2);	
	}

	/* Add connection */
	eventc_connections_add(thread_1, thread_2);

	/* Start all components */
	start_component(thread_1);	
	start_component(thread_2);	

	/* Wait the main program on these threads */
	// TODO sort this out - we cannot wait on all threads to finish
	wait_thread(thread_1);
	wait_thread(thread_2);

	/* Free components now - TODO need a clear function on each component */
	free(thread_1);
	free(thread_2);

	return 0;

}

static void init_component(comp_t * comp_details)
{
	static int instance_id = 999; /* TODO move this to a better place */

	/* Set instance id */
	comp_details->instance_id = instance_id++;

	/* Open queue */
	comp_details->queue_id = open_queue(comp_details->instance_id);

	/* Start thread */
	start_thread(comp_details);

}

static void start_component(comp_t * comp_details)
{

	int ret; 

	eventc_call_t * call_struct = NULL;

	call_struct = malloc(sizeof(*call_struct));
	assert(call_struct != NULL);

	// Fire a NULL pointer at the component to start it
	call_struct->function_id = 0;
	call_struct->comp_id = comp_details->comp_id;
	call_struct->data = NULL;

	ret = mq_send(comp_details->queue_id, (const char *)&call_struct, sizeof(call_struct), 0); 
//	printf("ret = %d, errno = %d (%s)\n", ret, errno, strerror(errno));
	assert(ret == 0);
	
}



static void start_thread(comp_t * comp_details)
{

	int s;
	pthread_attr_t attr;

	s = pthread_attr_init(&attr);

	/* Create the new thread for this component */
	s = pthread_create(&(comp_details->thread_id), &attr, comp_details->main_func, comp_details);
	assert(s == 0);

}

static void wait_thread(comp_t * comp_details)
{

	int s;
	s = pthread_join(comp_details->thread_id, NULL);
	assert(s == 0);

}

static mqd_t open_queue(int instance_id)
{

	char queue_name[100];
	mqd_t new_mq_ref;

	/* Create the queue */
	(void)snprintf(queue_name, sizeof(queue_name), "/thread%d", instance_id);
	new_mq_ref = create_thread_q(queue_name);

	return new_mq_ref;

}

/******* Event C code ****/



/******* Util funcs ****/



static mqd_t create_thread_q(char * name)
{

    mqd_t mq;
    struct mq_attr attr;

    /* initialize the queue attributes */
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(void *);
    attr.mq_curmsgs = 0;

    /* create the message queue */
    mq = mq_open(name, O_CREAT | O_RDWR, 0644, &attr);
    assert((mqd_t)-1 != mq);
	return mq;

}


