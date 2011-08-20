import sys
import re
import string
import datetime

# Returns the new template with replacements done
def template_replace(template, subs_dict):

	for key in subs_dict:
		template = template.replace("<%s>" % key, subs_dict[key])

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
template_call_func_h = open("eventc_call_func.template.h", 'r').read()
template_main = open("eventc_main.template.c", 'r').read()
template_main_switch = open("eventc_main_switch.template.c", 'r').read()

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
next_function_id = 0

# Find all functions that can be called externally
for m in function_re.finditer(component_header_file):

	# Increase function id
	next_function_id = next_function_id + 1

	# Create template subsitutions
	subs_dict = {}
	subs_dict['ORIG_NAME'] = m.group(1)
	subs_dict['NAME'] = "CALL_" + subs_dict['ORIG_NAME']
	subs_dict['INPUT_TYPE'] = m.group(4)
	subs_dict['FUNC_ID'] = "%d" % next_function_id
	subs_dict['COMP_ID'] = component_name

	# Create code for each function
	call_func_implemenations.append(template_replace(template_call_func, subs_dict))
	call_func_protos.append(template_replace(template_call_func_h, subs_dict))
	call_func_switch.append(template_replace(template_main_switch, subs_dict))

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

# Create template subsitutions
subs_dict = {}

subs_dict['MAIN_FUNC_NAME'] = component_main_func
subs_dict['COMP_START_FUNC'] = component_start_func
subs_dict['COMP_TYPE'] = component_type
subs_dict['COMP_ID'] = component_name
subs_dict['SWITCH_CONTENTS'] = "\n".join(call_func_switch)

out_c_file.write(template_replace(template_main, subs_dict))

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

