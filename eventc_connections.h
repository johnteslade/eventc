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

mqd_t eventc_connections_find_receiver(
	comp_t * sender_details, 
	int dest_comp_id,
	bool allow_loopback
);

#endif /* eventc_connections_h */


