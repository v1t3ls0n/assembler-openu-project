
all:  data.h  helpers.c encode.c table.c  commands.c  
	gcc -ansi -Wall -pedantic  data.h table.c commands.c  helpers.c encode.c -o compiler -lm

clean:
	rm *.o all
	ls