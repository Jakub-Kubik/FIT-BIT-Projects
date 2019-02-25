/**
 * @description  implemenatation of eratosthnes algorithm
 * @date         24.03.2017
 * @author       Jan Jakub Kubik (xkubik32@stud.fit.vutbr.cz) */

#include <stdio.h>
#include <math.h>         
#include "bit_array.h"    

void Eratosthenes(bit_array_t pole) {

  #define N ba_size(pole)
  #define SqrtN sqrt(N)

  // 0,1, 2 arent prime numbers
  ba_set_bit(pole, 0L, 1L);
  ba_set_bit(pole, 1L, 1L);
  //ba_set_bit(pole, 2L, 1L);

  for (unsigned long i = 2L; i < SqrtN; i++ ) {
    if (ba_get_bit(pole,i) == 0L) {
      for (unsigned long j = i+i; j < N; j += i ) {
        ba_set_bit(pole, j, 1L);
      }
    }
  }
}
