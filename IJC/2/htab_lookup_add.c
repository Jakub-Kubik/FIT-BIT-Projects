/**
 * @description  Searching for item and if isnt in hash table add it
 * @date         26.04.2017
 * @author       Jan Jakub Kubik (xkubik32@stud.fit.vutbr.cz) */

#include <stdio.h>
#include <stdlib.h>
#include "htab.h"


struct htab_listitem *htab_lookup_add(htab_t *t, const char *key) {

  // get index in hashed table
  unsigned index = (hash_function(key) % TABLE_SIZE) ;        

  struct htab_listitem *itm = t->pointer[index];
  struct htab_listitem *itm_previous = itm;
  struct htab_listitem *itm_new = NULL;

  // Check if isnt linear list empty and create new item
  if(itm == NULL) {
    itm_new = malloc ( sizeof(htab_listitem) );
    if ( itm_new == NULL ) {
      fprintf(stderr, "Can not allocate memory for new item in hashed talbe\n");
      return NULL;
    }

    itm_new->key = malloc( (strlen(key) + EOW) * sizeof(char) ); // EOW -> end of word
  	if(itm_new->key == NULL) {
      fprintf(stderr, "Can not allocate memory for new item key in hashed table\n");
      return NULL;
    }

    t->pointer[index] = itm_new;

    // copy item to new allocated place
    strcpy(t->pointer[index]->key,key);
    t->pointer[index]->data = 0;
    t->pointer[index]->next = NULL;

    return t->pointer[index];
  }

  // Try to find item
  else {
    while ( (itm != NULL) && ( strcmp(itm->key, key) != 0) ) {
      itm_previous = itm;
      itm = itm->next;
    }

    // Item found
    if ( (itm != NULL) && (strcmp(itm->key, key) == 0 )) {
        return itm;
    }

    // Item isnt found so i create new item and add it to the second position in lineear list
    else {
      // allocate required memory
      itm_new = malloc ( sizeof(htab_listitem) );
      if ( itm_new == NULL ) {
        fprintf(stderr, "Cannot allocate memory for new item in hashed talbe\n");
        return NULL;
      }
      else {
        itm_new->key = NULL;
      }

      itm_new->key = malloc( (strlen(key) + EOW) * sizeof(char) ); // EOW -> end of word
    	if(itm_new->key == NULL) {
        fprintf(stderr, "Cant allocate memory for new item key in hashed table\n");
        return NULL;
      }

      itm = itm_previous;

      // copy item to new allocated place
    	strcpy(itm_new->key,key);
    	itm_new->data = 0;
      itm_new->next = itm->next;
      itm->next = itm_new;

    	return itm_new;
    }
  }
}
