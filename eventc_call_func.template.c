void <NAME>(comp_t * comp_details, <INPUT_TYPE> * struct_in)
{ 

	// TODO create another function will the bulk of this code in it - only variables are comp_details, input type name and size

	<INPUT_TYPE> * tmp_struct = NULL; 
	eventc_call_t * call_struct = NULL;
	int ret = -1; 
	mqd_t recv_queue; 

	call_struct = malloc(sizeof(*call_struct));
	assert(call_struct != NULL);

	tmp_struct = malloc(sizeof(*tmp_struct)); 
	assert(tmp_struct != NULL); 
	memset(tmp_struct, 0x00, sizeof(*tmp_struct)); 
	memcpy(tmp_struct, struct_in, sizeof(*tmp_struct)); 

	recv_queue = eventc_connections_find_receiver(comp_details, <COMP_ID>); 

	call_struct->function_id = <FUNC_ID>;
	call_struct->comp_id = <COMP_ID>;
	call_struct->data = (void *)tmp_struct;

	ret = mq_send(recv_queue, (const char *)&call_struct, sizeof(call_struct), 0); 
	assert(ret == 0); 
	
}
