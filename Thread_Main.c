/* Main thread code */

#include "eventc.h"
#include "model.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "eventc_component.h"

#include "Thread_Main.h"
#include "Thread_Main_pri.h"

#include "Thread1.h"
#include "Thread2.h"

bool thread1_2_mutator(eventc_mutate_t * mutate_opt, void * data)
{

	static int packet_count = 0;

	packet_count++;

	if (packet_count % 2)
	{
		mutate_opt->drop = true;
		return true;
	}
	else
	{
		return false;
	}

}
comp_t * thread_main_new(void)
{

	thread_main_comp_t * new_self = NULL;
	comp_t * thread_1 = NULL;
	comp_t * thread_2 = NULL;
	comp_t * thread_3 = NULL;
	
	printf("%s: top\n", __FUNCTION__);

	/* Initialise the component fields */
	EVENTC_NEW_COMPONENT(new_self, EVENTC_MAIN_COMPONENT_ID, "Main Thread", &thread_main_main);


	/* Create the sub components */
	
	/* Create thread 1 */
	thread_1 = thread_1_new();
	eventc_component_addsub(EVENTC_COMP_DETAILS(new_self), thread_1);

	/* Create thread 2 */
	thread_2 = thread_2_new();
	eventc_component_addsub(EVENTC_COMP_DETAILS(new_self), thread_2);

	/* Create thread 3 */
//	thread_3 = thread_2_new();
//	eventc_component_addsub(new_self, thread_3);

	/* Init components */
	eventc_component_initsub(EVENTC_COMP_DETAILS(new_self));

	/* Add connection */
	eventc_connections_add_with_mutate(thread_1, thread_2, &thread1_2_mutator, NULL);
	//eventc_connections_add(thread_1, thread_3);
	
	/* Return thread details */
	return (comp_t *)new_self;

}

void thread_main_start(thread_main_comp_t * self)
{

	printf("%s: start\n", __FUNCTION__);

	/* Start all sub components */
	eventc_component_startsub(EVENTC_COMP_DETAILS(self));	

}

void thread_main_end(thread_main_comp_t * self)
{
	printf("%s: end\n", __FUNCTION__);
}


