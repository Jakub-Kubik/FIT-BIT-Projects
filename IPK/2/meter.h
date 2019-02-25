/**
 * @description  header for meter.cpp
 * @author       Jakub Kubik 
 * @date         8.4.2018 */

#ifndef METER_H
#define METER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <stdbool.h>
#include <netinet/in.h>


/*./ipk-mtrip meter -h vzdáleny_host -p vzdálený_port - s velikost_sondy -t doba_mereni

    vzdáleny_host - doménové jméno nebo IP adresa stanice, na které je reflektor spuštěn.
    vzdálený_port - číslo portu, na kterém je reflektor spuštěn.
    velikost_sondy - velikost dat použitých v "probe" paketu
    doba_mereni - celový čas měření */
class MeterArguments {  // class for parsing meter(client) arguemnts

    public:
        // method for running all init methods
        bool checkArg( int argc, char *argv[] );
        
        // simple methods for getting value from object
        std::string getHost()       { return host; };
        int getPort()               { return remotePort; };
        unsigned int getSondaSize() { return sondaSize; };
        unsigned int getTime()      { return time; } ;
        
    protected:
        std::string host; 
        int remotePort;
        unsigned int sondaSize;
        unsigned int time;

    private:
        // check and save host name
        bool setHost( int argc, char *argv[] );
        // check and save port number
        bool setPort( int argc, char *argv[] );
        // check and set sonda size
        bool setSondaSize( int argc, char *argv[] );
        // check and set time
        bool setTime( int argc, char *argv[] );
};

// class for meter(client)
class Meter { 

    public:
        // method for default init of object
        bool run(MeterArguments arg);

        void sendUDP();
        void recvUDP();
        void sendTCP();
        void recvTCP();
        void closeSockets();

        // simple methods for getting and setting object attributes
        char *getMsg() { return buffer; };
        void setMsg(int size);

            int pipes;

    private:
    
        char *ipServer;
        int clientSocketUDP;
        int clientSocketTCP;
        struct sockaddr_in serverAddress;
        char buffer[BUFSIZE];
        socklen_t serverlen = sizeof(serverAddress);
        struct hostent *hostentStruct; // for DNS conversion

        bool createSockets();
        bool convertDNStoIP(MeterArguments arg);
        bool TCPconnect();
        void init(MeterArguments arg);
};

#endif