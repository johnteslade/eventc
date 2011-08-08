env = Environment()   # Create an environmnet

env.Append(CCFLAGS = '-g')

env.Append(LIBS = [
	'rt', # For mq
	'pthread',
])

env.Program(target = "eventc", source = ["myprog.c","Thread1.c"])
