/* Functions for handling timed calls */

#include "eventc_timed.h"
#include "eventc_component.h" /* TODO remove when mqueue create is in better place */
#include "eventc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <errno.h>
#include <stdbool.h>

/***************************************/
// Types
/***************************************/

#define MAX_TIMED_QUEUE 20

typedef struct
{
	bool in_use;
	timed_event_call_t * event_details;
} queued_timed_event_t;

/***************************************/
// Local Vars
/***************************************/

static queued_timed_event_t timed_queue[MAX_TIMED_QUEUE] = {{0}}; /* Queued items */

static mqd_t timed_queue_id; /* Queue id for timed messages */

/***************************************/
// Static protypes
/***************************************/

static void send_ready_events(void);
static int find_earliest_event(struct timespec * earliest_time);
static void add_timed_event(timed_event_call_t * new_event_details);

/***************************************/
// Public
/***************************************/

/* Returns the mqueue */
mqd_t eventc_timed_get_q(void)
{
	return timed_queue_id;
}

/* Thread start point */
void * eventc_timed_start(void * dummy)
{

	int events_waiting = 0; /* Are there events waiting in the queue? */
	struct timespec next_event = {0}; /* Time of next event */
	timed_event_call_t * timed_event = NULL;
	ssize_t bytes_read;

	timed_queue_id = create_thread_q("/timed_queue.mq");

	while (1)
	{

		/* Wait on the queue - timeout if we have items in the queue */
		events_waiting = find_earliest_event(&next_event);

		if (events_waiting)
		{
			printf("%s: timed event, waiting for %d %ld\n", __FUNCTION__, (int)next_event.tv_sec, next_event.tv_nsec);
			bytes_read = mq_timedreceive(timed_queue_id, (void *)&timed_event, sizeof(timed_event), NULL, &next_event);
			assert( (bytes_read == sizeof(timed_event)) || ( (bytes_read == -1) && (errno == ETIMEDOUT)) );
		}
		else
		{
			bytes_read = mq_receive(timed_queue_id, (void *)&timed_event, sizeof(timed_event), NULL);
			assert(bytes_read == sizeof(timed_event));
		}

		/* If we have a new event add it to the queue */
		if (bytes_read >= 0)
		{
			EVENTC_ASSERT_CORRECT_STRUCT(*timed_event, EVENTC_STRUCT_timed_call_t);
			add_timed_event(timed_event);	
		}

		/* Send anything that is now ready */
		send_ready_events();

	}	
}

/***************************************/
// Private
/***************************************/

/* Sends events that are now ready to go */
static void send_ready_events(void)
{

	int i = 0; /* Loop counter */
	int ret = -1;
	struct timespec current_time = {0};

	ret = clock_gettime(CLOCK_REALTIME, &current_time);
	assert(ret == 0);

	/* Find a free row and add */
	for (i = 0; i < EVENTC_ARRAY_SIZE(timed_queue); i++)
	{
		if (timed_queue[i].in_use == true)
		{

			/* Find earliest time in the list */
			if ( (timed_queue[i].event_details->secs < current_time.tv_sec) || ( (timed_queue[i].event_details->secs == current_time.tv_sec) && (timed_queue[i].event_details->nsecs <= current_time.tv_nsec) ) )
			{

				printf("%s: timed event now being released Dest Comp: %d, Dest Func: %d\n", __FUNCTION__, timed_queue[i].event_details->event_call->comp_id, timed_queue[i].event_details->event_call->function_id);

				/* Send the message */
				ret = mq_send(timed_queue[i].event_details->dest_queue_id, (const char *)&timed_queue[i].event_details->event_call, sizeof(timed_queue[i].event_details->event_call), 0); 
				assert(ret == 0); 
			
				free(timed_queue[i].event_details);

				memset(&(timed_queue[i]), 0x00, sizeof(timed_queue[i]));
				timed_queue[i].in_use = false;
			}
		}
	}
}

