/* Functions for the main thread call */

#include "eventc_main.h"
#include "eventc_timed.h"
#include "eventc_component.h"
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

/***************************************/
// Local Vars
/***************************************/

static bool eventc_init = false; /*!< Has eventc been initialised */

/***************************************/
// Static protypes
/***************************************/

/***************************************/
// Public
/***************************************/

/**
 * Initialisation for eventc main
 *
 * @return None
 */
void eventc_main_init(
	void
)
{
	/* Only allow this to be called once */
	assert(eventc_init == false);
	eventc_init = true;

	/* Start the timed thread   TODO could be in a better place - eventc_timed.c? */
	{
		int s;
		pthread_attr_t attr;
		pthread_t thread_id;

		s = pthread_attr_init(&attr);

		/* Create the new thread for this component */
		s = pthread_create(&thread_id, &attr, eventc_timed_start, NULL);
		assert(s == 0);
	}

}

/**
 * Start eventc and waits for exit
 *
 * @return None
 */
void eventc_main_go(
	comp_t * inner_component /*!< The inner component to start */ 
)
{

	comp_t outer_component = {0}; /*!< Outer component needed to hold the sub components - has no other use */

	/* Add it as a sub component */
	eventc_component_addsub(&outer_component, inner_component);

	/* Init component */
	eventc_component_initsub(&outer_component);

	/* Start */
	eventc_component_startsub(&outer_component);	

	/* Wait the main program on the inner component */
	eventc_component_wait(inner_component);

}


