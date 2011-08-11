#include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <mqueue.h>


#include "eventc.h"
#include "model.h"
#include "Thread1.h"

/***************************************/
// Static protypes
/***************************************/

static void init_component(comp_t * comp_details);
static void start_thread(comp_t * comp_details);
static mqd_t open_queue(int instance_id);
static mqd_t create_thread_q(char * name);

// Temp vars until reciever queues impleneted.  TODO remove
static mqd_t thread1_mq;
static mqd_t thread2_mq;

/***************************************/
// Public
/***************************************/

void main()
{
	
	comp_t * thread_1 = NULL;
	
	/* Start thread 1 */
	thread_1 = thread_1_new();
	init_component(thread_1);	
	thread1_mq = thread_1->queue_id; // TODO remove



}

static void init_component(comp_t * comp_details)
{
	static int instance_id = 1; /* TODO move this to a better place */

	/* Open queue */
	comp_details->queue_id = open_queue(instance_id++);

	/* Start thread */
	start_thread(comp_details);

}

static void start_thread(comp_t * comp_details)
{

	int s;
	pthread_t thread_1_id;
	pthread_attr_t attr;

	s = pthread_attr_init(&attr);

	/* Create the new thread for this component */
	s = pthread_create(&thread_1_id, &attr, comp_details->start_func, comp_details);
	assert(s == 0);
	
	s = pthread_join(thread_1_id, NULL);
	assert(s == 0);

}

static mqd_t open_queue(int instance_id)
{

	char queue_name[100];
	mqd_t thread1_mq;

	/* Create the queue */
	sprintf(queue_name, "/thread%d", instance_id);
	thread1_mq = create_thread_q(queue_name);

}

/******* Event C code ****/

mqd_t find_receiver_queue(comp_t * sender_details, int dest_comp_id)
{

	if (dest_comp_id == THREAD_1)
	{
		return thread1_mq;
	}
	else if (dest_comp_id == THREAD_2)
	{
		return thread2_mq;
	}
	else
	{
		assert(0);
	}

}

/******* Util funcs ****/



static mqd_t create_thread_q(char * name)
{

    mqd_t mq;
    struct mq_attr attr;
    int must_stop = 0;

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


