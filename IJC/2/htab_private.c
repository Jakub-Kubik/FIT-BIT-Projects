/**
 * @description  Structure for hashed table
 * @date         26.04.2017
 * @author       Jan Jakub Kubik (xkubik32@stud.fit.vutbr.cz) */

#include "htab.h"


struct htab_t {
  long number_of_items;           
  long arr_size;                  
  htab_listitem *pointer[]; // flexible array member (1. listitem)
};
