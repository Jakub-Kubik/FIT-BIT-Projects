# IJC (The C programming language)

## Number of projects and language
2 projects in C language.

### Simple info about 1st project
1st project is aimed to counting prime numbers using bit vectors. Other part of
project is aimed on steganography.

### File structure of 1st project
    1/
    |-- Makefile
    |-- bit_array.h     # header file for eratosthenes.c and primes.c
    |-- eratosthenes.c  # implementation of eratosthenes algorithm
    |-- error.c         # funcitons for error printing
    |-- error.h         # header file for error.c
    |-- ppm.c           # reading and writing image
    |-- ppm.h           # header file for ppm.c
    |-- primes.c        # main for printing prime numbers
    `-- steg-decode.c   # main for steganography implementation


### Simple info about 2nd project
2nd project is aimed on static and dynamic library creation, hash table operations and rewriting program from C++ to C.

### File structue of 2nd project

    2/
    |-- Makefile
    |-- hash_function.c      # hash function
    |-- htab.h               # header file for hash table operations
    |-- htab_bucket_count.c  # number of buckets in hash table
    |-- htab_clear.c         # remove all items form hashed table
    |-- htab_find.c          # search for item in hashed table
    |-- htab_foreach.c       # print all items from hashed table
    |-- htab_free.c          # destructor for hahed table
    |-- htab_init.c          # constructor for hashed table
    |-- htab_lookup_add.c    # search for an item and add it
    |-- htab_move.c          # copy of hashed table
    |-- htab_private.c       # structure for hashed table
    |-- htab_remove.c        # remove required item from ahshed table
    |-- htab_size.c          # size of hashed table
    |-- io.c                 # get word length
    |-- tail.c               # tail utility written in C
    |-- tail2.cc             # tail utility written in C++
    `-- wordcount.c          # main for operations over hashed table


## Author
Jan Jakub Kubik.