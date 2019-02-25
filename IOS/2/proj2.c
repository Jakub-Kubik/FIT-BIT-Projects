/*
 * @decription  Modified child care synchronization problem 
 * @author      Jan Jakub Kubik (xkubik32@stud.fit.vutbr.cz)
 * @date        1.5.2017 */

#include "proj2.h"


int main( int argc, char *argv[] ) {
  
  srand(time(NULL)); 

  // Program arguments
  struct arguments checked_arg;
  int err = check_arguments( argc, argv, &checked_arg );
  if ( err == ERROR_ARG ) {
    error( ERROR_ARG );
  }

  open_sources(checked_arg); // allocate shared memory and create semaphores
                             // semaphores are in "/dev/shm"

 pid_t MasterParent;   // children of main process
 pid_t Parent;         // children of MasterParent

 pid_t MasterChildren; // children of main process
 pid_t Children;       // children of MasterChildren 


 MasterParent = fork(); 
 if ( MasterParent == -1 ) {
    fprintf(stderr, "Wasnt created process for generating Parents\n");
    close_sources();
    error(ERROR_PROCES);
 }
 else if ( MasterParent == 0 ) {

    // generate all parent processes
    for ( int i = 1; i <= checked_arg.A; i++ ) {

        // sleep MasterParent and then create Parent process
        if ( checked_arg.AGT != 0 ) {
          usleep( rand() % checked_arg.AGT * TOMILISECOND);
        }

        // generate parent process
        Parent = fork();
        if ( Parent == 0 ) {
          Parent_f(i, checked_arg);   // do all important action with Parent process
        }
       else if ( Parent == -1 ) {
         close_sources();
         error(ERROR_PROCES);
       }
    }
 }
 else { // I am in main process (parent of MasterParent and parent of MasterChildern)
   MasterChildren = fork(); 
   if ( MasterChildren == -1 ) {
     fprintf(stderr, "Wasn't created process for generating Children processes\n");
     close_sources();
     error(ERROR_PROCES);
   }
   else if ( MasterChildren == 0 ) {
   // in cycle generate al children processes
      for ( int i = 1; i <= checked_arg.C; i++ ) {

          // sleep MasterChild and then create Children process
          if ( checked_arg.CGT != 0 ) {
            usleep( rand() % checked_arg.CGT * TOMILISECOND);
          }

          // generate children process
          Children = fork();
          if ( Children == 0 ) {
            Child_f(i, checked_arg); // do all important action with Children process
          }
          else if ( Children == -1 ) {
            error(ERROR_PROCES);
          }
     }
   }

   // execution of MasterChlidren
   else {
     // Parent process (MasterChildren) waits here for child (Chlidren) to terminate
     for ( int i = 0; i < checked_arg.C; i++)
        wait(NULL);
   }

   // Parent process (MasterParent) waits here for child (Parent) to terminate
   for ( int i = 0; i < checked_arg.A; i++)
      wait(NULL);
  }
  // Main process is waiting to terminate MasterParent and MasterChlidren
  waitpid(MasterParent, NULL, 0);
  waitpid(MasterChildren, NULL, 0);

  // close all semaphores and free shared memory
  close_sources();

  return 0;
}


void Child_f(int idOfChld, struct arguments checked_arg) {

  sem_wait(writeToFile);
  fprintf(fw, " %-6d\t: C %-4d: started\n", (*actionId)++, idOfChld);
  sem_post(writeToFile);

  // enter chldren without any restriction or else
  sem_wait(mutex);
  if (( (((*chldInC) ) < 3 * (*adultInC)) || ((*adultToEnter == 0))   )) {

    sem_wait(writeToFile);
    fprintf(fw, " %-6d\t: C %-4d: enter\n", (*actionId)++, idOfChld);
    (*chldInC)++;
    sem_post(writeToFile);

    sem_post(mutex);
  }
  else {
    sem_wait(writeToFile);
    fprintf(fw, " %-6d\t: C %-4d: waiting : %-2d: %-2d\n", (*actionId)++, idOfChld, *adultInC, *chldInC);
    sem_post(writeToFile);

    (*chldIsWaiting)++;
    sem_post(mutex);

    // child process is waiting to space in center
    sem_wait(childCome);

    // process is enterning
    sem_wait(writeToFile);
    fprintf(fw, " %-6d\t: C %-4d: enter\n", (*actionId)++, idOfChld);
    (*chldInC)++;
    (*chldIsWaiting)--;
    sem_post(writeToFile);
  }

  // process simulate working if required time isnt 0
  if ( checked_arg.CWT != 0 ) {
    usleep( rand() % checked_arg.CWT * TOMILISECOND);
  }

  // process try to leave center
  sem_wait(mutex);
  sem_wait(writeToFile);
  fprintf(fw, " %-6d\t: C %-4d: trying to leave\n", (*actionId)++, idOfChld);
  sem_post(writeToFile);

  // process left center
  sem_wait(writeToFile);
  fprintf(fw, " %-6d\t: C %-4d: leave\n", (*actionId)++, idOfChld);
  (*chldInC)--;
  (*chldToEnter)--;
  sem_post(childCome);

  if ( *chldInC <= 3 * (*adultInC - 1)  ) sem_post(adultToLeave);
  sem_post(writeToFile);

  sem_post(mutex);

  // semaphore for ending all process together
  if ( ((*chldToEnter) == 0) && ((*adultToEnter) == 0) && (*chldInC == 0) && (*adultInC == 0) ) {
    for ( int i = 0; i < checked_arg.C + checked_arg.A; i++ )
      sem_post(endAllProcess);
  }
  else {
        sem_wait(endAllProcess);
  }
  // process has finished its existence
  sem_wait(writeToFile);
  fprintf(fw, " %-6d\t: C %-4d: finished\n", (*actionId)++, idOfChld);
  sem_post(writeToFile);

  exit(0);
}


