
CC=gcc																							# default compiler
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic 	# flags for compiler

all: 
	$(CC) $(CFLAGS) proj2.c -o proj2 -lm

clean:
	rm -f  proj2 proj2.o

zip:
	zip proj2.zip *.c *.h Makefile