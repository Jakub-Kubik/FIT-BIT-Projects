/**
 * @description  Print all items in hashed table
 * @date         26.04.2017
 * @author       Jan Jakub Kubik (xkubik32@stud.fit.vutbr.cz) */

#include "htab.h"


void htab_foreach(htab_t *t, void(*fn)(const char *key, const long data) ) {

  if ( t != NULL ) {
    struct htab_listitem *itm = NULL;

    for ( int index = 0; index < t->arr_size; index++ ) {

      itm = t->pointer[index];
      if ( itm == NULL ) {
        continue;
      }

      for ( ; itm != NULL; itm = itm->next ) {
          fn( itm->key, itm->data );
      }
    }
  }
}
