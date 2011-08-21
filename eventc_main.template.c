void * <MAIN_FUNC_NAME>(void * start_ptr)
{

	<COMP_TYPE> * local_attr = NULL;
	
	printf("%s: top\n", __FUNCTION__);
	
	assert(EVENTC_IS_VALID_PTR(start_ptr));

	local_attr = (<COMP_TYPE>*)start_ptr; 

	printf("%s started\n", __FUNCTION__);

	/* Check this is the correct stucture type */
	EVENTC_ASSERT_CORRECT_STRUCT(local_attr->comp_details, EVENTC_STRUCT_comp_t);
	assert(EVENTC_COMPONENT(local_attr) == <COMP_ID>);

	while (1)
	{

		eventc_call_t * call_struct = NULL;
		ssize_t bytes_read;

		/* receive the message */
		bytes_read = mq_receive(local_attr->comp_details.queue_id, (void *)&call_struct, sizeof(call_struct), NULL);

//		printf("%s %d: errno = %d - %s\n", __FUNCTION__, __LINE__, errno, strerror(errno));
		
		assert(bytes_read >= 0);
		assert(bytes_read == sizeof(call_struct));
		EVENTC_ASSERT_CORRECT_STRUCT(*call_struct, EVENTC_STRUCT_call_t);
		assert(call_struct->comp_id == <COMP_ID>);

//		printf("%s: mq recv\n", __FUNCTION__);

		/* Look at the type of the call */
		switch (call_struct->call_type)
		{
			
			case EVENTC_START:
				<COMP_START_FUNC>(local_attr);
				break;

			
			case EVENTC_DATA_CALL:

				switch (call_struct->function_id)
				{
					<SWITCH_CONTENTS>
					default:
						assert(0);
				}

				free(call_struct->data);
				break;
		}
		
		free(call_struct);
	
	}

	free(local_attr);

}
