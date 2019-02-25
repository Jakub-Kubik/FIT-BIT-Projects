/**
 * @description  Main for operations over hash table
 * @date         26.04.2017
 * @author       Jan Jakub Kubik (xkubik32@stud.fit.vutbr.cz) */

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#include "htab.h"


void printOneItem(const char *key, const long data){
 printf("%s\t%ld\n", key,data);
}


int main(void)
{
  htab_t *hashed_table;               // struct for hashed table
  struct htab_listitem *item = NULL;  // struct for one item
  bool warning = true;                // stopper for only 1 warning
  char word[WORD_MAX_SIZE+2];         // one word EOW -> for '\0'
  word[0] = '\0';

  // create hashed table
  hashed_table = htab_init(TABLE_SIZE);
  if ( hashed_table == NULL ) {
    fprintf(stderr, "Cant allocate memory for hashed table\n");
    exit(1);
  }

  // ----------------------------------------------------------
  //  Most importatnt algorithm from whole program:
  //    get word by word from stdin
  //    and save it to hashed table
  //    if there isnt this word or
  //    add 1 to the same item i hashed table
  // ----------------------------------------------------------
  while ( (get_word(word, (WORD_MAX_SIZE), stdin)) != EOF ) { // +1 for '\0'
    //printf("%s\n", word);
    if ( strlen(word) == WORD_MAX_SIZE ) {
      if ( warning ) {  // print only firtst warning
        fprintf(stderr, "Word has more than 127 characters\n");
        warning = false;
      }
      word[WORD_MAX_SIZE] = '\0';
    }

    // jump over isspaces
    if ( ((strlen(word)) == 0) ) {
      continue;
    }

    // add item to hashed table
    item = htab_lookup_add(hashed_table, word);
    // if memory wasnt allocated properly in function htab_lookup_add
    if ( item == NULL ) {
      htab_free(hashed_table);
      return 1;
    }

    item->data +=1;
    hashed_table->number_of_items++;

    word[0] = '\0';
  }

  // add in hashed table last word if EOF is on the same line as word
  if ( strlen(word) > 0 )  {
    // add item to hashed table
    item = htab_lookup_add(hashed_table, word);
    item->data +=1;
    hashed_table->number_of_items++;
  }

  // print all words and number of occurence in stdin
  htab_foreach(hashed_table, printOneItem);

  // free allocated space
  htab_free(hashed_table);
  return 0;
}
