
	

all:   data.h preProcessor.c variables.c errors.c helpers.c operations.c tables.c encode.c memory.c firstRun.c compiler.c
	gcc -ansi -Wall -pedantic preProcessor.c  variables.c errors.c helpers.c operations.c tables.c encode.c memory.c firstRun.c compiler.c -o compiler -lm
