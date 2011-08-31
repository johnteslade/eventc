/* The event c types file */


#ifndef eventc_types_h
#define eventc_types_h

#include <mqueue.h>
#include <pthread.h>
#include <stdbool.h>

/* Structures are often cast between void * and back.  Guards and type fields are added to check the structure type of a recieved value */

enum EVENTC_STRUCTS
{
	EVENTC_STRUCT_comp_t,
	EVENTC_STRUCT_call_t,
	EVENTC_STRUCT_timed_call_t
};

#define EVENTC_STRUCT_START int eventc_guard; int eventc_struct_type;

#define EVENTC_GUARD_VALUE (0xECECEC)

#define EVENTC_INIT_STRUCT(x, y) (x).eventc_guard = EVENTC_GUARD_VALUE; (x).eventc_struct_type = (y);

#define EVENTC_ASSERT_CORRECT_STRUCT(x, y) assert((x).eventc_guard == EVENTC_GUARD_VALUE); assert((x).eventc_struct_type == (y));

/* Type of call being made */
typedef enum EVENTC_CALL
{
	EVENTC_DATA_CALL,
	EVENTC_START,
	EVENTC_END
} EVENTC_CALL_E;

typedef void * (*pthread_start_routine)(void *);

/* Sub component list */
typedef struct {
	void ** components;
	int count;
} eventc_subcomponents_t;


/* Base component struct */
typedef struct {
	EVENTC_STRUCT_START
	bool init; /* TODO initilaise this and check throughout */
	int comp_id;
	char * comp_name;
	int instance_id;
	mqd_t queue_id;
	pthread_t thread_id;
	pthread_start_routine main_func;
	eventc_subcomponents_t subcomponents;
} comp_t;

/* Type used to send a call to another component */
typedef struct {
	EVENTC_STRUCT_START
	EVENTC_CALL_E call_type;
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
	EVENTC_INIT_STRUCT((x)->comp_details, EVENTC_STRUCT_comp_t) \
	(x)->comp_details.comp_id = (in_comp_id); \
	(x)->comp_details.comp_name = (in_comp_name); \
	(x)->comp_details.main_func = (in_main_func);  \
	(x)->comp_details.subcomponents.components = NULL; \
	(x)->comp_details.subcomponents.count = 0;
	

#endif /* eventc_types_h */



