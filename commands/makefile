
all:  data.h  helpers.c encode.c tables.c  commands.c compiler.c
	gcc -ansi -Wall -pedantic  data.h errors.c tables.c commands.c  helpers.c encode.c compiler.c -o compiler -lm

clean:
	rm *.o all
	ls