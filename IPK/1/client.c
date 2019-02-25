/**
 * @description  main function for client
 * @author       Jakub Kubik 
 * @date         12.3.2018 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <stdbool.h>
#include <netinet/in.h>

#include "client.h"
#include "communication.h"

int main(int argc, char *argv[]) {

    char *host;     // DNS name or IP
    int portNmb;    // port number
    int opt;        // -n(as 2) | -f(as 3) | -l(as 4)
    char *login = NULL;

    int retValue = parseArguments(argc, argv, &host, &portNmb, &opt, &login);
    if ( retValue != OK ) {
        fprintf(stderr, "ERROR by parsing arguments\n");
        return ERR;
    }
    #ifdef DEBUG
        printf("Arguments are OK\n");
        printf("Port number is: %d\n", portNmb);
        printf("host name is %s\n", host);
        printf("option is %d\n", opt);
        if (login != NULL)  printf("login is %s\n", login);
    #endif


    int client_socket;
    struct sockaddr_in server;
    char *server_reply = malloc(BUFSIZE);
    char server_req[BUFSIZE];
    server_req[0] = '\0';
    
    // create socket
    if ((client_socket = socket(AF_INET , SOCK_STREAM , 0)) <= 0) {
        fprintf(stderr, "ERROR: creating client socket\n");
        exit(EXIT_FAILURE);
    }
    #ifdef DEBUG
    printf("Socket created\n");
    #endif 


    struct hostent *hostent_struct;
    char * ip_server;

    // convert DNS name to ip address if it is required
    hostent_struct = gethostbyname(host);
    if ( hostent_struct == NULL ) {
        fprintf(stderr, "Canot gethotbyname\n");
        exit(EXIT_FAILURE);
    }
    ip_server = inet_ntoa(*((struct in_addr *)hostent_struct->h_addr_list[0]));
    if ( ip_server == NULL ) {
        fprintf(stderr, "ERROR: cannot conver DNS name to IP\n");
        exit(EXIT_FAILURE);
    }
    #ifdef DEBUG
    printf("Hostname: %s Host IP: %s\n", host, ip_server);
    #endif

    // default init of output port and address structure
    server.sin_addr.s_addr = inet_addr(ip_server);
    server.sin_family = AF_INET;
    server.sin_port = htons( portNmb );
 
    // connect to remote server
    if (connect(client_socket , (struct sockaddr *)&server , sizeof(server)) != 0) {
        fprintf(stderr, "connect failed. Error\n");
        exit(EXIT_FAILURE);
    }
    #ifdef DEBUG
    printf("Connected to remote server\n");
    #endif

    /**
     * Reading data from server
     * ------------------------
     * first byte will be flag to inform server that i want data 
     * second byte will be option (-n, -f, -l)
     * and from third byte to end will be login or third byte will be '\0' */
    char *req;
    req = flgOpt2Str(DATAREQ_FLAG, opt);
    strcat(server_req, req);
    if ( login != NULL ) {
        strcat(server_req, login);
    }    
    #ifdef DEBUG
    printf("sending to server: %s\n", server_req);
    #endif

    // first message to server with all above items
    if ( ( send(client_socket, server_req, strlen(server_req) + 1, 0) ) < 0 ) {
            fprintf(stderr, "ERROR: by sending reqv for data to server\n");
            exit(EXIT_FAILURE);
    }
    
    int state = INIT;
    bool run = true;    // this variable is for informing program that communication hasnt ended

    // state machine for client
    while ( run ) {
 
        switch( state ) {
            
            // default state for client
            case INIT:
                if ( recv(client_socket, server_reply, BUFSIZE, 0) == -1 ) {
                    fprintf(stderr, "ERROR: by reiving data from server\n");
                    exit(EXIT_FAILURE);
                }

                switch ( server_reply[0] ) {
                    case '\0':   
                        #ifdef DEBUG
                        printf("Reply from server is empty (next state -> ENDCOM)\n");
                        #endif
                        state = ENDCOM;
                    break;

                    default: 
                        state = DATAPRINT;
                    break; 
                }
            break;
            
            // recieving data state
            case ONLYREQ:
                if ( recv(client_socket, server_reply, BUFSIZE, 0) == -1 ) {
                    fprintf(stderr, "ERROR: by recieving data from server\n");
                    exit(EXIT_FAILURE);
                }

            // print data state
            case DATAPRINT:
                if ( server_reply[0] == '\0' ) {
                    state = ENDCOM;
                    break;
                }
                printf("%s\n", server_reply);
                
                // pri prepinaci -n a -f posielam serveru len 1 ziadost o data
                if ( opt == NAME || opt == PATH ) {
                    state = ENDCOM;
                    break;
                }

                // sending server request for next data
                if ( ( send(client_socket, "1", 2, 0) ) == -1 ) { // "1" is DATAREQ flag
                    fprintf(stderr, "ERROR: by sending reqv for data in DATAPRINT to server\n");
                    exit(EXIT_FAILURE);
                }
        
                state = ONLYREQ;
            break;
            
            // end state
            case ENDCOM:    
               #ifdef DEBUG    
                printf("Communiction is ending\n");
                #endif
                run = false; // flag for ending this cycle for exec this client request

                // sending server request for ending communication
                if ( ( send(client_socket, "2", 2, 0) ) == -1 ) { // "2" is CONECTCLOSE flag
                    fprintf(stderr, "ERROR: by sending reqv for data in DATAPRINT to server\n");
                    exit(EXIT_FAILURE);
                }
            break; 
        }
    }
    #ifdef DEBUG
    printf("Request sent to server\n");
    #endif
    

    // close client socket for communication
    close(client_socket);
    
    return 0;
}