/********
 *
 *      File Name: Multithread.c
 *
 *      Name of team members:
 *
 *      Marcel Riera
 *
 *      Anthony Sanchez-Ayra.
 *
 *
 *      How to run the program:
 *
 *      To run this program please make sure that the makefile
 *      is in the same directory as this source file. 
 *
 *      To compile this file just type make.
 *
 *      Another way to compile the file is by typing the
 *      following commands:
 *
 *      gcc Multithread.c  -o Multithread.out -lpthread
 *
 *      Afterwards to execute the program please use the
 *      command below followed by a number.
 *
 *      ./Multithread.out [ ENTER NUMBER HERE ]
 *
 *      For example:
 *
 *      ./Multithread.out 1
 *
 *      If the input you provide is not followed by a number
 *      the program will give you an error and describe its
 *      expected input.
 *
 *      There are two ways to ru3n this program:
 *
 *      Without synchronization of the pthreads:
 *
 *      To run the program without synchronization make
 *      sure that the preproccessed variable PTHREAD_SYNC
 *      is set to 0.
 *
 *      PTHREAD_SYNC = 0;
 *
 *      To run the program with synchronization make sure
 *      that the preproccess variable PTHREAD_SYNC is set
 *      to 1.
 *
 *      PTHREAD_SYNC = 1;
 *
 *      When toggling with synchronization please make sure
 *      that you compile the program.
 *
 *      It is known that our program has warnings when we
 *      compile, but it runs fine regardless.
 *
 * ****/

#include <stdio.h> // Includes standard input library for C.

#include <stdlib.h> // Includes the standard library for C.


#include <pthread.h> // Includes the pthread library to initialize thread variables.

#include <unistd.h> // Needed on my machine to use usleep().

#define INVALID_ARGUMENTS -1 // Whenever an invalid argument is found, -1 is returned.

#define EXPECTED_ARGUMENTS 2 // Number of expected arguments from command line input.

#define PTHREAD_SYNC 1  // VARIABLE THAT TOGGLES SYNCHRONIZATION.


// Function prototypes
int validateArguments(int argc, char *argv[]);
void *simpleThread(void *threadArgs);

// Global pthread variables for the mutex and barrier.
pthread_mutex_t mutexSum;
pthread_barrier_t myBarrier;

// The shared memory variable the pthreads will be modifying.
int sharedVariable = 0;

int main(int argc, char *argv[]) 
{
	const int THREAD_AMOUNT = validateArguments(argc, argv); // Gather cmd line input


	if (THREAD_AMOUNT == INVALID_ARGUMENTS) 
        {

	   printf("Invalid arguments: command line expects 1 argument > 0\n");
	   
           printf("e.g. \"./Multithread.out 4\"\n");
	
           return EXIT_SUCCESS;

	}
        
        // Initializes the variables necessary for synchronization.
        if ( PTHREAD_SYNC == 1 )
        {

            pthread_mutex_init ( &mutexSum, NULL );

            pthread_barrier_init ( &myBarrier, NULL, THREAD_AMOUNT  );

        }

	pthread_t *threadIDs = calloc(THREAD_AMOUNT, sizeof(pthread_t)); // Allocate array of thread IDs
	
        int curThread;

	setbuf(stdout, NULL); // Delete stdout buffer, similar to fflush after printf

	for (curThread = 0; curThread < THREAD_AMOUNT; curThread++) // Run threads
        {
           pthread_create(&threadIDs[curThread], NULL, simpleThread, (void*) ( curThread ));
        }

	pthread_exit(0);  // Wait for all threads to end

	free(threadIDs);

        // Removes the varaibles used for synchronization.
        if ( PTHREAD_SYNC == 1 )
        {

           pthread_mutex_destroy ( &mutexSum );

           pthread_barrier_destroy ( &myBarrier );

        }

	return EXIT_SUCCESS;
}

/*********** validateArguments ( length, input )***
 *   |
 *   |   Function: validateArguments
 *   |
 *   |   Parameters:
 *   |
 *   |   length: The length of the amount of
 *   |           inputs entered by the user.
 *   |
 *   |   input: The input that the user entered
 *   |          through the command line.
 *   |
 *   |   return: validatedInput - An integer
 *   |           that specifies if the input
 *   |           given by the user was valid or
 *   |           not.
 *   |
 **********************************************/
int validateArguments( int length, char *input[] ) {
	
        int validatedInput = 0;

	if ( length != EXPECTED_ARGUMENTS )
        {
		return INVALID_ARGUMENTS;
        }
	else 
        {
           validatedInput = atoi( input [ 1 ] );

	   if ( validatedInput <= 0 )
           {
	      return INVALID_ARGUMENTS;
           }
	
           return validatedInput;

	}

}

/*********** simpleThread ( threadArgs  ) ******
 *   |
 *   |   Function: simpleThread
 *   |
 *   |   Parameters:
 *   |
 *   |   threadId: A pointer to number
 *   |             of the thread.
 *   |           
 *   |
 *   |   return: None.
 *   |
 *   |   Note: There are two variations of this
 *   |         method. The first variation is
 *   |         the unsynchronized version of it.
 *   |         To access the usynchronized version
 *   |         please make sure the preproccessed
 *   |         varaible PTHREAD_SYNC is set to
 *   |         0 as follows:
 *   |
 *   |         PTHREAD_SYNC = 0;
 *   |
 *   |         If you would like to run the 
 *   |         synchronized version then please
 *   |         make sure that the PTHREAD_SYNC
 *   |         variable is set to 1 as follows:
 *   |
 *   |         PTHREAD_SYNC = 1;
 *   |
 *   |         Make sure to compile the file
 *   |         everytime the varaibles are
 *   |         changed!
 *   |
 *   |
 **********************************************/
void * simpleThread( void *threadArgs ) 
{
	int which = ( int ) ( threadArgs );
	
        int num, val;        

	for (num = 0; num < 20; num++) {

           //Run unsynchronized verssion of threads.

           if ( PTHREAD_SYNC == 0 )
           {
		if ( random() > RAND_MAX / 2 )
			usleep( 500 );

		val = sharedVariable;
		printf( "***Thread %d sees value %d\n", which, val );
		sharedVariable = val + 1;

          }
          else //Run synchronized version of threads.
          {

             pthread_mutex_lock ( &mutexSum );

             val = sharedVariable;

             printf( "***Thread %d sees value %d\n", which, val );

             sharedVariable = val + 1;

             pthread_mutex_unlock ( &mutexSum ); 

          }

	}

        pthread_barrier_wait( &myBarrier );

	val = sharedVariable;

	printf( "Thread %d sees final value %d\n", which, val );

}

