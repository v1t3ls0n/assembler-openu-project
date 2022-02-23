
all:   data.h  variables.c errors.c helpers.c commands.c tables.c encode.c memory.c parse.c compiler.c
	gcc -ansi -Wall -pedantic variables.c errors.c helpers.c commands.c tables.c encode.c memory.c parse.c compiler.c -o compiler -lm
# all: data.h  errors.c 
# 	gcc -ansi -Wall -pedantic data.h  errors.c -o errors -lm

# all: data.h  encode.c 
# 	gcc -ansi -Wall -pedantic data.h encode.c -o encode -lm

# variables: variables.h variables.c
# 	gcc -ansi -Wall -pedantic variables.h variables.c  -o variables -lm


# clean:
# 	rm *.o all
# 	# ls