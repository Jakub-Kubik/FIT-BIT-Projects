/**
 * @description  Get word length
 * @date         26.04.2017
 * @author       Jan Jakub Kubik (xkubik32@stud.fit.vutbr.cz) */

#include <ctype.h>

#include "htab.h"


int get_word(char *s, int max, FILE *f) {

  int c;
  int nmb_of_chars = 0;
  s[0] = '\0';

  c = fgetc(f);
  while ( (c != EOF) && ( isspace(c) == 0 ) ) {

    // get all chars to the end of word
    if ( max == nmb_of_chars ) {
        while ( !isspace(fgetc(f)) )
          ;
      break;
    }

    s[nmb_of_chars] = c;
    nmb_of_chars++;
    s[nmb_of_chars]='\0';

    // get next char
    c = fgetc(f);  
  }

  if ( c == EOF ) {
    return EOF;
  }

  return nmb_of_chars;
}
