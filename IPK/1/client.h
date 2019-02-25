/**
 * @description  header file for client.c
 * @author       Jakub Kubik 
 * @date         12.3.2018 */

#ifndef CLIENT_H
#define CLIENT_H

// for checking arguments
// return values
#define OK 0
#define ERR 1

#define MAXPORT 65535
// arguments option
#define HOST    1
#define HOSTNAM 2  // DNS host name or ip
#define PORT    3  // -p
#define PORTNMB 4   // value of port
#define OPTIONS 5    // options for getting user 
#define LOGIN   6      // login of reqired user

// ########## states of my FSM for client ##########
#define INIT      100
#define ONLYREQ   101
#define DATAPRINT 102
#define ENDCOM    103

// function for checing client arguments
int parseArguments(int argc, char *argv[],char **host, int *portNmb, int *opt, char **login);

char *flgOpt2Str(int flag, int opt);

#endif