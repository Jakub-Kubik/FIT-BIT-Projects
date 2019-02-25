/**
 * @description  function for parsing program arguments
 * @author       Jakub Kubik 
 * @date         12.3.2018 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "client.h"
#include "communication.h"


int parseArguments(int argc, char *argv[],char **host, int *portNmb, int *opt, char **login) {
    
    if ( argc == 6 || argc == 7 ) {
        // checking host and saving host name or IP
        if ( strcmp(argv[HOST], "-h") == 0 ) {
        char *help;
        help = malloc(strlen(argv[HOSTNAM]) +1);
        if (help == NULL) return ERR; // error by allocating memory
        strcpy( help, argv[HOSTNAM]);
        *host = help;
        }
        else return ERR;
        
        // checking and saving port and port number
        if ( strcmp(argv[PORT], "-p") == 0 ) {
            char *endPtr = NULL;
            int base = 10;
            *portNmb = strtol(argv[PORTNMB], &endPtr, base);

            if ( (*portNmb == 0) || (strlen(endPtr) != 0) ) { 
                                                              
                if ( (*argv[PORTNMB] != '0')  ) {             
                                                              
                    fprintf(stderr, "port inst integer value (%s)\n", endPtr);
                    return ERR;
                }
                
            }
            else if ((*portNmb > MAXPORT) || (*portNmb < 0)) {
                fprintf(stderr, "65535 is max value of port\n");
                return ERR;
            }   
        }
        else return ERR;

        // checking options
        if ((strcmp(argv[OPTIONS], "-n") == 0) && (argc == 7))   *opt = NAME;
        
        else if (strcmp(argv[OPTIONS], "-f") == 0 && (argc == 7))  *opt = PATH;
        
        else if (strcmp(argv[OPTIONS], "-l") == 0)   *opt = ALLUSR;
        
        else return ERR;    // not allowed option or not enough arguments

        // checking and saving login if is required
        if ( argc == 7 ) {

            // duplicit using of -n or -f or -l at login position
            if ( (strcmp(argv[LOGIN], "-n") == 0) || (strcmp(argv[LOGIN], "-f") == 0) || (strcmp(argv[LOGIN], "-l") == 0) ) {
                return ERR;
            }
            
            char *helpLogin = malloc(strlen(argv[LOGIN]) + 1);
            if ( helpLogin == NULL ) return ERR; // error by allocating memory
            strcpy( helpLogin, argv[LOGIN]);
            *login = helpLogin;
        } 
        return OK;
    }
    else return ERR; // not allowed number of arguments
}


char *flgOpt2Str(int flag, int opt) {

    char *serverReq;

    if ( opt == 0 ) {
        serverReq = malloc(1*sizeof(char) + 1); // for saving one byte int values to string
        sprintf(serverReq, "%d", flag);
    }
    else {
        serverReq = malloc(2*sizeof(char) + 1); // for saving 2 one byte int values to string
        sprintf(serverReq, "%d%d", flag, opt);
    }

    return serverReq;
}
