/**
 * @description  steganography implementation
 * @date         24.03.2017
 * @author       Jan Jakub Kubik (xkubik32@stud.fit.vutbr.cz) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "ppm.h"        
#include "bit_array.h"        

#define SIZE 3000000


int main(int argc, char *argv[])
{
  if (argc != 2) {
    error_msg("No filename\n");
  }

  struct ppm *a;
  int actual_size = 200;
  char *msg = malloc(actual_size*sizeof(char));
  int msg_size = -1;

  // read data
  a = ppm_read(argv[1]);
  if (a == NULL) {
    free(a);
    return 1;
  }

  // create bit array for prime numbes
  ba_create(array, SIZE);

  // count prime numbers
  Eratosthenes(array);

  unsigned long position = -1;

  do {
    unsigned long j=0;

    msg_size++;
    msg[msg_size]='\0';

    for(int i=0; i<BITS; i++) {
      position++;
      for(j = position; (ba_get_bit(array, j)) == 1 ; j++)
        ;
      position = j;

      int cislo = 1 & a->data[position];
      msg[msg_size] += (cislo << i);

    }
      // if required inrease increase mem size
      if (msg_size == actual_size-1) {
        actual_size += 200;
        msg=realloc(msg, actual_size);
        if ( msg == NULL) {
          free(msg);
          free(a);
          return 1;
        }
      }
  } while( (msg[msg_size] != '\0') && (position < (3 * a->xsize * a->ysize)) );


  free(a);

  if ( msg[msg_size] != '\0' ) {
    error_msg("String in message insnt corretly ended \n");
  }

  // print message
  printf("%s\n", msg);
  free(msg);

  return 0;
}
