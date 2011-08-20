/* The event c types file */


#ifndef eventc_types_h
#define eventc_types_h

#include <mqueue.h>
#include <pthread.h>

typedef void * (*pthread_start_routine)(void *);

typedef struct {
	int comp_id;
	char * comp_name;
	int instance_id;
	mqd_t queue_id;
	pthread_t thread_id;
	pthread_start_routine main_func;
} comp_t;


/* Type used to send a call to another component */
typedef struct {
	int function_id;
	int comp_id;
	void * data;
} eventc_call_t;

#define EVENTC_COMP_STRUCT_START comp_t comp_details; /* Required as first element of compeont structs.  Allows all components to contain hidden elements. */

/* TODO create macros to access parts of the structure - based on the name of the first element */

#define EVENTC_COMPONENT(x) (x)->comp_details.comp_id
#define EVENTC_SET_COMPONENT(x, y) EVENTC_COMPONENT(x) = (y);

#define EVENTC_MAIN_ROUTINE(x) (x)->comp_details.main_func
#define EVENTC_SET_MAIN_ROUTINE(x, y) EVENTC_MAIN_ROUTINE(x) = (y);

#define EVENTC_SET_COMP_T(x, in_comp_id, in_comp_name, in_main_func) \
	(x)->comp_details.comp_id = (in_comp_id); \
	(x)->comp_details.comp_name = (in_comp_name); \
	(x)->comp_details.main_func = (in_main_func); 
	

#endif /* eventc_types_h */



