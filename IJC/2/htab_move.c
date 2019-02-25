/**
 * @description  Make copy of hashed table
 * @date         26.04.2017
 * @author       Jan Jakub Kubik (xkubik32@stud.fit.vutbr.cz) */

#include <stdio.h>
#include <stdlib.h>

#include "htab.h"


htab_t *htab_move(long newsize, htab_t *t2) {

  // creating new hashed table
  htab_t *table_new = htab_init(newsize);
  if ( table_new == NULL ) {
    fprintf(stderr, "Cant allocate memory for new hashed table\n");
    return NULL;
  }

  // new hashed table is created and default initialized
  table_new->arr_size = newsize;
  table_new->number_of_items = 0;

  // walking through all buckets
  for ( int i = 0; i < t2->arr_size; i++ ) {

    struct htab_listitem *itm = t2->pointer[i];
    struct htab_listitem *itm_prev = t2->pointer[i];

    // jump over empty buckets
    if ( t2->pointer[i] == NULL ) {
      continue;
    }

    // walking through linear list
    while ( itm != NULL ) {
      itm_prev = itm;
      itm = itm->next;

      int index = hash_function(itm_prev->key) % newsize;
      struct htab_listitem *itm_new = table_new->pointer[index];

      // linear list is yet empty
      if ( table_new->pointer[index] == NULL ) {
        table_new->pointer[index] = itm_prev;
        table_new->pointer[index]->next = NULL;
      }
      // linear list has an item
      else {
        while(itm_new->next != NULL) {
          itm_new = itm_new->next;
        }
        itm_prev->next = NULL;
        itm_new->next = itm_prev;
      }
      table_new->number_of_items++;
    }
  }

  for (int i = 0; i<t2->arr_size; i++) {
    t2->pointer[i] = NULL;
  }
  
  return table_new;
}
