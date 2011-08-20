

/*

Event c defined connection list 

*/

#include "eventc_connections.h"
#include "eventc.h"

#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define MAX_CONNECTIONS 10

/* Pair of components - use full struct here to prevent memory problems */
typedef struct
{
	int in_use;
	comp_t comp_1;
	comp_t comp_2;
} connection_pair;

static connection_pair connection_pair_list[MAX_CONNECTIONS] = {0};

static int eventc_connections_is_match(
	comp_t * comp_sender, 
	comp_t * comp_dest,
	int sender_instance_id,
	int dest_comp_id
);

void eventc_connections_add(
	comp_t * comp_1, 
	comp_t * comp_2
)
{

	int i = 0; /* Loop counter */

	assert(EVENTC_IS_VALID_PTR(comp_1));
	assert(EVENTC_IS_VALID_PTR(comp_2));

	printf("%s: adding connection between %d (comp %d) and %d (comp %d)\n", __FUNCTION__, comp_1->instance_id, comp_1->comp_id, comp_2->instance_id, comp_2->comp_id);

	/* TODO look for multiple matches */

	for (i = 0; i < MAX_CONNECTIONS; i++)
	{
		if (connection_pair_list[i].in_use == 0)
		{
			memcpy(&(connection_pair_list[i].comp_1), comp_1, sizeof(*comp_1));
			memcpy(&(connection_pair_list[i].comp_2), comp_2, sizeof(*comp_2));
			connection_pair_list[i].in_use = 2;
			break;
		}
	}

}

mqd_t eventc_connections_find_receiver(
	comp_t * sender_details, 
	int dest_comp_id
)
{
	
	int i = 0; /* Loop counter */

	printf("%s: sending from %d (comp %d) to comp %d\n", __FUNCTION__, sender_details->instance_id, sender_details->comp_id, dest_comp_id);

	/* Look at items in use and find the destination */
	for (i = 0; i < MAX_CONNECTIONS; i++)
	{
		if (connection_pair_list[i].in_use != 0)
		{
			if (eventc_connections_is_match(&connection_pair_list[i].comp_1, &connection_pair_list[i].comp_2, sender_details->instance_id, dest_comp_id))
			{
				return connection_pair_list[i].comp_2.queue_id;
			}
			else if (eventc_connections_is_match(&connection_pair_list[i].comp_2, &connection_pair_list[i].comp_1, sender_details->instance_id, dest_comp_id))
			{
				return connection_pair_list[i].comp_1.queue_id;
			}
		}
	}

	assert(0);	
	return -1;

}

/* Detects if this component is a match to the current row */
static int eventc_connections_is_match(
	comp_t * comp_sender, 
	comp_t * comp_dest,
	int sender_instance_id,
	int dest_comp_id
)
{
	return ( (comp_sender->instance_id == sender_instance_id) && (comp_dest->comp_id == dest_comp_id) );
}


