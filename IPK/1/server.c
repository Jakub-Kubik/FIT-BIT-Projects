/**
 * @description  main funciton for concurrent server (fork)
 * @author       Jakub Kubik 
 * @date         12.3.2018 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <netinet/in.h>

#include "server.h"
#include "communication.h"


int main(int argc, char *argv[])  {

    int portNmb;
    int retValue = parseArguments(argc, argv, &portNmb);
    if ( retValue != OK ) {
        fprintf(stderr, "ERROR by parsing arguments\n");
        return ERR;
    }
    #ifdef DEBUG
        printf("Arguments are OK\n");
        printf("Port number is: %d\n", portNmb);
    #endif

    int server_socket;
	struct sockaddr_in server;
	char buffer[BUFSIZE];

	// create server socket
    if ( (server_socket = socket(AF_INET, SOCK_STREAM, 0)) <= 0 ) {
		fprintf(stderr, "ERROR: creating server socket\n");
		exit(EXIT_FAILURE);
	}
	#ifdef DEBUG
    printf("Socket created\n");
    #endif 

	// default init of server port and address structure
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(portNmb);
	server.sin_addr.s_addr = INADDR_ANY;

	// assign port number to server socket
    if ( bind(server_socket, (struct sockaddr*)&server, sizeof(server)) < 0 ) {
		fprintf(stderr, "ERROR: bind in server\n");
		exit(EXIT_FAILURE);
	}
	#ifdef DEBUG
    printf("Server bind success\n");
    #endif 

	// listening socket
	if ( listen(server_socket, 5) < 0 ) {
		fprintf(stderr, "ERROR: by listening socket on server\n");
		exit(EXIT_FAILURE);
	}
	#ifdef DEBUG
    printf("Server is listening\n");
    #endif

	int pipe;
	struct sockaddr_in client;
	unsigned int addrlen=sizeof(client);
	char strToSend[200];	
	int pid;
	int state;
	
	// Concurent server
	while ( true ) {	

		// accept a connection and create a data pipe
		pipe = accept(server_socket, (struct sockaddr*)&client, &addrlen);
		if ( pipe == -1 ) {
			fprintf(stderr, "ERROR: by accepting on server\n");
			exit(EXIT_FAILURE);
		}
		#ifdef DEBUG
		printf("%s:%d connected\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
		#endif
		
		// forking for processing more than one requests
		if ((pid = fork()) == -1) { // error by forking
            close(pipe);
			fprintf(stderr, "ERROR: by creating fork\n");
            continue;
        }
        else if(pid == 0) {	// child process

			int opt = 99; 		// var for saving option in search 
								// for searching right data (99 is only default initialization)
			bool end = true;	// variable for processing one communication with client

			FILE *fr;
			if ( (fr = fopen("/etc/passwd", "r")) == NULL ) {
				fprintf(stderr, "ERROR: file-> /etc/passwd cant open\n");
				
				// i nedd to get data because without it i cannot send data to client
				end = false;
				if ( (recv(pipe, buffer, BUFSIZE, 0)) == -1 ) {
					fprintf(stderr, "ERROR: cant recv data from client by file openionig error\n");
					exit(EXIT_FAILURE);
				}
				if ( (send(pipe, "", 2, 0) ) == -1 ) {
					fprintf(stderr, "ERROR: send data to client by file opening error\n");
					exit(EXIT_FAILURE);
				}
			}
			state = INIT;
			

			// State machine for server
			while ( end ) {
				switch ( state ) {
				case INIT:
					;
					char login[100]; 	// variable for login to search in file
					login[0] = '\0';	
				
					if ( recv(pipe, buffer, BUFSIZE, 0) == -1 ) {
						fprintf(stderr, "ERROR: by reciving data from client\n");
						exit(EXIT_FAILURE);
					}
					#ifdef DEBUG
					printf("%s je flag, prepinac a login\n", buffer);
					#endif
					
					// data to search are on second byte in message from client
					if ( buffer[1] == '1' ) {	// -n
						opt = NAME;
					}
					else if ( buffer[1] == '2' ) {	// -f
						opt = PATH;
					}
					else if ( buffer[1] == '3' ) {	// -l
						opt = ALLUSR;
					}

					// remove first two bytes and save only login
					if ( buffer[2] != '\0' ) {
						strcpy(login, buffer+2);
						#ifdef DEBUG
						fprintf(stderr, "login to search is: %s\n", login);
						#endif
					}	
					
					int i = 0;	// index of str to process
					int j = 0;	// index of str to send

					
					// with function get one string with all requuired item that
					// i want to send to client
					//form: data:data:data:'\0';
					
					char *strToProcess = fileToStr(fr, login, opt);
					#ifdef DEBUG
					printf("string to process: %s\n", strToProcess);
					#endif
					
				case SEARCH:
					;
					bool cont = true;
					// get one value from string to process and sent it to client
					do {
						if ( strToProcess[i] == ':' ) {
							i++;
							cont = false;
						}
						else {
							strToSend[j] = strToProcess[i]; 
							j++;
							i++;
						}			
					} while(strToProcess[i] != '\0' && cont == true );
					strToSend[j] = '\0';
					j = 0;
					
					if ( (send(pipe, strToSend, strlen(strToSend) + 1, 0)) == -1 ) { // + 1 for '\0'
						fprintf(stderr, "ERROR: sending request to server for next data\n");
						exit(EXIT_FAILURE);
					} 
					if ((recv(pipe, buffer, BUFSIZE, 0)) == -1 ) {
						fprintf(stderr, "ERROR: recieving data from server\n");
						exit(EXIT_FAILURE);
					}

					if ( buffer[0] == '2' )  state = END;	// client wont require more data
					else state = SEARCH;
					
				break;

				case END:
				#ifdef DEBUG
					printf("END STATE\n\n");
				#endif
					fclose(fr);
					if ( strToProcess != NULL ) free(strToProcess);
					strToProcess = NULL;
					state = INIT;
					end = false;	// this one request is processed
				break;
				}
			}		
        }
		else { // parrent process
            close(pipe);
        }

		// close communication with client
		close(pipe);
	}

	// close socket for server
	close(server_socket);
	return 0;

}

char *fileToStr(FILE *fr, char *login, int opt)
{
	#define REALOC 200	// minimaly i need this anout of free space in
							// string for saving reqired items
	int countRealoc = 0;
	int new = 4000;
	int size = 4000;			// size of my str
	char *strToReturn = malloc(size * sizeof(char));
	if ( strToReturn == NULL ) {
		fprintf(stderr, "ERROR: strToReturn default allpcation\n");
		exit(EXIT_FAILURE);
	}
	strToReturn[0] = ':';	// default init of my str
	strToReturn[1] = '\0';	// this let me know in on line 188 that no more items is in this str

	char string[BUFSIZE];	// variable for saving lines from /etc/passwd

	int i = 0;
	int j = 0;
	int count = 0;

	if ( opt == NAME ) {	// -n sent client
		while( (fgets(string, BUFSIZE, fr)) != NULL  ) {
			// find match
			if ( strncmp(login, string, strlen(login)) == 0 ) {
				// find required element
				while ( count < 5 ) {
						if ( string[i] == ':' ) count++;
						else if ( count == 4 ) {
							strToReturn[j] = string[i];
							j++;
						}
						i++;
				}
				strToReturn[j] = ':'; 
				j++;
				strToReturn[j] = '\0'; 
				break;
			}
		}
	}
	else if ( opt == PATH ) {	// -f sent client
		while( (fgets(string, BUFSIZE, fr)) != NULL  ) {
			// find match
			if ( strncmp(login, string, strlen(login)) == 0 ) {
				// find required element
				while ( count < 6 ) {
					if ( string[i] == ':' ) count++;
					else if ( count == 5 ) {
						strToReturn[j] = string[i];
						j++;
					}
					i++;
				}
			strToReturn[j] = ':'; 
			j++;
			strToReturn[j] = '\0'; 
			break;
			}
		}
	}
	else if ( opt == ALLUSR ) {	// -l (whith login or without login) sent client
		bool reqLogin = false;
		if ( login[0] != '\0' ) {
			#ifdef DEBUG
			printf("login %s is required \n", login);
			#endif
			reqLogin = true;
		}
		bool match = true;
		while( (fgets(string, BUFSIZE, fr)) != NULL  ) {
			countRealoc++;
			#ifdef DEBUG
			printf("string from /etc/passwd: %s\n", string);
			#endif
			
			// if there is not enough space for saving all user names
			if (  countRealoc == 200 ) {
				countRealoc = 0;
				#ifdef DEBUG
				printf("Realocating memory\n");
				#endif

				char *tmp = realloc(strToReturn, strlen(strToReturn) + new);
				if (tmp == NULL) {
					fprintf(stderr, "ERROR: by realocating memory\n");
					exit(0);
				} else {
					strToReturn = tmp;
				}
				size += new;
				
			}
			// find match if needed
			if ( reqLogin ) {
				if ( strncmp(login, string, strlen(login)) != 0 ) match = false;
				else match = true;
			}
			if ( match ) {
				while ( string[j] != ':' ) {
                    strToReturn[i] = string[j];
                    i++;
					j++;
                }
				strToReturn[i] = ':'; 
				i++;
                strToReturn[i] = '\0';
				j = 0; 
			}
		}
	}
	return strToReturn;
}