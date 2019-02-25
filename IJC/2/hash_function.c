/**
 * @description  Hash function
 * @date         26.04.2017
 * @author       Jan Jakub Kubik (xkubik32@stud.fit.vutbr.cz) */

#include "htab.h"


unsigned int hash_function(const char *str) {

  unsigned int h=0;
  const unsigned char *p;
  
  for(p=(const unsigned char*)str; *p!='\0'; p++)
    h = 65599*h + *p;

  return h;
}
