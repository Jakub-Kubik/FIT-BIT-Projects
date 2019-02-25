/**
 * @description  Print last 10 prime numbers in SIZE sequence
 * @date         24.03.2017
 * @author       Jan Jakub Kubik (xkubik32@stud.fit.vutbr.cz) */

#include <stdio.h>
#include <stdlib.h>

#include "bit_array.h"


#define SIZE 303000000  // 303mil size of bit array cca 40MB


int main(void) {

  // create bit array
  ba_create(array, SIZE);

  // find out prime numbers
  Eratosthenes(array);

  // move on last 10 prime numbers
  long unsigned poradie = SIZE; int i = 10;
  while ( i > 0) {
    if (ba_get_bit(array, poradie) == 0)
      i--;
    poradie--;
  }

  // print last 10 prime numbers
  for (unsigned long i = poradie; i<SIZE; i++) {
    if (ba_get_bit(array, i) == 0) {
      printf("%lu\n", i);
    }
  }

  free(array);

  return 0;
}
