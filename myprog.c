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



typedef struct {
	char char1;
	int int1;
	int int2;
} thread_2_input;

static mqd_t thread1_mq;
static mqd_t thread2_mq;

/***************************************/
// Static protypes
/***************************************/

static void * thread_1(void * start_ptr);

static void thread_1_function(thread_1_input * struct_in);
static void thread_1_function_call(thread_1_input * struct_in);

static void * thread_2(void * start_ptr);

static void thread_2_function(thread_2_input * struct_in);
static void thread_2_function_call(thread_2_input * struct_in);

static mqd_t create_thread_q(char * name);


/***************************************/
// Public
/***************************************/

void main()
{
	
	int s;
	pthread_t thread_1_id;
	pthread_t thread_2_id;
	pthread_attr_t attr;

	
	thread1_mq = create_thread_q("/thread1");
	thread2_mq = create_thread_q("/thread2");


	s = pthread_attr_init(&attr);

//	s = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
//	assert(s == 0);

	s = pthread_create(&thread_1_id, &attr, &thread_1, NULL);
	assert(s == 0);
	
	s = pthread_create(&thread_2_id, &attr, &thread_2, NULL);
	assert(s == 0);

	s = pthread_join(thread_1_id, NULL);
	assert(s == 0);
	
	s = pthread_join(thread_2_id, NULL);
	assert(s == 0);

}

/***************************************/
// Private
/***************************************/

/******* Thread 1 ****/

static void * thread_1(void * start_ptr)
{


	printf("%s started\n", __FUNCTION__);

	while (1)
	{

		thread_1_input * recv_data = NULL;
		ssize_t bytes_read;

		/* receive the message */
		bytes_read = mq_receive(thread1_mq, (void *)&recv_data, sizeof(recv_data), NULL);
//		printf("%s %d: errno = %d - %s\n", __FUNCTION__, __LINE__, errno, strerror(errno));
		assert(bytes_read >= 0);

		printf("%s: mq recv\n", __FUNCTION__);

		thread_1_function(recv_data);
		
		free(recv_data);
	
	}

}

static void thread_1_function(thread_1_input * struct_in)
{

	printf("%s: got %d %d\n", __FUNCTION__, struct_in->int1, struct_in->int2);
	
	// Slow it down slightly
	sleep(1);

	// Send message 
	{
		thread_2_input send_struct;
		send_struct.int1 = struct_in->int1 + 1;
		send_struct.int2 = struct_in->int2 + 1;
		thread_2_function_call(&send_struct);
	}	

}

static void thread_1_function_call(thread_1_input * struct_in)
{

	thread_1_input * tmp_struct;
	int ret;

	tmp_struct = malloc(sizeof(*tmp_struct));
	memcpy(tmp_struct, struct_in, sizeof(tmp_struct));

	ret = mq_send(thread1_mq, &tmp_struct, sizeof(tmp_struct), 0);
	assert(ret == 0);
	
}


/******* Thread 2 ****/

static void * thread_2(void * start_ptr)
{

	
	printf("%s started\n", __FUNCTION__);

	// Send message 
	{
		thread_1_input send_struct;
		send_struct.int1 = 1;
		send_struct.int2 = 2;
		thread_1_function_call(&send_struct);
	}	

	while (1)
	{

		thread_2_input * recv_data;
		ssize_t bytes_read;

		/* receive the message */
		bytes_read = mq_receive(thread2_mq, (void *)&recv_data, sizeof(recv_data), NULL);
		assert(bytes_read >= 0);

		printf("%s: mq recv\n", __FUNCTION__);

		thread_2_function(recv_data);

		free(recv_data);

	}

}

static void thread_2_function(thread_2_input * struct_in)
{

	printf("%s: got %d %d %d\n", __FUNCTION__, struct_in->char1, struct_in->int1, struct_in->int2);
	
	// Send message 
	{
		thread_1_input send_struct;
		send_struct.int1 = struct_in->int1 + 1;
		send_struct.int2 = struct_in->int2 - 1;
		thread_1_function_call(&send_struct);
	}

}

static void thread_2_function_call(thread_2_input * struct_in)
{

	thread_2_input * tmp_struct;
	int ret;

	tmp_struct = malloc(sizeof(*tmp_struct));
	memcpy(tmp_struct, struct_in, sizeof(tmp_struct));

	ret = mq_send(thread2_mq, &tmp_struct, sizeof(tmp_struct), 0);
//	printf("%s %d: errno = %d - %s\n", __FUNCTION__, __LINE__, errno, strerror(errno));
	assert(ret == 0);
	
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


