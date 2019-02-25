/**
 * @description  header file for eratosthenes.c and primes.c
 * @date         24.03.2017
 * @author       Jan Jakub Kubik (xkubik32@stud.fit.vutbr.cz) */

#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "error.h"    

// @description new dtatypee for bitarray
typedef unsigned long * bit_array_t;

/**
 * @brief        find prime numbers in array
 * @param array  array to check */
void Eratosthenes(bit_array_t pole);


#define BITS 8        // @description number of bitst for byte
#define SIZE_STORE 1  // @description alocational unit to be saved size of bit array
#define INDEX 0       // @description index wheire is save bit array size 
 

/**
 * @description macros for set and get item from bit array */
#define BA_GET_BIT(p, i) \
  ( 0L != ( (p[1L + (i) / (sizeof(unsigned long) * BITS)]) & (1L << ((i) % (sizeof(unsigned long) * BITS) ))) )

#define BA_SET_BIT(p, i, b) \
  (  ((b) == 1) ? \
     ((p[1L + (i) / (sizeof(unsigned long) * BITS)]) |= (1L << ((i) % (sizeof(unsigned long) * BITS) ))) : \
     (p[1L + (i) / (sizeof(unsigned long) * BITS)] &= (~(1L << ((i) % (sizeof(unsigned long) * BITS) )))))


/**
 * @description count number of longs for saving bit array size + bit array */
#define BA_size_UL(size) \
  SIZE_STORE  + \
  ( (size) / (sizeof(unsigned long)*BITS )) + \
  ( ((size) % (sizeof(unsigned long))) != 0) + 1


/**
 * @description crate of bit array */
#define ba_create(meno_pola, size) \
  unsigned long *meno_pola  = calloc(BA_size_UL(size), sizeof(unsigned long)); \
  meno_pola[INDEX] = size;


// @decription conditional use of inline function 
#ifdef USE_INLINE
  /**
   * @brief return size of bit array which is save at index 0(INDEX) */
  inline unsigned long ba_size( bit_array_t meno_pola ) { return meno_pola[INDEX]; }


  /**
   * @brief set bit to required value */
  inline void ba_set_bit( bit_array_t meno_pola, unsigned long index, bool vyraz ) {
    if ( index <= ba_size(meno_pola) )
      BA_SET_BIT(meno_pola, index, vyraz);
    else
      error_msg("Index %lu out of range 0..%lu", index, meno_pola[0]);  // meno_pola[0] -> size bit pola
  }


  /**
   * @brief get requird value from specific index */
  inline bool ba_get_bit( bit_array_t meno_pola, unsigned long index ) {
    if ( index <= ba_size(meno_pola) )
      return BA_GET_BIT(meno_pola, index);
    error_msg("Index %lu mimo rozsah 0..%lu", index, meno_pola[0]);
    return 1;
  }


#else // dont use inline function
  /**
   * @description size of bit array */
  #define ba_size(meno_pola) ( (meno_pola[INDEX]) )


  /**
   * @description set bit to required value */
  #define ba_set_bit(meno_pola, index, vyraz) \
   ( ((index) <= (ba_size(meno_pola)) ) ? \
      (void)BA_SET_BIT(meno_pola, index, vyraz) : \
      error_msg("Index %lu mimo rozsah 0..%lu", index, meno_pola[0]))
  

  /**
   * @description get required value from specific index */
  #define ba_get_bit(meno_pola, index) \
    (((index) <= (ba_size((meno_pola)))  ) ? \
      BA_GET_BIT((meno_pola), (index)) : \
      ((error_msg("Index %lu mimo rozsah 0..%lu", index, meno_pola[0])),1))


#endif // # USE_INLINE

#endif
