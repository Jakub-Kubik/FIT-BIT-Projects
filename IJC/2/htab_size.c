/**
 * @description  Get size of hashed table
 * @date         26.04.2017
 * @author       Jan Jakub Kubik (xkubik32@stud.fit.vutbr.cz) */

#include "htab.h"


size_t htab_size(htab_t *t) {
  return t->number_of_items;
}