void Parent_f(int idOfParrent, struct arguments checked_arg) {

  sem_wait(writeToFile);
    fprintf(fw, " %-6d\t: A %-4d: started\n", (*actionId)++, idOfParrent);
  sem_post(writeToFile);


  sem_wait(writeToFile);
  fprintf(fw, " %-6d\t: A %-4d: enter\n", (*actionId)++, idOfParrent);
  (*adultInC)++;
  (*adultToEnter)--;

  sem_post(childCome);  // MAKING SPACE FOR 3 CHILDREN
  sem_post(childCome);
  sem_post(childCome);
  sem_post(writeToFile);


  // process is sismulating working if required time isnt 0
  if ( checked_arg.AWT != 0 ) {
    usleep( rand() % checked_arg.AWT * TOMILISECOND);
  }

  sem_wait(mutex); 

  // process try to leave center
  sem_wait(writeToFile);
  fprintf(fw, " %-6d\t: A %-4d: trying to leave\n", (*actionId)++, idOfParrent);
  sem_post(writeToFile);

  // in center is enough adults and process can leave or else
  if ( (*chldIsWaiting == 0) && (( ( 3 * ((*adultInC) - 1)) >= (*chldInC) ) )  ) {

    sem_wait(writeToFile);
    fprintf(fw, " %-6d\t: A %-4d: leave\n", (*actionId)++, idOfParrent);
    (*adultInC)--;

    sem_post(writeToFile);
    sem_post(mutex);
  }
  else {
    // process is waiting
    sem_wait(writeToFile);
    fprintf(fw, " %-6d\t: A %-4d: waiting : %-2d: %-2d\n", (*actionId)++, idOfParrent, *adultInC, *chldInC );
    sem_post(writeToFile);
    sem_post(mutex);

    // lock adult process to leave
    sem_wait(adultToLeave);


    if ( *chldIsWaiting != 0  ) sem_wait(adultToLeave);

    // in center i enough adults this can leave
    sem_wait(writeToFile);
    fprintf(fw, " %-6d\t: A %-4d: leave\n", (*actionId)++, idOfParrent);
    (*adultInC)--;

    sem_wait(childCome);  // TAKING SPACE FOR 3 CHILDREN
    sem_wait(childCome);
    sem_wait(childCome);

    sem_post(writeToFile);
  }

  // for avoiding deadlock when no adults to enter center
  if ( ( *adultToEnter == 0 ) && (*adultInC == 0) ) {
    for (int i = 0; i < checked_arg.C; i++) {
      sem_post(childCome);
    }
  }

  // semaphore for ending all process together
  if ( ((*adultToEnter) == 0) && ((*chldToEnter) == 0) && (*adultInC == 0) && (*chldInC == 0) ) {
    for ( int i = 0; i < checked_arg.C + checked_arg.A; i++ )
      sem_post(endAllProcess);
  }
  else {
      sem_wait(endAllProcess);
  }
  // process is ending its existence
  sem_wait(writeToFile);
    fprintf(fw, " %-6d\t: A %-4d: finished\n", (*actionId)++, idOfParrent);
  sem_post(writeToFile);

  exit(0);
}


