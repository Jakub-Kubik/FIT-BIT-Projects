/**
 * @description  header file for server.c
 * @author       Jakub Kubik 
 * @date         12.3.2018 */

#ifndef SERVER_H
#define SERVER_H


// return values
#define OK 0
#define ERR 1
// max port number
#define MAXPORT 65535

#define PORTNMB 2

// states of my FSM for server
#define INIT 	101
#define SEARCH 	102
#define END 	103

int parseArguments(int argc, char *argv[], int *portNmb);

char *fileToStr(FILE *fr, char *login, int opt);

#endif