/**
 * @description  Header file for hash table operations
 * @date         26.04.2017
 * @author       Jan Jakub Kubik (xkubik32@stud.fit.vutbr.cz) */

#ifndef wordcount_h
#define wordcount_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


size_t s;   

/**
 * @brief  The Second Edition of the 20-volume
 *         Oxford English Dictionary contains
 *         full entries for 171,476 words in current use
 *         #define TABLE_SIZE 65536    // 2^16 */
#define TABLE_SIZE 65536
#define EOW  1              // end of word '\0'
#define WORD_MAX_SIZE 127   // max size for word



// @decription  structure for storing one item in hashed table
typedef struct htab_listitem {
  char *key;                  // string
  long data;                  // number of occurence key in text
  struct htab_listitem *next; // pointer to next item
} htab_listitem;

// @description  structure for hashed table 
typedef struct htab_t{
  long number_of_items;           // number of all items
  long arr_size;                  // number of linear list
  htab_listitem *pointer[]; // flexible array member (1. listitem)
} htab_t;


/**
 * @brief         calculates index in hashed table by PEPE algorithm
 * @param string  for calculation
 * @return        counted index */
unsigned int hash_function(const char *str);


/**
 * @brief       create hashed table
 * @param size  number of buckets to alocate
 * @return      hthashed table */
htab_t *htab_init(long size);


/**
 * @brief          move all items form hashed table to another
 * @param *t2      pointer to old hashed table
 * @param newsize  number of buckets in new hashed table
 * @return htab_t  new hashed table */
htab_t *htab_move(long newsize, htab_t *t2);


/**
 * @brief clear  whole table from members
 * @param *t     pointer to hashed table
 * @return       number of stored elements */
size_t htab_size(htab_t *t);


/**
 * @brief     count number of buckets
 * @param *t  pointer to hashed table
 * @return    number of buckets in table */
size_t htab_bucket_count(htab_t *t);


/**
 * @brief       find intem in table or alocate space for it
 * @param *t    hashed table
 * @param *key  searched string
 * @return      pointer to found item */
struct htab_listitem *htab_lookup_add(htab_t *t, const char *key);


/**
 * @brief       find item in hashed table
 * @param *t    hashed table
 * @param *key  searched string
 * @return      pointer to fund item or NULL */
struct htab_listitem *htab_find(htab_t *t, const char *key);


/**
 * @brief          print all items in hashed table
 * @param *t       hashed table
 * @param pointer  to function to be called for all items */
void htab_foreach(htab_t *t, void(*fn)(const char *key, const long data) );


/**
 * @brief        remove specific item in hashed table
 * @param *t     hashed table
 * @param *key   item to be removed
 * @return bool  true or false */
bool htab_remove(htab_t *t, const char *key);


/**
 * @brief     remove all items form hashed table
 * @param *t  hashed table */
void htab_clear(htab_t *t);


/**
 * @brief     destructor - reove whole table from heap
 * @param *t  hashed table */
void htab_free(htab_t *t);


/**
 * @brief      get one word from stream
 * @param *s   store word here
 * @param max  max number of char in word
 * @param *f   file stream descriptor
 * @return     int number of char in word */
int get_word(char *s, int max, FILE *f);


#endif
