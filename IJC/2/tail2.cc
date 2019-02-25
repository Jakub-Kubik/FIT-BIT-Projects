/**
 * @description  Utilit tail written in C++
 * @date         26.04.2017
 * @author       Jan Jakub Kubik (xkubik32@stud.fit.vutbr.cz) */

#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <queue>


#define DEFAULT_NMB_OF_LINES 10	// @description default number of lines
using namespace std;


// @description structure for arguments
struct arguments {
	string filename;
	long nmb_of_lines;
};


/**
 * @brief             check all parameters in argv
 * @param argc        number of arguments
 * @param argv        all arguments
 * @return arguments  structure with filename, or number of lines or both */
arguments checkArgs( int argc, char *argv[] );


/**
 * @brief          check if number has right format
 * @param *option  -n to check
 * @param argv     number to check
 * @return int     number of lines or exit(1) */
int number(char * str);


// @brief  prints help for program
void wrongArgs(void);



int main(int argc, char *argv[]){

	std::ios::sync_with_stdio(false);

	istream *input = &(cin);  // stream for input
	ifstream fr;              // file descriptor
	queue<string> output;     // queue with all required lines
	string line;          	  // buffer for one line
	long loaded_lines = 0;    // number of lines
	arguments checked_args;		// structure for arguments

	// checking arguments
	checked_args = checkArgs(argc,argv);

	// open file or stdin
	if(checked_args.filename[0] != '\0'){
		fr.open(checked_args.filename, ifstream::in);
		if(fr.is_open()){
			input = &fr;
		}
    else {
      fprintf(stderr, "Cant open file for reading\n");
      return 1;
      }
		}

  // get required lines in queue
	while(getline(*input,line)){
		if(loaded_lines >= checked_args.nmb_of_lines){
			output.pop();
			loaded_lines--;
		}
		loaded_lines++;
		output.push(line);
	}

  // print all required lines
	while( !output.empty() ){
		line = output.front();
	    cout << line << '\n';
	    output.pop();
	  }

	return 0;
}


arguments checkArgs( int argc, char *argv[] ) {

	arguments checking_args;

  switch(argc) {
    //  argc without -n
    case 1:
      checking_args.nmb_of_lines = DEFAULT_NMB_OF_LINES;
      checking_args.filename = '\0';
      break;
    // only filename
    case 2:
      checking_args.nmb_of_lines = 10;
      checking_args.filename = (string)(argv[1]);
      break;
    // -n  and munber of lines
    case 3:
      if(	!((string)(argv[1])).compare("-n") ) {
        checking_args.nmb_of_lines = number(argv[2]);
        checking_args.filename = '\0';
      }
      else {
        wrongArgs();
        exit(1);
      }
      break;
    // -n , number of lines and filename
    case 4:
    if(	!((string)(argv[1])).compare("-n") ) {
      checking_args.nmb_of_lines = number(argv[2]);
      checking_args.filename = (string)(argv[3]);
    }
    else {
      wrongArgs();
      exit(1);
    }
      break;
    // wrong parameters
    default:
      wrongArgs();
      exit(1);
      break;
  }
  return checking_args;
}


int number(char * str) {

  char *ptr;
  int length;
  length = strtol(str, &ptr, 10);
  if ( *ptr != 0 ) {
    fprintf(stderr, "Wrong format for number\n");
    wrongArgs();   // help to program
    exit(1);
  }

  return length;
}


void wrongArgs(void) {

  fprintf(stderr, "\n\n********************Wrong args**********************\n");
  fprintf(stderr, "This program prints last N lines from file or stdin \n");
  fprintf(stderr, "----------------------------------------------------\n");
  fprintf(stderr, "Using:\n");
  fprintf(stderr, "\t./tail [-n number] [FILENAME]\n");
  fprintf(stderr, "\t-----------------------------\n");
  fprintf(stderr, "\t- both parameters are optional\n");
  fprintf(stderr, "\t- Defaultn value of number is 10\n");
  fprintf(stderr, "\t- Default value of stream is stdin\n");
  fprintf(stderr, "****************************************************\n");
}
