/**
 * @decription  header file form proj2.c
 * @author      Jan Jakub Kubik (xkubik32@stud.fit.vutbr.cz)
 * @date        1.5.2017 */

#ifndef PROJ2_H
#define PROJ2_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <semaphore.h>

// @brief    structure for processing and ussing all argumnts
struct arguments
{
  int A;   // @brief   number of adult process; A > 0
  int C;   // @brief   number of children process; C > 0
  int AGT; // @brief   max value after whicch i generate new process A; AGT >=0 && AGT < 5001
  int CGT; // @brief   max value after which i generate new process C; CGT >=0 && CGT < 5001
  int AWT; // @brief   max value to simualte working A in center; AWT >= 0 && AWT < 5001
  int CWT; // @brief   max value to simulate working C in center; CwT >= 0 && CWT < 5001
} ;
// @brief    file descriptor
  FILE *fw;

// semaphores
sem_t *endAllProcess;      // @brief   semaphore for clossing all generated processes at the end
sem_t *writeToFile;        // @brief   semaphore for writting action to file
sem_t *childCome;          // @brief   semaphore for leting chlidren enter to center
sem_t *adultToLeave;       // @brief   semaphore for letting adult leave center
sem_t *mutex;              // @brief   semaphore for helping


// Shared variables
int *actionId;         // @brief   number of action for writing to file
int *adultInC;         // @brief   number of adult in center
int *chldInC;          // @brief   number of children in center
int *adultToEnter;     // @brief   number of adult to leave center to end all process
int *chldToEnter;      // @brief   number of children to leave center to end all process
int *chldIsWaiting;    // @brief   number of children process waiting in front to enter to center
int *adultIsWaiting;   // @brief   number of adult process waiting in front to leve center
// Space for shared variables
int shm_action;
int shm_adult;
int shm_chld;
int shm_adultEnter;
int shm_chldEnter;
int shm_chldIsWaiting;
int shm_adultIsWaiting;


// @decription number of arguumnents
#define NMB_OF_REQ_ARG    7

// @description error codes
#define NO_ERROR      0
#define ERROR_ARG     1
#define ERROR_PROCES  2
#define ERROR_FILE    3


// @decription for generating process and for working process (for convering time)
#define TOMILISECOND  1000


/**
 * @brief               check all passed arguments and save them to structure
 * @param argc          number of arguments
 * @param *argv[]       array with strings of all arguments
 * @param *checked_arg  structure for saving arguments if is everithing ok
 * @return code */
int check_arguments( int argc, char *argv[], struct arguments *checked_arg );


/** @brief print help to my program */
void help(void);


/** @brief print error message */
void error(int n);


/**
 * @brief              open all required semaphores and create shared memory
 * @param checked_arg  structure with checked arguments */
void open_sources(struct arguments checked_arg);  


/** @brief close all semaphores and shared memory */
void close_sources(void);


/**
 * @brief              all important action for parent process
 * @param idOfParrent  identification number of current parent process
 * @param checked_arg  structure with checked arguments */
void Parent_f(int idOfParrent, struct arguments checked_arg);


/**
 * @brief              all important action for children process
 * @param idOfChld     identification number of current children process
 * @param checked_arg  structure with checked arguments */
void Child_f(int idOfChld, struct arguments checked_arg);

#endif
