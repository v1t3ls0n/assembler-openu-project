
all:  data.h  compiler.c helpers.c encode.c tables.c  commands.c
	gcc -ansi -Wall -pedantic  compiler.c data.h  errors.c tables.c commands.c  helpers.c encode.c  -o compiler -lm

clean:
	rm *.o all
	ls