
all: commands.h table.h  table.c  commands.c 
	gcc -ansi -Wall -pedantic table.h commands.h table.c commands.c -o compiler -lm

clean:
	rm *.o all
	ls