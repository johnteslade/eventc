/*

Public header file for eventc connections

*/

#ifndef eventc_connections_h
#define eventc_connections_h

#include "eventc_types.h"

#include <stdbool.h>

void eventc_connections_add(
	comp_t * comp_1, 
	comp_t * comp_2
);

void eventc_connections_add_with_mutate(
	comp_t * comp_1, 
	comp_t * comp_2,
	eventc_mutator_function * mutator_1_2,
	eventc_mutator_function * mutator_2_1
);

mqd_t eventc_connections_find_receiver(
	eventc_mutator_function ** mutator_func, /*! Out: The mutator function for this connection */
	comp_t * sender_details, 
	int dest_comp_id,
	bool allow_loopback
);

#endif /* eventc_connections_h */


