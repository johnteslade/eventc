/*

Public header file for eventc component

*/

#ifndef eventc_component_h
#define eventc_component_h

#include "eventc_types.h"

void eventc_component_init(comp_t * comp_details);

void eventc_component_start(comp_t * comp_details);

void eventc_component_wait(comp_t * comp_details);

mqd_t create_thread_q(char * name);

void eventc_component_addsub(comp_t * comp_details, comp_t * sub_comp);

void eventc_component_initsub(comp_t * comp_details);

#endif /* eventc_component_h */


