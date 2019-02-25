/**
 * @description  Client - meter or server - reflector
 * @author       Jakub Kubik 
 * @date         8.4.2018 */

#include <iostream>
#include <vector>
#include <cmath>
#include "ipk-mtrip.h"
#include "reflect.h"
#include "meter.h"
#include "timer.h"

#include <thread>
#include <climits>


int Client(int argc, char *argv[]);
int Server(int argc, char *argv[]);

using namespace std;


int main( int argc, char *argv[] ) {

    if ( strcmp(argv[1], "meter") == 0 ) {

        if ( Client(argc, argv) == ERR ) return ERR;
    }
    else if ( strcmp(argv[1], "reflect") == 0 ) {

        if ( Server(argc, argv) == ERR ) return ERR;
    }
    else {

        cerr << "Wrong parameter " << argv[1] << endl;
        return ERR;
    }
    
    return OK;
}


int Client(int argc, char *argv[]) {

    MeterArguments arg; // object for processing meter arguments

    if ( !(arg.checkArg(argc, argv)) ) return ERR;
    #ifdef DEBUG
    cout << "Host: " << arg.getHost() << ", Port: " << arg.getPort()
         << ", Velkost sondy: "<< arg.getSondaSize() << ", Cas: " << arg.getTime() << endl;
    #endif

    Meter client; // object for creating client

    if ( !client.run(arg) ) return ERR;
    #ifdef DEBUG
        cout << "Meter(client) is running" << endl;
    #endif
    

    Timer RTTtimer;                  
    int RTTsum = 0;
    int RTTcount = 0;                        // number of streams and number of measured RTT
    Timer Stream;                            // timer for one stream
    unsigned long long T = DEFAULTPERIOD;    // default period 
    float maxSpeed = 0;
    float minSpeed = INT_MAX;
    float avgSpeed;
    vector<float> allSpeed;                  // for counting odchylku
    Timer programRun;                        // timer for whole client

    programRun.start();
    while( programRun.elapsedSeconds() <= arg.getTime() ) {  
    
        client.setMsg(arg.getSondaSize());  // set the msg len according program arguments

        // sending one stream
        Stream.start();
        for ( int i = 0; i < PACKETINSTREAM; i++ ) {  
            std::this_thread::sleep_for(std::chrono::nanoseconds(T));
            client.sendUDP();   
        }
        unsigned long stream = Stream.elapsedMicroseconds();
        Stream.stop();
        
        // send end msg for one stream to server and cout 
        // from it actual RTT
        RTTtimer.start();
        client.sendTCP();
        client.recvTCP();
        RTTsum += RTTtimer.elapsedMicroseconds();
        RTTtimer.stop();
        RTTcount++;

        // get number of acknowledged packets from msg from server        
        char *msg = client.getMsg();
        msg += sizeof(char);
        long int ackPackets = strtol(msg, NULL, 10);
        msg -= sizeof(char);
        
        #ifdef DEBUG
        cout << "\n\n\nrecvd msg from server: " << client.getMsg() << endl;
        cout << "clients reaction: " << endl;
        #endif
        if ( client.getMsg()[0] == '2' ) {  // increase speed
            #ifdef DEBUG
            cout << "increase speed by decreasing waiting time for packet\n" << endl;
            #endif
            if ( T <= 50 ) {
                T = 0;    // for ability to increase time later
            } 
            else T -= TIMECHANGEDOWN;
        }
        else if ( client.getMsg()[0] == '3' ) { // decrease speed
            #ifdef DEBUG
            cout << "decrease speed by increasing waiting time for packet\n" << endl;
            #endif
            T += TIMECHANGEUP;
        } 
        double sec = stream/1000.0/1000.0;  // converting to seconds
        double BitSize = arg.getSondaSize()*BYTE*PACKETINSTREAM;
        double speedBit = BitSize / sec;
        double speedMbit = speedBit / 1024.0/1024.0;    // converting to Mb
        
        cout << "Stream: " << RTTcount << endl;
        cout << "----------------------------------------------------" << endl;
        //cout << "Number of acknowledged packets: " << ackPackets << endl;
        cout << "Duration of one stream: " << sec << endl;
        cout << "Actual speed:" << speedBit << "b/s " << speedMbit << "Mb/s" << endl;
        cout << "time to stop for packet: " << T << endl << endl;
        
        if ( speedMbit > maxSpeed ) maxSpeed = speedMbit;
        if ( speedMbit < minSpeed ) minSpeed = speedMbit;
        allSpeed.push_back(speedMbit);

            
    }   // end of mesuring speed and everithing
    cout << "Run time of program: " << programRun.elapsedMilliseconds() / 1000.0 << " Seconds" << endl; 
    programRun.stop();
    cout << "Number of streams: " << RTTcount << endl << endl;
    
    // ending of whole server not only one stream
    const char *msgs = "1";
    client.setMsg(69);
    client.sendTCP(); 


    cout << "===============================================================" << endl;
    cout << "================== RESULTS OF MEASURING =======================" << endl;
    cout << "===============================================================" << endl;
    cout << "\tAverage RTT:\t\t     " << RTTsum/float(RTTcount) / 1000.0 << " Milliceconds" << endl;
    cout << "\tMAX speed:\t\t     " << maxSpeed << " Mb/s" << endl;
    cout << "\tMIN speed:\t\t     " << minSpeed << " Mb/s" << endl;
    for(std::vector<float>::iterator it = allSpeed.begin(); it != allSpeed.end(); ++it)
        avgSpeed += *it;
    avgSpeed /= float(RTTcount);
    cout << "\tAverage speed:\t\t     " << avgSpeed << " Mb/s" << endl;
    double standOd = 0;
    for(std::vector<float>::iterator it = allSpeed.begin(); it != allSpeed.end(); ++it)
        standOd += (*it-avgSpeed)*(*it-avgSpeed);
    standOd /= RTTcount;
    cout << "\tStandard deviation of speed: " << sqrt(standOd) << " Mb/s" << endl << endl;
    
    client.closeSockets(); 
}

