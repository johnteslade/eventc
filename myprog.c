

#include <stdlib.h>

#include "eventc.h"
#include "eventc_connections.h"
#include "eventc_component.h"

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
	
	/* Create thread 1 */
	{
		thread_1 = thread_1_new();
		eventc_component_init(thread_1);	
	}

	/* Create thread 2 */
	{
		thread_2 = thread_2_new();
		eventc_component_init(thread_2);	
	}

	/* Create thread 3 */
	{
		thread_3 = thread_2_new();
		eventc_component_init(thread_3);	
	}

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



