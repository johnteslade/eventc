

#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

#include "eventc.h"
#include "eventc_connections.h"
#include "eventc_component.h"
#include "eventc_timed.h"

#include "model.h"
#include "Thread1.h"
#include "Thread2.h"

/***************************************/
// Static protypes
/***************************************/

/***************************************/
// Public
/***************************************/

int main()
{
	
	comp_t * thread_1 = NULL;
	comp_t * thread_2 = NULL;
	comp_t * thread_3 = NULL;
	
	comp_t outer_thread = {0}; /* temp outer componennt TODO remove */

	/* Start the timed thread - TODO intetially don't join here.  TODO needs a better place for this */
	{
		int s;
		pthread_attr_t attr;
		pthread_t thread_id;

		s = pthread_attr_init(&attr);

		/* Create the new thread for this component */
		s = pthread_create(&thread_id, &attr, eventc_timed_start, NULL);
		assert(s == 0);
	}



	/* Create thread 1 */
	thread_1 = thread_1_new();
	eventc_component_addsub(&outer_thread, thread_1);

	/* Create thread 2 */
	thread_2 = thread_2_new();
	eventc_component_addsub(&outer_thread, thread_2);

	/* Create thread 3 */
	thread_3 = thread_2_new();
	eventc_component_addsub(&outer_thread, thread_3);

	/* Init components */
	eventc_component_initsub(&outer_thread);

	/* Add connection */
	eventc_connections_add(thread_1, thread_2);
	//eventc_connections_add(thread_1, thread_3);

	/* Start all components */
	eventc_component_start(thread_1);	
	eventc_component_start(thread_2);	

	/* Wait the main program on these threads */
	// TODO sort this out - we cannot wait on all threads to finish
	eventc_component_wait(thread_1);
	eventc_component_wait(thread_2);

	/* Free components now - TODO need a clear function on each component */
	free(thread_1);
	free(thread_2);

	return 0;

}



