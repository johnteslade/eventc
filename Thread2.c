


static void * thread_2(void * start_ptr)
{

	
	printf("%s started\n", __FUNCTION__);

	// Send message 
	{
		thread_1_input send_struct;
		send_struct.int1 = 1;
		send_struct.int2 = 2;
		thread_1_function_call(&send_struct);
	}	

	while (1)
	{

		thread_2_input * recv_data;
		ssize_t bytes_read;

		/* receive the message */
		bytes_read = mq_receive(thread2_mq, (void *)&recv_data, sizeof(recv_data), NULL);
		assert(bytes_read >= 0);

		printf("%s: mq recv\n", __FUNCTION__);

		thread_2_function(recv_data);

		free(recv_data);

	}

}

static void thread_2_function(thread_2_input * struct_in)
{

	printf("%s: got %d %d %d\n", __FUNCTION__, struct_in->char1, struct_in->int1, struct_in->int2);
	
	// Send message 
	{
		thread_1_input send_struct;
		send_struct.int1 = struct_in->int1 + 1;
		send_struct.int2 = struct_in->int2 - 1;
		thread_1_function_call(&send_struct);
	}

}

static void thread_2_function_call(thread_2_input * struct_in)
{

	thread_2_input * tmp_struct;
	int ret;

	tmp_struct = malloc(sizeof(*tmp_struct));
	memcpy(tmp_struct, struct_in, sizeof(tmp_struct));

	ret = mq_send(thread2_mq, &tmp_struct, sizeof(tmp_struct), 0);
//	printf("%s %d: errno = %d - %s\n", __FUNCTION__, __LINE__, errno, strerror(errno));
	assert(ret == 0);
	
}

