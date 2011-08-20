import sys
import re
import string
import datetime

def gen_call_func(template, name, input_type, comp_id, func_id):

	template = template.replace("<NAME>", name)
	template = template.replace("<INPUT_TYPE>", input_type)
	template = template.replace("<COMP_ID>", comp_id)
	template = template.replace("<FUNC_ID>", "%d" % func_id)

	return template

def gen_call_proto(name, input_type):
	
	return """void %s(comp_t *, %s *);""" % (name, input_type)
	
def gen_call_switch(name, input_type, func_id):
	
	return """case %d: %s(local_attr, (%s *)call_struct->data); break;""" % (func_id, name, input_type)

def gen_main(template, name, start_func, comp_type, comp_id, switch_contents):

	template = template.replace("<MAIN_FUNC_NAME>", name)
	template = template.replace("<COMP_TYPE>", comp_type)
	template = template.replace("<COMP_START_FUNC>", start_func)
	template = template.replace("<COMP_ID>", comp_id)
	template = template.replace("<SWITCH_CONTENTS>", switch_contents)

	return template	

header_file = sys.argv[1]

# Create the output file names - TODO reenable this
#base_file_name = re.sub(".h$", "", header_file) 
#output_c_file = base_file_name + "_autogen.c"
#output_h_file = base_file_name + "_autogen.h"

output_c_file = sys.argv[2]
output_h_file = sys.argv[3]

# Open header and get contents
component_header_file = open(header_file, 'r').read()

# Open templates
template_call_func = open("eventc_call_func.template.c", 'r').read()
template_main = open("eventc_main.template.c", 'r').read()

# Find the name of the component
component_name = re.search("EVENTC\s*:\s*comp=(\w*)", component_header_file).group(1)
component_type = re.search("EVENTC\s*:\s*comp_type=(\w*)", component_header_file).group(1)
component_main_func = re.search("EVENTC_COMP_MAIN\s*(\w*)\s*\(", component_header_file).group(1)
component_start_func = re.search("EVENTC_COMP_START\s*(\w*)\s*\(", component_header_file).group(1)

# Regex to find the call functions
function_re = re.compile("""EVENTC_FUNCTION_RET\s*(\w*)\s*\(\s*(\w*)\s*\*\s*(\w*)\s*,\s*(\w*)\s*\*\s*(\w*)\s*\)\s*;""")

call_func_implemenations = []
call_func_protos = []
call_func_switch = []
function_id = 0

for m in function_re.finditer(component_header_file):

	function_name_orig = m.group(1)
	function_name = "CALL_" + function_name_orig
	function_input_type = m.group(4)
	function_id = function_id + 1

	call_func_implemenations.append(gen_call_func(template_call_func, function_name, function_input_type, component_name, function_id))
	call_func_protos.append(gen_call_proto(function_name, function_input_type))
	call_func_switch.append(gen_call_switch(function_name_orig, function_input_type, function_id))

#print call_func_implemenations
#print call_func_protos
#print call_func_switch

# Open the output c file and spit out data
out_c_file = open(output_c_file, 'w')

out_c_file.write("//Autogen output : %s\n\n" % datetime.datetime.now().strftime("%Y-%m-%d %H:%M")) 

# Include the header files
included_headers = [
	header_file,
	"eventc.h",
	"eventc_funcqueue.h",
	"model.h",
	"stdio.h",
	"stdlib.h",
	"string.h",
	"assert.h",
	"unistd.h",
]	

for header in included_headers:
	out_c_file.write("""#include "%s"\n\n""" % header) 

# Main func
out_c_file.write("\n\n///////////////////////////////\n// MAIN FUNC\n///////////////////////////////\n\n")
out_c_file.write(gen_main(template_main, component_main_func, component_start_func, component_type, component_name, "\n".join(call_func_switch)))

# Call funcs
out_c_file.write("\n\n///////////////////////////////\n// CALL FUNCS\n///////////////////////////////\n\n")
out_c_file.write("\n\n".join(call_func_implemenations))


# Output the header file
out_h_file = open(output_h_file, 'w')

# TODO need to add guards

out_h_file.write("//Autogen output : %s\n\n" % datetime.datetime.now().strftime("%Y-%m-%d %H:%M")) 

out_h_file.write("\n\n///////////////////////////////\n// CALL FUNCS\n///////////////////////////////\n\n")
out_h_file.write("\n\n".join(call_func_protos))

print "Done.  Created %s and %s" % (output_c_file, output_h_file)

