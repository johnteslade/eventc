env = Environment()   # Create an environmnet

env.Append(CCFLAGS = '-g')

env.Append(LIBS = [
	'rt', # For mq
	'pthread',
])

source_files = [
	"myprog.c",
	"Thread1.c",
	"Thread2.c",
]

env.Program(
	target = "eventc", 
	source = source_files
)
