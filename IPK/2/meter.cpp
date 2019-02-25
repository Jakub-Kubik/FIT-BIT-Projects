/**
 * @description  arg and client
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
#include <chrono>
#include <thread>

#include "ipk-mtrip.h"
#include "meter.h"

using namespace std;


// @description MeterArgumnets class
bool MeterArguments::checkArg( int argc, char *argv[] ) {

    if ( argc != METERARG              || 
         !(setHost( argc, argv ))      || 
         !(setPort(argc, argv))        ||
         !(setSondaSize( argc, argv )) ||
         !(setTime(argc, argv)) ) {
            cerr << "ERROR:worng arguments\n";
            return false;
        }
    else return true;
}


bool MeterArguments::setHost( int argc, char *argv[] ) {

    string referal = "-h";
    if ( referal.compare(argv[2]) == 0 ) {
        host = argv[3];
        return true;
    }
    else return false;
}


bool MeterArguments::setPort( int argc, char *argv[] ) {

    string referal = "-p";
    if ( referal.compare(argv[4]) == 0 ) {
        remotePort = stoi(argv[5]);
        // check port value
        if ( remotePort > MAXPORT || remotePort < MINPORT ) 
            return false;
        return true;
    }
    else 
        return false;
}


bool MeterArguments::setSondaSize( int argc, char *argv[] ) {

    string referal = "-s";
    if ( referal.compare(argv[6]) == 0 ) {
        sondaSize = stoi(argv[7]);
        if ( sondaSize > 65507 ) {
            cerr << "ERROR to big datapart for packet " << sondaSize << endl;
            return false;  
        }
        return true;
    }
    else 
        return false;
}


bool MeterArguments::setTime( int argc, char *argv[] ) {

    string referal = "-t";
    if ( referal.compare(argv[8]) == 0 ) {
        time = stoi(argv[9]);       
        return true;
    }
    else 
        return false;
}


// @description Meter class
bool Meter::run(MeterArguments arg) {

    if ( !createSockets() || !(convertDNStoIP(arg)) ) return false;
    init(arg);
    if ( !TCPconnect() ) return false;
    return true;
}


bool Meter::createSockets() {

    if ( (clientSocketUDP = socket(AF_INET, SOCK_DGRAM, 0)) <= 0 ) {
        cerr << "ERROR: by creating UDP client socket" << endl;
        return false;
    }
    if ((clientSocketTCP = socket(AF_INET , SOCK_STREAM , 0)) <= 0) {
        cerr << "ERROR: creating TCP client socket" << endl;
        return false;
    }
    return true;
}

void Meter::closeSockets() {

    close(clientSocketUDP);
    close(clientSocketTCP);
}


bool Meter::convertDNStoIP(MeterArguments arg) {

    struct hostent *hostentStruct;

    hostentStruct = gethostbyname(arg.getHost().c_str());   // c_str() converts c++ string to const char*
    if ( hostentStruct == NULL ) {
        cerr << "ERROR: by using function gethostbyname" << endl;
        return false;
    }
    ipServer = inet_ntoa(*((struct in_addr *)hostentStruct->h_addr_list[0]));
    if ( ipServer == NULL ) {
        fprintf(stderr, "ERROR: cannot conver DNS name to IP\n");
        return false;
    }
    #ifdef DEBUG
    printf("Hostname: %s Host IP: %s\n", arg.getHost().c_str(), ipServer);
    #endif

    return true;
}


bool Meter::TCPconnect() {

    if (connect(clientSocketTCP , (struct sockaddr *)&serverAddress , sizeof(serverAddress)) != 0) {
        cout << errno <<endl;
        fprintf(stderr, "connect failed. Error\n");
        return false;
    }
    return true;
}


void Meter::init(MeterArguments arg) {

    serverAddress.sin_addr.s_addr = inet_addr(ipServer);
    serverAddress.sin_family = AF_INET; 
    serverAddress.sin_port = htons( arg.getPort() );
    #ifdef DEBUG
	printf("value for connection: %s:%d\n", inet_ntoa(serverAddress.sin_addr), ntohs(serverAddress.sin_port));
	#endif
}   


void Meter::sendUDP() {

    int sentBytes = sendto(clientSocketUDP, buffer, strlen(buffer), 0,
                    (struct sockaddr *) &serverAddress, serverlen); // +1 for '\0'
    if ( sentBytes == -1 ) {
        cerr << "ERROR: cannot send UDP data to server" << endl;
        exit(EXIT_FAILURE);
    }
}


void Meter::recvUDP() {

    int recvBytes = recvfrom(clientSocketUDP, buffer, strlen(buffer), 0,
                (struct sockaddr *) &serverAddress, &serverlen);
    if ( recvBytes == -1 ) {
        cerr << "ERROR: cannot recv UDP data from server" << endl;
        exit(EXIT_FAILURE);
    }
    #ifdef DEBUG
    cout << "recieved UDP message from server: " << buffer << endl;
    #endif
}


void Meter::sendTCP() {
    int sentBytes = send(clientSocketTCP, buffer, 5, 0);   // +1 for '\0'
    if ( sentBytes == -1 ) {
        cerr << "ERROR: cannot send TCP data to server" << endl;
        exit(EXIT_FAILURE);
    }
}


void Meter::recvTCP() {

    buffer[0] = '\0';
    int recvByte = recv(clientSocketTCP, buffer, BUFSIZE, 0);
    if ( recvByte == -1 ) {
        cerr << "ERROR: cannot recv TCP data from server" << endl;
        exit(EXIT_FAILURE);
    }   
}


void Meter::setMsg(int size) {
    
    string msg;
  
    if ( size == 69 ) {
        msg = "1";
        buffer[0] = '1';
        buffer[1] = '\0';
    }
    
    msg.clear();
    if ( size != 69 ) {
        for ( int i = 0; i < size; i++  ) {
            msg.append("0");
        }
        strcpy(buffer, msg.c_str());
    }
    
    #ifdef DEBUG
    cout << "\n\n\n\n\n\n#####################################################" << endl;
    cout << "sizeof str to send: " << strlen(buffer) << " " << buffer << endl;
    cout << "\n\n\n\n\n\n#####################################################" << endl; 
    #endif 
}
