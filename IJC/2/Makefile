CC=gcc
CPP=g++
CFLAGS=-O2 -std=c99 -g -Wall -pedantic
CPPFLAGS=-O2 -std=c++11 -g -Wall -pedantic

DYNAMIC_FILES=htab_bucket_count-dynamic.o hash_function-dynamic.o htab_clear-dynamic.o htab_find-dynamic.o \
				htab_foreach-dynamic.o htab_free-dynamic.o htab_init-dynamic.o \
				 htab_lookup_add-dynamic.o htab_move-dynamic.o htab_remove-dynamic.o htab_size-dynamic.o

STATIC_FILES=htab_bucket_count-static.o hash_function-static.o htab_clear-static.o htab_find-static.o \
				htab_foreach-static.o htab_free-static.o htab_init-static.o \
				htab_lookup_add-static.o htab_move-static.o htab_remove-static.o htab_size-static.o

all: tail Bpart


tail: tailc tail2c
	$(CC) $(CFLAGS) tailc -o tail
	$(CPP) $(CPPFLAGS) tail2c -o tail2

tailc: tail.c
	$(CC) $(CFLAGS) -c tail.c -o tailc

tail2c: tail2.cc
	$(CPP) $(CPPFLAGS) -c tail2.cc -o tail2c



Bpart: wordcount wordcount-dynamic


libhtab.so: $(DYNAMIC_FILES)
	$(CC) $(CFLAGS) -fPIC -shared -o libhtab.so  $(DYNAMIC_FILES)

libhtab.a: $(STATIC_FILES)
	ar crs libhtab.a  $(STATIC_FILES)

wordcount-dynamic: wrdcnt.o hash_function.o io.o libhtab.so
	$(CC) $(CFLAGS) -fPIC -o wordcount-dynamic  wrdcnt.o io.o -L. -lhtab

wordcount: wrdcnt.o hash_function.o io.o libhtab.a
	$(CC) $(CFLAGS) -o wordcount -static wrdcnt.o io.o -L. -lhtab


wrdcnt.o: wordcount.c htab.h
	$(CC) $(CFLAGS) -c wordcount.c  -o wrdcnt.o

io.o: io.c htab.h
	$(CC) $(CFLAGS) -c io.c 	-o io.o


htab_bucket_count-dynamic.o: htab_bucket_count.c htab.h
	$(CC) $(CFLAGS) -fPIC -c	htab_bucket_count.c 	-o htab_bucket_count-dynamic.o

hash_function-dynamic.o: hash_function.c htab.h
	$(CC) $(CFLAGS) -fPIC -c hash_function.c 	-o hash_function-dynamic.o

htab_clear-dynamic.o: htab_clear.c htab.h
	$(CC) $(CFLAGS) -fPIC -c 	htab_clear.c 	-o htab_clear-dynamic.o

htab_find-dynamic.o: htab_find.c htab.h
	$(CC) $(CFLAGS) -fPIC -c htab_find.c 	-o htab_find-dynamic.o

htab_foreach-dynamic.o: htab_foreach.c htab.h
	$(CC) $(CFLAGS) -fPIC -c htab_foreach.c	 -o htab_foreach-dynamic.o

htab_free-dynamic.o: htab_free.c htab.h
	$(CC) $(CFLAGS) -fPIC -c  htab_free.c  -o htab_free-dynamic.o

htab_init-dynamic.o: htab_init.c htab.h
	$(CC) $(CFLAGS) -fPIC -c htab_init.c  -o htab_init-dynamic.o

htab_lookup_add-dynamic.o: htab_lookup_add.c htab.h
	$(CC) $(CFLAGS) -fPIC -c htab_lookup_add.c	-o htab_lookup_add-dynamic.o

htab_move-dynamic.o: htab_move.c htab.h
	$(CC) $(CFLAGS) -fPIC -c htab_move.c  -o htab_move-dynamic.o

htab_remove-dynamic.o: htab_remove.c htab.h
	$(CC) $(CFLAGS) -fPIC -c htab_remove.c  -o htab_remove-dynamic.o

htab_size-dynamic.o: htab_size.c htab.h
	$(CC) $(CFLAGS) -fPIC -c htab_size.c  -o htab_size-dynamic.o

# compile for static library
htab_bucket_count-static.o: htab_bucket_count.c htab.h
	$(CC) $(CFLAGS) -c	htab_bucket_count.c 	-o htab_bucket_count-static.o

hash_function-static.o: hash_function.c htab.h
	$(CC) $(CFLAGS) -c hash_function.c 	-o hash_function-static.o

htab_clear-static.o: htab_clear.c htab.h
	$(CC) $(CFLAGS) -c 	htab_clear.c 	-o htab_clear-static.o

htab_find-static.o: htab_find.c htab.h
	$(CC) $(CFLAGS) -c htab_find.c 	-o htab_find-static.o

htab_foreach-static.o: htab_foreach.c htab.h
	$(CC) $(CFLAGS) -c htab_foreach.c	 -o htab_foreach-static.o

htab_free-static.o: htab_free.c htab.h
	$(CC) $(CFLAGS) -c  htab_free.c  -o htab_free-static.o

htab_init-static.o: htab_init.c htab.h
	$(CC) $(CFLAGS) -c htab_init.c  -o htab_init-static.o

htab_lookup_add-static.o: htab_lookup_add.c htab.h
	$(CC) $(CFLAGS) -c htab_lookup_add.c	-o htab_lookup_add-static.o

htab_move-static.o: htab_move.c htab.h
	$(CC) $(CFLAGS) -c htab_move.c  -o htab_move-static.o

htab_remove-static.o: htab_remove.c htab.h
	$(CC) $(CFLAGS) -c htab_remove.c  -o htab_remove-static.o

htab_size-static.o: htab_size.c htab.h
	$(CC) $(CFLAGS) -c htab_size.c  -o htab_size-static.o



clean:
	rm -f *.o tail tail2 wordcount wordcount-dynamic libhtab.a libhtab.so tailc tail2c



zip: *.c *.cc *.h Makefile
	zip xkubik32.zip *.c *.cc *.h Makefile
