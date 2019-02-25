/**
 * @description  implementation of error printing
 * @date         24.03.2017
 * @author       Jan Jakub Kubik (xkubik32@stud.fit.vutbr.cz) */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>  
#include "error.h"    


void warning_msg(const char *fmt, ...) {

  va_list argumenty;
  va_start(argumenty, fmt);

  fprintf(stderr, "ERROR: ");
  vfprintf(stderr, fmt, argumenty);

  va_end(argumenty);
}


void error_msg(const char *fmt, ...) {

  va_list argumenty;
  va_start(argumenty, fmt);

  fprintf(stderr, "CHYBA: ");
  vfprintf(stderr, fmt, argumenty);

  va_end(argumenty);
  exit(1);
}
