/* Defines the public prototypes */


#ifndef eventc_pub_proto_h
#define eventc_pub_proto_h


#define FUNC(name, input) void call_##name(input *);
#include EVENTC_PRIVATE_HEADER


#endif /* eventc_pub_proto_h */



