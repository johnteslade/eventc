/* The event c main header file */


#ifndef eventc_h
#define eventc_h

#include <mqueue.h>
#include "eventc_types.h"
#include "eventc_connections.h"

/* Functions */

mqd_t find_receiver_queue(comp_t * sender_details, int dest_comp_id);

#define CONCAT(x,y) x##y

#define EVENTC_FUNCTION_RET void

#define EVENTC_COMP_START void

#define EVENTC_COMP_END void

#define EVENTC_COMP_MAIN void *

#define EVENTC_IS_VALID_PTR(x) (x != NULL)

#define EVENTC_ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

#endif /* eventc_h */



