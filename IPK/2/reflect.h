/**
 * @description  header file for reflet.cpp
 * @author       Jakub Kubik 
 * @date         8.4.2018 */

#ifndef REFLECT_H
#define REFLECT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <stdbool.h>
#include <netinet/in.h>

using namespace std;

/*./ipk-mtrip reflect -p port 
    port - číslo portu, na kterém bude reflektor spuštěn.*/

 // class for parsing reflector(server) arguments
class ReflectArguments {   

    public:
        // check number of arguments
        bool checkArg( int argc, char *argv[] );
        // getting value from private variable portNmb
        int getPortNmb( void ) { return portNmb; };

    protected:
        int portNmb;

    private:
        // check port numb and set it
        bool setPortNmb( int argc, char *argv[] );
};


// class for reflector (server)
class Reflect {

    public:
        // default method
        bool run(ReflectArguments arg);

        void sendUDP();
        void recvUDP();
        void sendTCP();
        void recvTCP();

        void closeSockets();

        // simple methods for getting and setting object attributes
        char * getMsg() { return buffer; };
        void setMsg(char * msg);

        void acceptingTCP();

        int serverSocketUDP;
        int serverSocketTCP;
         int pipe;

    private:
       
        struct sockaddr_in client;
        struct sockaddr_in server;
        char buffer[BUFSIZE];
        struct sockaddr_in clientAddress;
        socklen_t clientlen = sizeof(clientAddress);

        bool createSockets();
        bool init(ReflectArguments arg);
};

#endif