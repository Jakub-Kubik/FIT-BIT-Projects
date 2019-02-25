/**
 * @description  Search and remove required item from hashed table
 * @date         26.04.2017
 * @author       Jan Jakub Kubik (xkubik32@stud.fit.vutbr.cz) */

#include <stdio.h>
#include <stdbool.h>

#include "htab.h"


bool htab_remove(htab_t *t, const char *key) {

  int index = (hash_function(key) % TABLE_SIZE);

  struct htab_listitem *itm = t->pointer[index];
  struct htab_listitem *itm_previous = itm;
  struct htab_listitem *itm_head = t->pointer[index];


  if ( itm == NULL ) {
    return false;
  }
  else {
    // find required item
    while ( (itm != NULL) && ((strcmp( itm->key, key )) != 0) ) {
      itm_previous = itm;
      itm = itm->next;
    }

    if ( (itm != NULL) && ((strcmp(itm_previous->key, key)) == 0) ) {

      if ( itm == itm_head ) {
        t->pointer[index] = itm->next;
        free(itm->key);
        free(itm);
        t->number_of_items--;
      }
      else {
        itm_previous->next = itm->next;

        free(itm->key);
        free(itm);

        t->number_of_items--;
      }

      if ( itm == itm_head ) {
          t->pointer[index] = NULL;
      }
      return true;
    }
    else {
      return false;
    }
  }
}