void open_sources(struct arguments checked_arg) {

  // open file for output
  fw = fopen("proj2.out", "w");
  if ( fw == NULL ) {
    fprintf(stderr, "File for writing cant open\n");
    error(ERROR_FILE);
  }

  setbuf(fw, NULL); // set buffer for output to file*/

  // creating semaphores (int /dev/shm)
  endAllProcess = sem_open("/xkubik32_endAllProcess", O_CREAT| O_EXCL, 0666, 0);
  writeToFile = sem_open("/xkubik32_writeToFile", O_CREAT| O_EXCL, 0666, 1);
  childCome = sem_open("/xkubik32_childCome", O_CREAT | O_EXCL, 0666, 0  );
  adultToLeave = sem_open("/xkubik32_adultToLeave", O_CREAT | O_EXCL, 0666, 0);
  mutex = sem_open("/xkubik32_mutex", O_CREAT | O_EXCL, 0666, 1);

  // check right creation of semaphores
  if ( endAllProcess == SEM_FAILED
    || writeToFile == SEM_FAILED || childCome == SEM_FAILED
    || adultToLeave == SEM_FAILED || mutex == SEM_FAILED )
    {
      fprintf(stderr, "Error by creating semaphores\n");
      error(ERROR_PROCES);
    }

  // creating shared memory
  //
  // id of current action
  if ( (shm_action = shmget(IPC_PRIVATE, sizeof(int),
     IPC_CREAT | IPC_EXCL | 0666)) == -1 ) {
       error(ERROR_PROCES);
     }
  if ( (actionId = (int *)shmat(shm_action, NULL, 0)) == NULL ) {
    error(ERROR_PROCES);
  }
  *actionId = 1; // default initialization


  // number of adult in center
  if ( (shm_adult = shmget(IPC_PRIVATE, sizeof(int),
     IPC_CREAT | IPC_EXCL | 0666)) == -1  ) {
       error(ERROR_PROCES);
     }
  if ( (adultInC = (int *)shmat(shm_adult, NULL, 0)) == NULL ) {
    error(ERROR_PROCES);
  }
  *adultInC = 0; // default initialization

  // number of childre in center
  if ( (shm_chld = shmget(IPC_PRIVATE, sizeof(int),
     IPC_CREAT | IPC_EXCL | 0666)) == -1 ) {
       error(ERROR_PROCES);
     }
  if ( (chldInC = (int *)shmat(shm_chld, NULL, 0)) == NULL ) {
    error(ERROR_PROCES);
  }
  *chldInC = 0; // default initialization

  // number of adult process to enter to the center
  if ( (shm_adultEnter = shmget(IPC_PRIVATE, sizeof(int),
     IPC_CREAT | IPC_EXCL | 0666)) == -1  ) {
       error(ERROR_PROCES);
     }
  if ( (adultToEnter = (int *)shmat(shm_adultEnter, NULL, 0)) == NULL ) {
    error(ERROR_PROCES);
  }
  *adultToEnter = checked_arg.A;

  // number of chlidren process to enter to the center
  if ( (shm_chldEnter = shmget(IPC_PRIVATE, sizeof(int),
     IPC_CREAT | IPC_EXCL | 0666)) == -1  ) {
       error(ERROR_PROCES);
     }
  if ( (chldToEnter = (int *)shmat(shm_chldEnter, NULL, 0)) == NULL ) {
    error(ERROR_PROCES);
  }
  *chldToEnter = checked_arg.C;

  // front for waiting children process to enter
  if ( (shm_chldIsWaiting = shmget(IPC_PRIVATE, sizeof(int),
     IPC_CREAT | IPC_EXCL | 0666)) == -1  ) {
       error(ERROR_PROCES);
     }
  if ( (chldIsWaiting = (int *)shmat(shm_chldIsWaiting, NULL, 0)) == NULL ) {
    error(ERROR_PROCES);
  }
  *chldIsWaiting = 0;

  // front for waiting adult process to leave
  if ( (shm_adultIsWaiting = shmget(IPC_PRIVATE, sizeof(int),
     IPC_CREAT | IPC_EXCL | 0666)) == -1  ) {
       error(ERROR_PROCES);
     }
  if ( (adultIsWaiting = (int *)shmat(shm_adultIsWaiting, NULL, 0)) == NULL ) {
    error(ERROR_PROCES);
  }
  *adultIsWaiting = 0;

}


void close_sources(void) {

  // close file for writing
  fclose(fw);

  // dealocation of shared memory
  if ( shmctl(shm_action, IPC_RMID, NULL) == -1 ) {
    error(ERROR_PROCES);
  }
  if ( shmctl(shm_adult, IPC_RMID, NULL) == -1 ) {
    error(ERROR_PROCES);
  }
  if ( shmctl(shm_chld, IPC_RMID, NULL) == -1 ) {
    error(ERROR_PROCES);
  }
  if ( shmctl(shm_adultEnter, IPC_RMID, NULL) == -1 ) {
    error(ERROR_PROCES);
  }
  if ( shmctl(shm_chldEnter, IPC_RMID, NULL) == -1 ) {
    error(ERROR_PROCES);
  }
  if ( shmctl(shm_adultIsWaiting, IPC_RMID, NULL) == -1 ) {
    error(ERROR_PROCES);
  }
  if ( shmctl(shm_chldIsWaiting, IPC_RMID, NULL) == -1 ) {
    error(ERROR_PROCES);
  }

  // free semaphores
  sem_unlink("xkubik32_endAllProcess");
  sem_unlink("xkubik32_writeToFile");
  sem_unlink("xkubik32_childCome");
  sem_unlink("xkubik32_adultToLeave");
  sem_unlink("xkubik32_mutex");

  sem_close(endAllProcess);
  sem_close(writeToFile);
  sem_close(childCome);
  sem_close(adultToLeave);
  sem_close(mutex);
}


