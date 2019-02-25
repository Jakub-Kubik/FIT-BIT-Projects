/**
 * @description  Find item in hashed table or return 0
 * @date         26.04.2017
 * @author       Jan Jakub Kubik (xkubik32@stud.fit.vutbr.cz) */

#include "htab.h"


struct htab_listitem *htab_find(htab_t *t, const char *key) {

  unsigned index = hash_function(key) % TABLE_SIZE;               // get index in hashed table
  struct htab_listitem *itm = t->pointer[index];      // poiter to first item in required bucket

  if ( itm == NULL ) {
    return NULL;
  }
  else {
    while ( (itm != NULL) && (strcmp(itm->key, key) != 0)  ) {
      itm = itm->next;
    }
    if ( itm == NULL ) {
      return NULL;
    }
    return itm;
  }
}
