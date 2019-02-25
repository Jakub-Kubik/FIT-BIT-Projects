/**
 * @description  Utilit tail written in C
 * @date         26.04.2017
 * @author       Jan Jakub Kubik (xkubik32@stud.fit.vutbr.cz) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_L_SIZE           1024   
#define DEFAULT_NMB_OF_LINES   10   


// @description structure for arguments
typedef struct arguments {
  char * filename;
  long nmb_of_lines;
} arguments;


/**
 * @brief             check all parameters in argv
 * @param argc        number of arguments
 * @param argv        all arguments
 * @return arguments  structure with filename, or number of lines or both */
arguments checkArgs( int argc, char *argv[] );


 /**
  * @brief          check if number has right format
  * @param *option  -n to check
  * @param argv      number to check
  * @return int      number of lines or exit(1) */
int number( char *option, char *number );


// @brief    prints help for program
void wrongArgs(void);


 /**
  * @brief         gets last N lines from stream
  * @param *fr     file stream descriptor
  * @param number  number of required lines
  * @return        array of string -  all required lines or NULL if has error ocured */
char ** all_important_action( FILE *fr, int number );



int main( int argc, char *argv[] ) {

  char **string;            // last N lines
  FILE *fr = stdin;        
  arguments checked_args;   // structure for arguments
  int close_file = false;   // if i need to close file (stdin i dont close)

  // checking arguments
  checked_args = checkArgs(argc,argv);

  // open file or stdin (stdin is default)
  if(checked_args.filename != NULL ) {
    fr = fopen(checked_args.filename, "r");
    if(fr == NULL ){
      fprintf(stderr, "Cant open required file\n");
        free(checked_args.filename);
      return 1;
    }
    else {
      close_file = true;
    }
    free(checked_args.filename);
  }

  // get pointer to required lines
  string = all_important_action( fr, checked_args.nmb_of_lines );

  if ( close_file == true ) {
      fclose(fr);
    }

  // print last N lines if no errors
  if ( string != NULL ) {
    // print all required lines
    for (int i = 0; i < checked_args.nmb_of_lines; i++) {
      fprintf(stdout, "%s", string[i]);
    }
    // freeing memory
    for (int i = 0; i < checked_args.nmb_of_lines; i++) {
      free(string[i]);
    }
    free(string);
  }
  else {
    return 1;
  }
  return 0;
}


char ** all_important_action( FILE *fr, int number ) {

  char **string;  // array of string
  int c;          // one char

  // allocating memory for last number lines
  string = calloc( number, sizeof(char *) );
  if ( string == NULL) {
    fprintf(stderr, "Cant allocate memory for strings\n");
    return NULL;
  }
  for (int i=0; i<number; i++) {
    string[i]=calloc(MAX_L_SIZE, sizeof(char));
    if ( string[i] == NULL ) {
      fprintf(stderr, "Cant allocate memory for one string\n");
      for (int j = 0; j < i; j++) {
        free(string[j]);
      }
      free(string);

      return NULL;
    }
  }

  int i=0;
  int j=0;
  int stopper=0;
  // ----------------------------------------------------
  // Most important algorithm of my program:
  //  - getting line by chars up to 1024 chars
  //  - chars jump over
  //  - each line is ended with '\n' char
  //  - all stream is ended by EOF
  // ---------------------------------------------------
  while ( (c = fgetc(fr)) != EOF ) {

    // make last line free for next line from stream
    if ( stopper == 1 ) {
      for (int i=1; i < number; i++) {
        strncpy( string[i-1], string[i], MAX_L_SIZE );
      }
      for ( unsigned int i=0; i<MAX_L_SIZE; i++ ) {
        string[j][i] = '\0';
      }
    }
    stopper=0;

    // get one char
    string[j][i] = c;
    // check max length of string and if is line correctly ended
    if ( (i == MAX_L_SIZE-1) || (string[j][i] == '\n') ) {

      string[j][strlen(string[j])] = '\0';

      // check sting length
      if (string[j][strlen(string[j])-1] != '\n' ) {
        fprintf(stderr, "Too long line\n" );
        // get all chars to the end of line
        while ( fgetc(fr) != '\n' )
        ;
      string[j][strlen(string[j])-1] = '\n';
      }

    i=0;  // counting from the beginnig of line

    // for getting first N lines
    if ( j != number-1 ) {
      j++;
    }
    else {
      stopper = 1;
    }
  } // end of line
  else {
    i++;  // for next character
  }
} // End of all cycle for getting lines

  return string;  // all required lines
}


arguments checkArgs( int argc, char *argv[] ) {
  
  arguments checking_args;

  switch(argc) {
    //  argc without -n
    case 1:
      checking_args.nmb_of_lines = DEFAULT_NMB_OF_LINES;
      checking_args.filename = NULL ;
      break;
    // only filename
    case 2:
      checking_args.nmb_of_lines = DEFAULT_NMB_OF_LINES;
      checking_args.filename =malloc((strlen(argv[1])+1) * sizeof(char)); // +1 for '\0'
      strcpy(checking_args.filename, argv[1]);
      break;
    // -n  and munber of lines
    case 3:
      checking_args.nmb_of_lines = number(argv[1], argv[2]);
      checking_args.filename = NULL;
      break;
    // -n , number of lines and filename
    case 4:
      checking_args.nmb_of_lines = number(argv[1], argv[2]);
      checking_args.filename = malloc((strlen(argv[3])+1) * sizeof(char));
      strcpy(checking_args.filename, argv[3]);
    break;
    // wrong parameters
    default:
      wrongArgs();  // help to porgram
      checking_args.filename = NULL;
      break;
  }

  return checking_args; // struct with all checked arguemnts
}


int number( char *option, char *number ) {

  if ( (strcmp(option, "-n")) != 0 ) {
    fprintf(stderr, "-n has wrong format\n");
    wrongArgs();   // help to program
    exit(1);
  }

  char *ptr;
  int length;
  length = strtol(number, &ptr, 10);
  if ( *ptr != 0 ) {
    fprintf(stderr, "Wrong format for number\n");
    wrongArgs();   // help to program
    exit(1);
  }
  return length;
}


void wrongArgs(void) {
  fprintf(stderr, "\n\n******************Wrong args**********************\n");
  fprintf(stderr, "This program prints last N lines from file or stdin \n");
  fprintf(stderr, "----------------------------------------------------\n");
  fprintf(stderr, "Using:\n");
  fprintf(stderr, "\t./tail [-n number] [FILENAME]\n");
  fprintf(stderr, "\t-----------------------------\n");
  fprintf(stderr, "\t- both parameters are optional\n");
  fprintf(stderr, "\t- Defaultn value of number is 10\n");
  fprintf(stderr, "\t- Default value of stream is stdin\n");
  fprintf(stderr, "********************************************************\n");
}
