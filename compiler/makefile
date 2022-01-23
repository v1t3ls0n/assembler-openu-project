
all:  data.h table.h compiler.c helpers.c encode.c tables.c commands.c parse.c
	gcc -ansi -Wall -pedantic data.h errors.c tables.c commands.c helpers.c encode.c parse.c compiler.c -o compiler -lm

# clean:
# 	rm *.o all
# 	ls