/*

Public header file for eventc timed

*/

#ifndef eventc_timed_h
#define eventc_timed_h

#include "eventc_types.h"

/* Type for a timed call */
typedef struct {
	EVENTC_STRUCT_START
	int secs;
	long nsecs;
	mqd_t dest_queue_id;
	eventc_call_t * event_call;
} timed_event_call_t;

mqd_t eventc_timed_get_q(void);

void * eventc_timed_start(void * dummy);

#endif /* eventc_timed_h */


