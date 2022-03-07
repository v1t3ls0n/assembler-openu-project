
	

all:   data.h preProcessor.c variables.c errors.c helpers.c operations.c tables.c encode.c memory.c firstRun.c compiler.c
	gcc -ansi -Wall -pedantic preProcessor.c  variables.c errors.c helpers.c operations.c tables.c encode.c memory.c firstRun.c compiler.c -o compiler -lm
# all: data.h  errors.c 
# 	gcc -ansi -Wall -pedantic data.h  errors.c -o errors -lm

# all: data.h  encode.c 
# 	gcc -ansi -Wall -pedantic data.h encode.c -o encode -lm

# variables: variables.h variables.c
# 	gcc -ansi -Wall -pedantic variables.h variables.c  -o variables -lm


clean:
	rm *.o all