# @description: MAKEFILE for first project to IPK
# @author Jan Jakub Kubik (xkubik32)
# @date: 12.3.2018

CC=gcc 																				
CFLAGS=-g -std=c99 -pedantic -Wall -Wextra -O2

all: ipk-client ipk-server


ipk-client: client.o client_arguments.o
	$(CC) $(CFLAGS) -o ipk-client client.o  client_arguments.o -lm

ipk-server: server.o server_arguments.o 
		$(CC) $(CFLAGS) -o ipk-server server.o  server_arguments.o -lm


client.o: client.c client.h client_arguments.c communication.h
	$(CC) $(CFLAGS) -c client.c

client_arguments.o: client_arguments.c  client.h communication.h
	$(CC) $(CFLAGS) -c client_arguments.c

server.o: server.c server.h communication.h communication.h
	$(CC) $(CFLAGS) -c server.c

server_arguments.o: server_arguments.c server.h communication.h
	$(CC) $(CFLAGS) -c server_arguments.c


clean:
	rm *.o ipk-client ipk-server


zip: *.c *.h Makefile
	zip xkubik32.zip *.c *.h Makefile dokumentace.pdf readme