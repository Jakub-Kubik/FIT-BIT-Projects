CC = g++
CPPFLAGS = -std=c++14 -Wall -Wextra -Werror

all: adjustnitrogen

adjustnitrogen: main.o
	$(CC) $(CPPFLAGS) -o adjustnitrogen main.o

main.o: main.cpp main.h
	$(CC) $(CPPFLAGS) -c main.cpp

run:
	./adjustnitrogen -v
	./adjustnitrogen -s

clean:
	rm *.o adjustnitrogen

zip: *.cpp *.h doc.pdf Makefile
	zip 03_xkapic02_xkubik32.zip *.cpp *.h doc.pdf Makefile
