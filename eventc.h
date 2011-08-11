/* The event c main header file */


#ifndef eventc_h
#define eventc_h

#include <mqueue.h>
#include "eventc_types.h"
#include "eventc_connections.h"

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
	recv_queue = eventc_connections_find_receiver(comp_details, COMP_ID); \
\
	ret = mq_send(recv_queue, (const char *)&tmp_struct, sizeof(tmp_struct), 0); \
	assert(ret == 0); \
	\
}

#endif /* eventc_h */



