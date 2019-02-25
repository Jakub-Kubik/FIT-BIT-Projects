/**
 * @description  funciton for parsing server arguments
 * @author       Jakub Kubik 
 * @date         12.3.2018 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server.h"


int parseArguments(int argc, char *argv[], int *portNmb)
{
    if ( argc == 3 ) {
        if ( strcmp(argv[1], "-p") == 0 ) {
            char *endPtr = NULL;
            int base = 10;
            *portNmb = strtol(argv[2], &endPtr, base);

            if ( (*portNmb == 0) || (strlen(endPtr) != 0) ) { 
                                                              
                if ( (*argv[PORTNMB] != '0')  ) {             
                                                              
                    fprintf(stderr, "port inst integer value (%s)\n", endPtr);
                    return ERR;
                } 
            }
            else if ((*portNmb > MAXPORT) || (*portNmb < 0)) {  // 65535 is max value of port
                fprintf(stderr, "65535 is max value of port\n");
                return ERR;
            }      
        }
        return OK; 
    }
    return ERR;
}
