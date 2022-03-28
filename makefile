
all:  compiler.c preProccesor.c  sharedStates.c  exportFiles.c  errors.c utils.c helpers.c parse.c operations.c tables.c memory.c firstRun.c secondRun.c 
	gcc -ansi -Wall -pedantic -g  compiler.c preProccesor.c sharedStates.c  exportFiles.c errors.c utils.c helpers.c parse.c operations.c tables.c  memory.c secondRun.c firstRun.c  -o compiler -lm
