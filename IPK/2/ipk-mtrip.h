/**
 * @description  header file for ipk-mtrip.cpp
 * @author       Jakub Kubik 
 * @date         8.4.2018 */

#ifndef ARGUEMNTS_H
#define ARGUEMNTS_H

//#define DEBUG           // for helpful print

#define ERR 99              // error has occured in executing program
#define OK 0                // no error has occured in executing program

#define MAXPORT 65535      // max port value
#define MINPORT 0          // min port value 

#define REFLECTARG 4    // number of server(reflector) arguments
#define METERARG 10      // number of client(meter) arguments

#define BUFSIZE 65536    // sizeof buffer for sending and revc msg on sever and client side

#define DEFAULTPERIOD 10000 // default period for waiting
#define PACKETINSTREAM 100
#define TIMECHANGEUP  60
#define TIMECHANGEDOWN 50
#define BYTE 8


/**
 * @brief          create and manipulate client object for bandwith measurement
 * @param *argv[]  all program arguments
 * @param argc     number of program arguments
 * @return         status code */
int Client(int argc, char *argv[]);


/**
 * @brief          create and manipulate server object for packet reflection
 * @param *argv[]  all program arguments
 * @param argc     number of program arguments
 * @return         status code */
int Server(int argc, char *argv[]);

#endif