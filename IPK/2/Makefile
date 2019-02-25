# #####################################
# Autor: Jan Jakub Kubik (xkubik32)
# VUT FIT 2BIA
# Date: 24.3.2018
# Description: Makefile for client
#			   and server
# #####################################

CC=g++																				
CFLAGS=-std=c++11 -static-libstdc++

all: ipk-mtrip 

# ######################################################################
ipk-mtrip: ipk-mtrip.o meter.o timer.o reflect.o
	$(CC) $(CFLAGS) -o ipk-mtrip ipk-mtrip.o meter.o timer.o reflect.o  -lm

# #####################################################################
ipk-mtrip.o: ipk-mtrip.cpp ipk-mtrip.h reflect.h timer.h
	$(CC) $(CFLAGS) -c ipk-mtrip.cpp 

meter.o: meter.cpp ipk-mtrip.h meter.h
	$(CC) $(CFLAGS) -c meter.cpp

ipk-mtrip_reflect.o: ipk-mtrip_reflect.cpp ipk-mtrip.h
	$(CC) $(CFLAGS) -c ipk-mtrip_reflect.cpp

reflect.o:	reflect.cpp ipk-mtrip.h reflect.h
	$(CC) $(CFLAGS) -c reflect.cpp

timer.o: timer.cpp timer.h
	$(CC) $(CFLAGS) -c timer.cpp

# #####################################################################
clean:
	rm *.o  ipk-mtrip

zip: *.cpp *.h Makefile 
	zip xkubik32.zip *.cpp *.h  Makefile dokumentace.pdf readme