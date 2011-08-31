

#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

#include "eventc.h"
#include "eventc_connections.h"
#include "eventc_component.h"
#include "eventc_timed.h"

#include "model.h"
#include "Thread_Main.h"

/***************************************/
// Static protypes
/***************************************/

/***************************************/
// Public
/***************************************/

int main()
{
	
	comp_t * inner_thread = NULL;
	comp_t outer_thread = {0}; /* TODO move this */

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
	inner_thread = thread_main_new();
	eventc_component_addsub(&outer_thread, inner_thread);

	/* Init components */
	eventc_component_initsub(&outer_thread);

	/* Start all components */
	eventc_component_startsub(&outer_thread);	

	/* Wait the main program on these threads */
	// TODO sort this out - we cannot wait on all threads to finish
	eventc_component_wait(inner_thread);

	/* Free components now - TODO need a clear function on each component */
	free(inner_thread);

	return 0;

}



