/* The event c main header file */


#ifndef eventc_h
#define eventc_h

#include <mqueue.h>

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

/* Functions */

mqd_t find_receiver_queue(comp_t * sender_details, int dest_comp_id);

#define CONCAT(x,y) x##y


#define CALL_FUNCTION_FUNC_PROTO(NAME, INPUT_TYPE) void NAME(comp_t * comp_details, INPUT_TYPE * struct_in)

#define CALL_FUNCTION_IMPLEMENTATION(NAME, INPUT_TYPE, COMP_ID) CALL_FUNCTION_FUNC_PROTO(NAME, INPUT_TYPE) \
{ \
\
	INPUT_TYPE * tmp_struct = NULL; \
	int ret = -1; \
	mqd_t recv_queue; \
\
	tmp_struct = malloc(sizeof(*tmp_struct)); \
	memset(tmp_struct, 0x00, sizeof(*tmp_struct)); \
	assert(tmp_struct != NULL); \
	memcpy(tmp_struct, struct_in, sizeof(tmp_struct)); \
\
	recv_queue = find_receiver_queue(comp_details, COMP_ID); \
\
	ret = mq_send(recv_queue, (const char *)&tmp_struct, sizeof(tmp_struct), 0); \
	assert(ret == 0); \
	\
}

#endif /* eventc_h */



