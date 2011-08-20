void <NAME>(
	comp_t * sender_details, 
	<INPUT_TYPE> * struct_in
)
{ 
	eventc_funcqueue_add(<FUNC_ID>, "<ORIG_NAME>", <COMP_ID>, sender_details, struct_in, sizeof(*struct_in));
}
