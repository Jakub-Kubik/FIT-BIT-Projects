/**
 * @description  Remove all items from hashed table
 * @date         26.04.2017
 * @author       Jan Jakub Kubik (xkubik32@stud.fit.vutbr.cz) */

#include "htab.h"


void htab_clear(htab_t *t) {

  struct htab_listitem *item = NULL;
  struct htab_listitem *item_previous = NULL;

	for(int index = 0; index < t->arr_size; index++) {
		item = t->pointer[index];
    item_previous = item;
    if ( (item == NULL) ) {
      continue;
    }

		while(item != NULL) {
        item_previous = item;
        item = item->next;
        free(item_previous->key);
        free(item_previous);
		}
  }
}
