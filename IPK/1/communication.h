/**
 * @description  comunication options
 * @author       Jakub Kubik 
 * @date         12.3.2018 */

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#define BUFSIZE 1500

// flags for sending requests from client to server
#define DATAREQ_FLAG       1
#define CONECTCLOSE_FLAG   2

// flags for sending requests from server to client
#define  DATAPRINT_FLAG    3
#define ALLDATA_FLAG       5 // all data are sent by server


// allowed options
#define NAME    1
#define PATH    2
#define ALLUSR  3

#endif