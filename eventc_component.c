/* Functions for components  */

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

/***************************************/
// Static protypes
/***************************************/

static void start_thread(comp_t * comp_details);
static mqd_t open_queue(int instance_id);

/***************************************/
// Local Variables
/***************************************/

static int next_instance_id = 1000; /* The next instance ID to use */

/***************************************/
// Public
/***************************************/

void eventc_component_init(comp_t * comp_details)
{

	/* Set instance id */
	comp_details->instance_id = next_instance_id++;

	/* Open queue */
	comp_details->queue_id = open_queue(comp_details->instance_id);

	/* Start thread */
	start_thread(comp_details);

	printf("%s: Instance created.  Comp %s, Instance %d\n", __FUNCTION__, comp_details->comp_name, comp_details->instance_id);

}

void eventc_component_start(comp_t * comp_details)
{

	int ret; 

	eventc_call_t * call_struct = NULL;

	/* Send special start message to component */
	call_struct = malloc(sizeof(*call_struct));
	assert(EVENTC_IS_VALID_PTR(call_struct));
	EVENTC_INIT_STRUCT(*call_struct, EVENTC_STRUCT_call_t);
	call_struct->function_id = 0;
	call_struct->comp_id = comp_details->comp_id;
	call_struct->call_type = EVENTC_START;

	ret = mq_send(comp_details->queue_id, (const char *)&call_struct, sizeof(call_struct), 0); 
//	printf("ret = %d, errno = %d (%s)\n", ret, errno, strerror(errno));
	assert(ret == 0);
	
}

void eventc_component_wait(comp_t * comp_details)
{

	int s;
	s = pthread_join(comp_details->thread_id, NULL);
	assert(s == 0);

}

/******* Util func : TODO this should be in a better place ****/

mqd_t create_thread_q(char * name)
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

/***************************************/
// Private
/***************************************/

static void start_thread(comp_t * comp_details)
{

	int s;
	pthread_attr_t attr;

	s = pthread_attr_init(&attr);

	/* Create the new thread for this component */
	s = pthread_create(&(comp_details->thread_id), &attr, comp_details->main_func, comp_details);
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


