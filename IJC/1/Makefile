CC=gcc																					
CFLAGS=-g -std=c99 -pedantic -Wall -Wextra -O2	

all: primes  steg-decode primes-i


primes: error.o eratosthenes.o primes.o
	$(CC) $(CFLAGS) -o primes error.o  eratosthenes.o primes.o -lm		# zlinkovanie vch .o suborov

primes-i: primes-i.o eratosthenes-i.o error.o
	$(CC) $(CFLAGS) -o primes-i primes-i.o eratosthenes.o error.o -lm

steg-decode: steg-decode.o ppm.o error.o eratosthenes.o    
	$(CC) $(CFLAGS) -o steg-decode  steg-decode.o ppm.o error.o eratosthenes.o -lm


primes.o: primes.c eratosthenes.c error.c bit_array.h error.h
	$(CC) $(CFLAGS) -c primes.c

eratosthenes.o: eratosthenes.c error.c error.h bit_array.h
	$(CC) $(CFLAGS) -c eratosthenes.c

primes-i.o: primes.c eratosthenes.c error.c bit_array.h error.h
	$(CC) $(CFLAGS) -c primes.c -o primes-i.o -DUSE_INLINE

eratosthenes-i.o: eratosthenes.c error.c error.h bit_array.h
	$(CC) $(CFLAGS) -c eratosthenes.c -o eratosthenes-i.o -DUSE_INLINE

error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c


steg-decode.o: steg-decode.c eratosthenes.c error.c error.h bit_array.h
	$(CC) $(FLAGS) -c steg-decode.c

ppm.o: ppm.c
	$(CC) $(FLAGS) -c ppm.c


run:
	time ./primes
	time ./primes-i


clean:
	rm *.o *.gch primes steg-decode primes-i


zip: *.c *.h Makefile
	zip xkubik32.zip *.c *.h Makefile
