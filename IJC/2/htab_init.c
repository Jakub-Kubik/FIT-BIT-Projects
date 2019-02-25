/**
 * @description  Constructor for hahed table
 * @date         26.04.2017
 * @author       Jan Jakub Kubik (xkubik32@stud.fit.vutbr.cz) */

#include <stdio.h>

#include "htab.h"


htab_t *htab_init(long size) {

  htab_t  *table;

	table = malloc(sizeof(htab_t) + (size) * sizeof(table->pointer[0]) );
	if(table == NULL){
		fprintf(stderr, "Cant allocate memory for hashed table\n");
		return NULL;
	}
  else {
    table->arr_size = size;
    table->number_of_items = 0;
  }

  // default initialization of hashed table
	for(unsigned i = 0; i < size; i++) {
		table->pointer[i] = NULL;
	}

return table;
}