int check_arguments( int argc, char *argv[], struct arguments * checked_arg ) {

  char *ptr = NULL;                 // poiter for strtol
  float check;                      // checking if number is int not float

  if ( argc != NMB_OF_REQ_ARG ) {
      return ERROR_ARG;
  }
  else {

    // 1. argument 
    checked_arg->A = strtol(argv[1], &ptr, 10);
    if ( *ptr != '\0' ) {
      return ERROR_ARG;
    }
    check = strtof(argv[1], NULL);
    if ( (checked_arg->A - check) != 0 ) {
      return ERROR_ARG;
    }
    // 2. argument 
    checked_arg->C = strtol(argv[2], &ptr, 10);
    if ( *ptr != '\0' ) {
      return ERROR_ARG;
    }
    check = strtof(argv[2], NULL);
    if ( (checked_arg->C - check) != 0 ) {
      return ERROR_ARG;
    }
    // 3 .argument
    checked_arg->AGT = strtol(argv[3], &ptr, 10);
    if ( *ptr != '\0' ) {
      return ERROR_ARG;
    }
    check = strtof(argv[3], NULL);
    if ( (checked_arg->AGT - check) != 0 ) {
      return ERROR_ARG;
    }
    // 4. argument
    checked_arg->CGT = strtol(argv[4], &ptr, 10);
    if ( *ptr != '\0' ) {
      return ERROR_ARG;
    }
    check = strtof(argv[4], NULL);
    if ( (checked_arg->CGT - check) != 0 ) {
      return ERROR_ARG;
    }
    // 5. argument
    checked_arg->AWT = strtol(argv[5], &ptr, 10);
    if ( *ptr != '\0' ) {
      return ERROR_ARG;
    }
    check = strtof(argv[5], NULL);
    if ( (checked_arg->AWT - check) != 0 ) {
      return ERROR_ARG;
    }
    // 6. arguemnt
    checked_arg->CWT = strtol(argv[6], &ptr, 10);
    if ( *ptr != '\0' ) {
      return ERROR_ARG;
    }
    check = strtof(argv[6], NULL);
    if ( (checked_arg->CWT - check) != 0 ) {
      return ERROR_ARG;
    }
    // check down values of all arguments
    if (   checked_arg->A <= 0 || checked_arg->C <= 0 ||
          checked_arg->AGT < 0 || checked_arg->CGT < 0 ||
          checked_arg->AWT < 0 || checked_arg->CWT < 0      ) {
      return ERROR_ARG;
    }
    // check upper values of all arguments
    if (  checked_arg->AGT >= 5001 || checked_arg->CGT >= 5001 ||
          checked_arg->AWT >= 5001 || checked_arg->CWT >= 5001      ) {
      return ERROR_ARG;
    }
  }
  return NO_ERROR;
}


void help(void) {

 printf("\n");
 printf("==========================================================================\n");
 printf("                      Author: Jakub Kubik (xkubik32)\n");
 printf("==========================================================================\n");
 printf("USING OF PROGRAM:\n");
 printf("--------------------------------------------------------------------------\n");
 printf("\t./proj2 A C AGT CGT AWT CWT\n\n");
 printf("\t!!! ALL NUMBERS ARE INTEGER !!!\n\n");
 printf("\t\tA   -> number of adult process, (A > 0)\n");
 printf("\t\tC   -> number of child process, (C > 0)\n");
 printf("\t\tAGT -> max time for generating new ADULT process\n"
         "\t\t\t(AGT >= 0 && AGT < 5001)\n     ");
 printf("\t\tCGT -> max time for generating new CHILD process\n"
         "\t\t\t(CGT >= 0 && CGT < 5001)\n     ");
 printf("\t\tAWT-> max time for ADULT process to simulate working\n"
         "\t\t\t(AWT >= 0 && AWT < 5001)\n ");
 printf("\t\tCWT -> max time for ADULT process to simulate working\n"
         "\t\t\t(CWT >= 0 && CWT < 5001)\n ");
 printf("\n==========================================================================\n");

}


void error(int n) {

 if ( n == ERROR_ARG ) {
   fprintf(stderr, "\nError with arguments to program\n");
   help();
   exit(1);
 }
 else if ( n == ERROR_PROCES ) {
   fprintf(stderr, "\nError with processes\n");
   exit(2);
 }
}
