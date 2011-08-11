/* The event c types file */


#ifndef eventc_types_h
#define eventc_types_h

#include <mqueue.h>
#include <pthread.h>

typedef void * (*pthread_start_routine)(void *);

typedef struct {
	int comp_id;
	int instance_id;
	mqd_t queue_id;
	pthread_t thread_id;
	pthread_start_routine start_func;
} comp_t;

#define EVENTC_COMP_STRUCT_START comp_t comp_details; /* Required as first element of compeont structs.  Allows all components to contain hidden elements. */

/* TODO create macros to access parts of the structure - based on the name of the first element */

#define EVENTC_COMPONENT(x) (x)->comp_details.comp_id
#define EVENTC_SET_COMPONENT(x, y) EVENTC_COMPONENT(x) = (y);

#define EVENTC_START_ROUTINE(x) (x)->comp_details.start_func
#define EVENTC_SET_START_ROUTINE(x, y) EVENTC_START_ROUTINE(x) = (y);


#endif /* eventc_types_h */