int Server(int argc, char *argv[]) {

    ReflectArguments arg; // class for reflect arguments

    if ( !(arg.checkArg( argc, argv )) ) return ERR;
    #ifdef DEBUG
    cout << "Port number: " << arg.getPortNmb() << endl;
    #endif

    Reflect server; // object for server

    if ( !server.run(arg) ) {
        cerr << "ERROR: running server" << endl;
        return ERR;
    }
    #ifdef DEBUG
        cout << "Reflector(server) is runnig";
    #endif
   
   
    server.acceptingTCP();

    int rcvdPackets = 0;    // number of recieved packets
    int retval;             // retval for select option

    while (true)  {
        fd_set readSockSet;

        FD_ZERO(&readSockSet);
        FD_SET(server.serverSocketUDP, &readSockSet);
        FD_SET(server.pipe, &readSockSet);

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
        
        // selecting which packet is in input stream
        retval = select(FD_SETSIZE, &readSockSet, NULL, NULL, &timeout);
        if ( retval == -1 ) {
            cerr << "ERROR by select" << endl;
            break;
        }
        else if ( retval == 0 ) {
            #ifdef DEBUG
            cout << "Timeout ..." << endl;
            #endif
            continue;
        }
        // UDP packet recieved
        else if (FD_ISSET(server.serverSocketUDP, &readSockSet)) {

            server.recvUDP();
            rcvdPackets++;
            #ifdef DEBUG
            cout << "UDP Client query incoming..." << endl;
            #endif
        } 
        // TCP packets revieved
        else if (FD_ISSET(server.pipe, &readSockSet)) {
            
            server.recvTCP();
            #ifdef DEBUG
            cout << "TCP Client requesting to connect..." << endl;
            #endif
            if ( server.getMsg()[0] == '1' ) break; // end of communnication(closing server)

            // 5% loss of packets do I accept
            char okej[10];
            char chyba[10];
            sprintf(okej, "2%d", rcvdPackets);  // setting up reply to client
            sprintf(chyba, "3%d", rcvdPackets);
                
            float percentage = rcvdPackets / 100.0;
            if ( percentage < 0.95 ) {
                server.setMsg(chyba);    // msg = decrease speed
            }
            else server.setMsg(okej); // msg = increase speed
            server.sendTCP();

            #ifdef DEBUG
            cout << "message to send: " << server.getMsg() << endl;
            cout  << " percentage of succ UDP: " << percentage << " packets: " << rcvdPackets << endl;
            #endif
            rcvdPackets = 0;
        } 
    }  

    server.closeSockets();
    return OK;
}
