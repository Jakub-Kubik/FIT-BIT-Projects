#
# Simple Makefile
#

CPP=g++
CPPFLAGS=-std=c++14 -Wall -Wextra -pedantic
LDFLAGS='-static-libstdc++'
CFLAGS   = pkg-config --cflags openssl
LDLIBS    = pkg-config --libs openssl


FEEDREADER_FILES=	src/main.cpp src/include/main.h\
					src/ArgumentsParser.cpp  src/include/ArgumentsParser.h\
					src/FeedfileParser.cpp src/include/FeedfileParser.h \
					src/Client.cpp src/include/Client.h\
					src/FeedParser.cpp src/include/FeedParser.h
			   		


all: feedreader 

feedreader: src/main.cpp $(FEEDREADER_FILES)
	$(CPP) $(CPPFLAGS)  $(FEEDREADER_FILES) $(LDFLAGS)  -lssl -lcrypto -lxml2 `xml2-config --cflags` `xml2-config --libs`  -o feedreader


clean: 
	rm -f *.o feedreader

test:
	python3 tests/test.py
	pkill -9 python3


zip: src test Makefile
	zip xkubik32.zip src/*  tests/* Makefile Readme.md LICENSE doc/*
