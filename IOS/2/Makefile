
# --------------
# -- Makefile --
# --------------
#
# *  ******  VUT FIT 1BIT BRNO ********************************
#
# 2.DOMOACI UKOL Z IOS ---------------------------------------
#
# Datum:		 1.5.2017
# Prelozene: gcc version 6.1.1 20160510 (Red Hat 6.1.1-2)
# Autor:     Jakub Kubik (xkubik32@stud.fit.vutbr.cz)
# -----------------------------------------------------------------

CC=gcc																							# default compiler
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic 	# flags for compiler

all: proj2

proj2: proj2.o
	$(CC) $(CFLAGS) proj2.o -o proj2 -lpthread

proj2.o: proj2.c proj2.h
	$(CC) $(CFLAGS) -c proj2.c	-o proj2.o


clean:
	rm -f *.o proj2

zip:
	zip proj2.zip *.c *.h Makefile
