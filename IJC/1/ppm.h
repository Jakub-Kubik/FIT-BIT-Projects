/**
 * @description  header file for ppm.h
 * @date         24.03.2017
 * @author       Jan Jakub Kubik (xkubik32@stud.fit.vutbr.cz) */

#ifndef PPM_H
#define PPM_H

// @description number of image colors
#define P_FARIEB 255


// @description structure for saving content from image
struct ppm {
  unsigned xsize;
  unsigned ysize;
  char  data[];    // RGB bytes, overall 3*xsize*ysize
};


/**
 * @brief           read content of image to dynamicly alocated structure in this function
 * @param filemane  name of file to be manipulated
 * @return          structure with content form file */
struct ppm * ppm_read(const char * filename);


/**
 * @brief           write structure to file in PPM format
 * @param p         structure for writing
 * @param filename  file to write
 * @return          status code */
int ppm_write(struct ppm *p, const char * filename);

#endif
