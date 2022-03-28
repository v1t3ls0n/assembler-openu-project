
	

all:   data.h preProccesor.c  sharedStates.c  exportFiles.c  errors.c utils.c helpers.c parse.c operations.c tables.c memory.c firstRun.c secondRun.c compiler.c
	gcc -ansi -Wall -pedantic -g data.h preProccesor.c sharedStates.c   exportFiles.c errors.c utils.c helpers.c parse.c operations.c tables.c  memory.c secondRun.c firstRun.c compiler.c -o compiler -lm