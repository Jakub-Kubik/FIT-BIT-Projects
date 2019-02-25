/**
 * @description  Destructor for hashed table
 * @date         26.04.2017
 * @author       Jan Jakub Kubik (xkubik32@stud.fit.vutbr.cz) */

#include "htab.h"


void htab_free(htab_t *t) {

  htab_clear(t);
  free(t);  // free whole alocated space for hashed table
}
