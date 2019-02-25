/**
 * @description  interface for reading and writing image form file
 * @date         24.03.2017
 * @author       Jan Jakub Kubik (xkubik32@stud.fit.vutbr.cz) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ppm.h" 
#include "error.h" 


struct ppm *ppm_read(const char *filename) {

  struct ppm *a;       
  FILE *fr;            
  char chck_format[3]; 

  if ((fr = fopen(filename, "rb")) == NULL) {
    warning_msg("Cant open %s for reading\n", filename);
    a = NULL;
    return a;
  }

  // format checking
  fgets( chck_format, sizeof(chck_format) , fr );
  if (chck_format[0] != 'P' && chck_format[1] != '6') {
    warning_msg("Wrong format in %s\n", filename);
    a = NULL;
    return a;
  }

  // size checking
  int x;
  int y;
  if( (fscanf(fr, "%d %d", &x, &y)) != 2 ) {
    warning_msg("Wrong size format in %s\n", filename);
    a = NULL;
    return a;
  }

  // color depth checking
  int p_farieb;
  if( (fscanf(fr, "%d ", &p_farieb)) == 1 ) {
    if (p_farieb != P_FARIEB) {
      warning_msg("Wrong number of clors in %s\n", filename);
      a = NULL;
      return a;
    }
  }

  a = malloc(sizeof(struct ppm) + 3*x*y*sizeof(a->data[0]));
  a->xsize =  x;
  a->ysize = y;
  memset(a->data, 0, a->xsize*a->ysize*3);

  if ((fread(a->data, sizeof(a->data[0]), 3*a->xsize*a->ysize, fr)) != 3*a->xsize*a->ysize) {
    warning_msg("Cant read data from %s\n", filename);
    a = NULL;
  }

  fclose(fr);

  return a; // return of pointer to structure with data
}


int ppm_write(struct ppm *p, const char * filename) {

  FILE *fw;

  if ((fw = fopen(filename, "wb")) == NULL) {
    warning_msg("Nepodarilo sa otvorit subor %s na zapis\n", filename);
    return -1;
  }
  // required data for this format
  fprintf(fw, "P6\n");
  fprintf(fw, "%d %d\n", p->xsize, p->ysize);
  fprintf(fw, "%d", P_FARIEB);

  // write of data
  fwrite(p->data, sizeof(p->data[0]), 3*p->xsize*p->ysize, fw);
  fclose(fw);

  return 0;
}
