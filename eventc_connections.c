

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

void eventc_connections_add(
	comp_t * comp_1, 
	comp_t * comp_2
)
{

	int i = 0; /* Loop counter */

	printf("%s: adding connection between %d and %d\n", __FUNCTION__, comp_1->comp_id, comp_2->comp_id);

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

	printf("%s: sending from %d to %d\n", __FUNCTION__, sender_details->comp_id, dest_comp_id);

	/* Look at items in use and find the destination */
	for (i = 0; i < MAX_CONNECTIONS; i++)
	{
		if (connection_pair_list[i].in_use != 0)
		{
			if ( (connection_pair_list[i].comp_1.comp_id == sender_details->comp_id) && 
				(connection_pair_list[i].comp_2.comp_id == dest_comp_id) )
			{
				return connection_pair_list[i].comp_2.queue_id;
			}
			else if ( (connection_pair_list[i].comp_2.comp_id == sender_details->comp_id) && 
				(connection_pair_list[i].comp_1.comp_id == dest_comp_id) )
			{
				return connection_pair_list[i].comp_1.queue_id;
			}
		}
	}

	assert(0);	
	return -1;

}



