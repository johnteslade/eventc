

#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

#include "eventc.h"
#include "eventc_connections.h"
#include "eventc_component.h"
#include "eventc_timed.h"
#include "eventc_main.h"

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

	/* Initialise event c */
	eventc_main_init();

	/* Create thread 1 */
	inner_thread = thread_main_new();
	
	eventc_main_go(inner_thread);

	/* Free components now */
	free(inner_thread);

	return 0;

}



