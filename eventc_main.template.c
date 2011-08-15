void * <MAIN_FUNC_NAME>(void * start_ptr)
{

	<COMP_TYPE> * local_attr = NULL;
	
	printf("%s: top\n", __FUNCTION__);
	
	assert(start_ptr != NULL);

	local_attr = (<COMP_TYPE>*)start_ptr; 

	printf("%s started\n", __FUNCTION__);

	/* Check this is the correct thread data */
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
		assert(call_struct->comp_id == <COMP_ID>);

		printf("%s: mq recv\n", __FUNCTION__);

		if (call_struct->data == NULL)
		{
			<COMP_START_FUNC>(local_attr);
		}
		else
		{

			switch (call_struct->function_id)
			{
				<SWITCH_CONTENTS>
				default:
					assert(0);
			}

			free(call_struct->data);
		}
		
		free(call_struct);
	
	}

	free(local_attr);

}