/* Finds the earliest time in the queue list, return 0 if there are no items */
static int find_earliest_event(struct timespec * earliest_time)
{

	int i = 0; /* Loop counter */
	bool item_found = false; /* Is there an item in the queue */

	/* Find a free row and add */
	for (i = 0; i < EVENTC_ARRAY_SIZE(timed_queue); i++)
	{
		if (timed_queue[i].in_use == true)
		{
			/* Find earliest time in the list */
			if ( (item_found == 0) || (timed_queue[i].event_details->secs < earliest_time->tv_sec) || ( (timed_queue[i].event_details->secs < earliest_time->tv_sec) && (timed_queue[i].event_details->nsecs < earliest_time->tv_nsec) ) )
			{
				earliest_time->tv_sec = timed_queue[i].event_details->secs;
				earliest_time->tv_nsec = timed_queue[i].event_details->nsecs;
			}
			item_found = true;
		}
	}

	return item_found;

}

/* Add item to queue */
static void add_timed_event(timed_event_call_t * new_event_details)
{

	int i = 0; /* Loop counter */
	bool item_added = false; /* Was the item added to the queue */

	printf("%s: new timed event added.  Dest Comp: %d, Dest Func: %d, Time: %d %ld\n", __FUNCTION__, new_event_details->event_call->comp_id, new_event_details->event_call->function_id, new_event_details->secs, new_event_details->nsecs);
	
	/* Find a free row and add */
	for (i = 0; i < EVENTC_ARRAY_SIZE(timed_queue); i++)
	{
		if (timed_queue[i].in_use == false)
		{
			timed_queue[i].event_details = new_event_details;
			timed_queue[i].in_use = true;
			item_added = true;
			break;
		}
	}
	
	assert(item_added == true);

}

#ifdef UNIT_TEST

#include "CUnit/Basic.h"


static void TEST_find_earliest(void)
{

	#define EVENT1_SECS 10
	#define EVENT2_SECS 20
	#define EVENT3_SECS 5

	timed_event_call_t event1;
	timed_event_call_t event2;
	timed_event_call_t event3;
	struct timespec earliest_time = {0};
	bool result = false;

	// Create the events
	event1.event_call = malloc(sizeof(*event1.event_call));
	event1.secs = EVENT1_SECS;
	event1.event_call->comp_id = 1;

	event2.event_call = malloc(sizeof(*event2.event_call));
	event2.secs = EVENT2_SECS;
	event2.event_call->comp_id = 2;

	event3.event_call = malloc(sizeof(*event3.event_call));
	event3.secs = EVENT3_SECS;
	event3.event_call->comp_id = 3;
	
	// Add first event and check earliest time
	add_timed_event(&event1);
	result = find_earliest_event(&earliest_time);
	CU_ASSERT_TRUE(result);
	CU_ASSERT_EQUAL(earliest_time.tv_sec, EVENT1_SECS);
	
	// Add event 2 - earliest should still be event 1
	add_timed_event(&event2);
	result = find_earliest_event(&earliest_time);
	CU_ASSERT_TRUE(result);
	CU_ASSERT_EQUAL(earliest_time.tv_sec, EVENT1_SECS);
	
	// Add event 3 - earliest should now be event 3
	add_timed_event(&event3);
	result = find_earliest_event(&earliest_time);
	CU_ASSERT_TRUE(result);
	CU_ASSERT_EQUAL(earliest_time.tv_sec, EVENT3_SECS);

}

int main()
{
	CU_pSuite pSuite = NULL;

	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry())
	{
		return CU_get_error();
	}

	/* add a suite to the registry */
	pSuite = CU_add_suite("Suite_1", NULL, NULL);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if ((NULL == CU_add_test(pSuite, "test of find earliest", TEST_find_earliest)) )
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* Run all tests using the CUnit Basic interface */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}

#endif /* UNIT_TEST */


