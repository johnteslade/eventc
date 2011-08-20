/*

Public header file for eventc funcqueue

*/

#ifndef eventc_funcqueue_h
#define eventc_funcqueue_h

#include "eventc_types.h"

void eventc_funcqueue_add(
	int function_id,
	char * function_name,
	int comp_id,
	comp_t * sender_details, 
	void * data,
	int data_len
);

#endif /* eventc_funcqueue_h */

