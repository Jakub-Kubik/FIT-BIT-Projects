/**
 * @description  implementation of server for reflection of data
 * @author       Jakub Kubik 
 * @date         8.4.2018 */

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <stdbool.h>
#include <netinet/in.h>

#include "ipk-mtrip.h"
#include "reflect.h"


// @description ReflectArguments class
bool ReflectArguments::checkArg( int argc, char *argv[] ) {

    if ( (argc != REFLECTARG) || !(setPortNmb( argc, argv )) ) {
        cerr << "ERROR: wrong arguments or port number" << endl;
        return false;
    }
    else return true;
}


bool ReflectArguments::setPortNmb( int argc, char *argv[] ) {

    string referal = "-p";
    if ( referal.compare(argv[2]) == 0 ) {
        portNmb = stoi(argv[3]);
        cout << "Port: " << portNmb << endl;
        // check port value
        if ( portNmb > MAXPORT || portNmb < MINPORT ) return false;
        return true;
    }
    else return false;
}


// @description Reflect class
bool Reflect::run(ReflectArguments arg) {

    if ( !createSockets() || !init(arg) ) return false;
    return true;
}


bool Reflect::createSockets() {

    if ( (serverSocketUDP = socket(AF_INET, SOCK_DGRAM, 0)) <= 0 ) {
        cerr << "ERROR: by creating UDP server socket" << endl;
        return false;
    }
    if ( (serverSocketTCP = socket(AF_INET, SOCK_STREAM, 0)) <= 0 ) {
		cerr << "ERROR: by creating TCP server socket" << endl;
		return false;
	}
    #ifdef DEBUG
    cout << "Socket created" << endl;
    #endif
    return true;
}


void Reflect::closeSockets() {
    close(serverSocketUDP);
    close(serverSocketTCP);
}


bool Reflect::init(ReflectArguments arg) {

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(arg.getPortNmb());
    //cout << "Port kto priradujem je " << arg.getPortNmb() << endl;
    server.sin_addr.s_addr = INADDR_ANY;
    

    // assaign port number to UDP server socket
    if ( bind(serverSocketUDP, (struct sockaddr *)&server, sizeof(server)) < 0 ) {
        cerr << "ERROR: UDP binding on server" << endl;
        return false;
    }
    #ifdef DEBUG
		printf("%s:%d prva connected\n", inet_ntoa(server.sin_addr), ntohs(server.sin_port));
	#endif

    // assaign port number to TCP server socket
    if ( bind(serverSocketTCP, (struct sockaddr *)&server, sizeof(server)) < 0 ) {
        cerr << "ERROR: TCP binding on server" << endl;
        return false;
    }
    #ifdef DEBUG
		printf("%s:%d connected\n", inet_ntoa(server.sin_addr), ntohs(server.sin_port));
		#endif

    // listeting on TCP socket (it means waiting for connection from client)
    if ( listen(serverSocketTCP, 5) < 0 ) {
		cerr << "ERROR: by TCP listening on server" << endl;
        return false;
	}
	#ifdef DEBUG
    cout << "Server binds succcess" << endl;
    printf("Server (TCP) is listening\n");
    #endif
   
    return true;
}


void Reflect::acceptingTCP() {
    
    unsigned int addrlen = sizeof(client);
    pipe = accept(serverSocketTCP, (struct sockaddr*)&client, &addrlen);
		if ( pipe == -1 ) {
			fprintf(stderr, "ERROR: by accepting on server\n");
			exit(EXIT_FAILURE);
		}
}


void Reflect::sendUDP() {

    int sentByte = sendto(serverSocketUDP, buffer, strlen(buffer), 0,
                (struct sockaddr *)&clientAddress, clientlen);
    if ( sentByte < 0 ) {
        cerr << "ERROR: cannot send UDP data to client" << endl;
    }   
}


void Reflect::recvUDP() {

    int recvByte = recvfrom(serverSocketUDP, buffer, strlen(buffer), 0,
                (struct sockaddr *) &clientAddress, &clientlen);
    if ( recvByte < 0 ) {
        cerr << "ERROR: cannot recv UDP data from client" << endl;
    }
}


void Reflect::sendTCP() {
    int sentBytes = send(pipe, buffer, 5, 0);   // +1 for '\0'
    if ( sentBytes == -1 ) {
        cerr << "ERROR: cannot send TCP data to client" << endl;
        exit(EXIT_FAILURE);
    }
}


void Reflect::recvTCP() {
    int recvByte = recv(pipe, buffer, 5, 0);
    if ( recvByte == -1 ) {
        cerr << "ERROR: cannot jebe recv TCP data from client" << endl;
        exit(EXIT_FAILURE);
    }
}


void Reflect::setMsg(char *msg) {
    buffer[0] = '\0';
    strcpy(buffer, msg);
}