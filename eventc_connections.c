

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

static connection_pair connection_pair_list[MAX_CONNECTIONS] = {{0}};

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
	int item_added = 0;

	assert(EVENTC_IS_VALID_PTR(comp_1));
	assert(EVENTC_IS_VALID_PTR(comp_2));

	printf("%s: adding connection between %s inst:%d and %s inst:%d\n", __FUNCTION__, comp_1->comp_name, comp_1->instance_id, comp_2->comp_name, comp_2->instance_id);

	/* Look for duplicate destinations */
	for (i = 0; i < MAX_CONNECTIONS; i++)
	{
		if (connection_pair_list[i].in_use != 0)
		{
			if (eventc_connections_is_match(&connection_pair_list[i].comp_1, &connection_pair_list[i].comp_2, comp_1->instance_id, comp_2->comp_id))
			{
				printf("%s: Duplicate! Already in list sender %s inst:%d to %s inst: %d\n", __FUNCTION__, connection_pair_list[i].comp_1.comp_name, connection_pair_list[i].comp_1.instance_id, connection_pair_list[i].comp_2.comp_name, connection_pair_list[i].comp_2.instance_id); 
				assert(0);
			}
			else if (eventc_connections_is_match(&connection_pair_list[i].comp_1, &connection_pair_list[i].comp_2, comp_2->instance_id, comp_1->comp_id))
			{
				printf("%s: Duplicate! Already in list sender %s inst:%d to %s inst: %d\n", __FUNCTION__, connection_pair_list[i].comp_2.comp_name, connection_pair_list[i].comp_2.instance_id, connection_pair_list[i].comp_1.comp_name, connection_pair_list[i].comp_1.instance_id); 
				assert(0);
			}
		}
	}

	/* Find a free row and add */
	for (i = 0; i < MAX_CONNECTIONS; i++)
	{
		if (connection_pair_list[i].in_use == 0)
		{
			memcpy(&(connection_pair_list[i].comp_1), comp_1, sizeof(*comp_1));
			memcpy(&(connection_pair_list[i].comp_2), comp_2, sizeof(*comp_2));
			connection_pair_list[i].in_use = 2;
			item_added = 1;
			break;
		}
	}

	assert(item_added == 1);

}

mqd_t eventc_connections_find_receiver(
	comp_t * sender_details, 
	int dest_comp_id
)
{
	
	int i = 0; /* Loop counter */
	comp_t * receiver = NULL; /* The found row */

	printf("%s: sending from %s inst:%d to comp %d\n", __FUNCTION__, sender_details->comp_name, sender_details->instance_id, dest_comp_id);

	/* Look at items in use and find the destination */
	for (i = 0; i < MAX_CONNECTIONS; i++)
	{
		if (connection_pair_list[i].in_use != 0)
		{
			if (eventc_connections_is_match(&connection_pair_list[i].comp_1, &connection_pair_list[i].comp_2, sender_details->instance_id, dest_comp_id))
			{
				receiver = &(connection_pair_list[i].comp_2);
				break;
			}
			else if (eventc_connections_is_match(&connection_pair_list[i].comp_2, &connection_pair_list[i].comp_1, sender_details->instance_id, dest_comp_id))
			{
				receiver = &(connection_pair_list[i].comp_1);
				break;
			}
		}
	}
	
	assert(EVENTC_IS_VALID_PTR(receiver));
	
	printf("%s: connection found.  sending from %s inst:%d to %s inst:%d\n", __FUNCTION__, sender_details->comp_name, sender_details->instance_id, receiver->comp_name, receiver->instance_id);

	return receiver->queue_id;

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


