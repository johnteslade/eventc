

/*

Event c defined connection list 

*/

#include <eventc_connections.h>
#include <eventc.h>

#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/***************************************/
// Types
/***************************************/

#define MAX_CONNECTIONS 10

/* Pair of components - use full struct here to prevent memory problems */
typedef struct
{
	bool in_use;
	comp_t comp_1;
	comp_t comp_2;
	eventc_mutator_function * mutator_1_2;
	eventc_mutator_function * mutator_2_1;
} connection_pair_t;

/***************************************/
// Local Vars
/***************************************/

static connection_pair_t connection_table[MAX_CONNECTIONS] = {{0}};

/***************************************/
// Static protypes
/***************************************/

static int eventc_connections_is_match(
	comp_t * comp_sender, 
	comp_t * comp_dest,
	int sender_instance_id,
	int dest_comp_id
);

static comp_t * find_reciever_in_table(
	eventc_mutator_function ** mutator_func, /*! Out: The mutator function for this connection */
	comp_t * sender, 
	int dest_comp_id
);

/***************************************/
// Public
/***************************************/


/* Creates a connection */
void eventc_connections_add(
	comp_t * comp_1, 
	comp_t * comp_2
)
{
	eventc_connections_add_with_mutate(comp_1, comp_2, NULL, NULL);
}

/* Creates a connection with mutation functions*/
void eventc_connections_add_with_mutate(
	comp_t * comp_1, 
	comp_t * comp_2,
	eventc_mutator_function * mutator_1_2,
	eventc_mutator_function * mutator_2_1
)
{
	int i = 0; /* Loop counter */
	bool item_added = false; /* Was item added */

	assert(EVENTC_IS_VALID_PTR(comp_1));
	assert(EVENTC_IS_VALID_PTR(comp_2));

	printf("%s: adding connection between %s inst:%d and %s inst:%d\n", __FUNCTION__, comp_1->comp_name, comp_1->instance_id, comp_2->comp_name, comp_2->instance_id);

	/* Look for duplicate destinations */
	for (i = 0; i < EVENTC_ARRAY_SIZE(connection_table); i++)
	{
		if (connection_table[i].in_use == true)
		{
			if (eventc_connections_is_match(&connection_table[i].comp_1, &connection_table[i].comp_2, comp_1->instance_id, comp_2->comp_id))
			{
				printf("%s: Duplicate! Already in list sender %s inst:%d to %s inst: %d\n", __FUNCTION__, connection_table[i].comp_1.comp_name, connection_table[i].comp_1.instance_id, connection_table[i].comp_2.comp_name, connection_table[i].comp_2.instance_id); 
				assert(0);
			}
			else if (eventc_connections_is_match(&connection_table[i].comp_1, &connection_table[i].comp_2, comp_2->instance_id, comp_1->comp_id))
			{
				printf("%s: Duplicate! Already in list sender %s inst:%d to %s inst: %d\n", __FUNCTION__, connection_table[i].comp_2.comp_name, connection_table[i].comp_2.instance_id, connection_table[i].comp_1.comp_name, connection_table[i].comp_1.instance_id); 
				assert(0);
			}
		}
	}

	/* Find a free row and add */
	for (i = 0; i < EVENTC_ARRAY_SIZE(connection_table); i++)
	{
		if (connection_table[i].in_use == false)
		{
			memcpy(&(connection_table[i].comp_1), comp_1, sizeof(*comp_1));
			memcpy(&(connection_table[i].comp_2), comp_2, sizeof(*comp_2));
			connection_table[i].in_use = true;
			connection_table[i].mutator_1_2 = mutator_1_2;
			connection_table[i].mutator_2_1 = mutator_2_1;
			item_added = true;
			break;
		}
	}

	assert(item_added == true);

}

/* Find the queue for a reciever */
mqd_t eventc_connections_find_receiver(
	eventc_mutator_function ** mutator_func, /*! Out: The mutator function for this connection */
	comp_t * sender, /*!< In: The sending of this message */ 
	int dest_comp_id, /*!< In: The expected destination */
	bool allow_loopback /*!< In: Is lookback (send to self) allowed on this search */
)
{
	comp_t * receiver = NULL; /* The found row */

	assert(EVENTC_IS_VALID_PTR(mutator_func));

	*mutator_func = NULL;

	printf("%s: sending from %s inst:%d to comp %d\n", __FUNCTION__, sender->comp_name, sender->instance_id, dest_comp_id);

	/* Check for sending to self */
	if (sender->comp_id == dest_comp_id)
	{
		printf("%s: Loopback to self\n", __FUNCTION__);
		assert(allow_loopback == true);
		receiver = sender;
	}
	else
	{
		receiver = find_reciever_in_table(mutator_func, sender, dest_comp_id);
	}

	assert(EVENTC_IS_VALID_PTR(receiver));
	
	printf("%s: connection found.  sending from %s inst:%d to %s inst:%d\n", __FUNCTION__, sender->comp_name, sender->instance_id, receiver->comp_name, receiver->instance_id);

	if (*mutator_func != NULL)
	{
		printf("%s: mutator func found\n", __FUNCTION__);
	}

	return receiver->queue_id;

}

/***************************************/
// Private
/***************************************/

/* Look for the receiver in the stored table */
// TODO improve with a bool return and the comp_t as double pointer
static comp_t * find_reciever_in_table(
	eventc_mutator_function ** mutator_func, /*! Out: The mutator function for this connection */
	comp_t * sender, 
	int dest_comp_id
)
{
	
	int i = 0; /* Loop counter */
	comp_t * receiver = NULL; /* The found row */

	assert(EVENTC_IS_VALID_PTR(mutator_func));

	/* Look at items in use and find the destination */
	for (i = 0; i < EVENTC_ARRAY_SIZE(connection_table); i++)
	{
		if (connection_table[i].in_use == true)
		{
			if (eventc_connections_is_match(&connection_table[i].comp_1, &connection_table[i].comp_2, sender->instance_id, dest_comp_id))
			{
				*mutator_func = connection_table[i].mutator_1_2;
				receiver = &(connection_table[i].comp_2);
				break;
			}
			else if (eventc_connections_is_match(&connection_table[i].comp_2, &connection_table[i].comp_1, sender->instance_id, dest_comp_id))
			{
				*mutator_func = connection_table[i].mutator_2_1;
				receiver = &(connection_table[i].comp_1);
				break;
			}
		}
	}

	return receiver;

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


