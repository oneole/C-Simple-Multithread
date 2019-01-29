#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // Needed on my machine to use usleep(), may be removed later

#define INVALID_ARGUMENTS -1
#define EXPECTED_ARGUMENTS 2

// Function prototypes
int validateArguments(int argc, char *argv[]);
void *simpleThread(void *threadArgs);

// Main program
int sharedVariable = 0;

int main(int argc, char *argv[]) {
	const int THREAD_AMOUNT = validateArguments(argc, argv); // Gather cmd line input
	if (THREAD_AMOUNT == INVALID_ARGUMENTS) {
		printf("Invalid arguments: command line expects 1 argument > 0\n");
		printf("e.g. \"./Multithread.out 4\"\n");
		return EXIT_SUCCESS;
	}

	pthread_t *threadIDs = calloc(THREAD_AMOUNT, sizeof(pthread_t)); // Allocate array of thread IDs
	int curThread;

	setbuf(stdout, NULL); // Delete stdout buffer, similar to fflush after printf

	for (curThread = 0; curThread < THREAD_AMOUNT; curThread++) // Run threads
		pthread_create(&threadIDs[curThread], NULL, simpleThread, (void*) ( curThread + 1 ));
	pthread_exit(0);  // Wait for all threads to end (I believe, I need to double check)
	free(threadIDs);
	return EXIT_SUCCESS;
}

// Functions
int validateArguments(int argc, char *argv[]) {
	int validatedInput = 0;
	if (argc != EXPECTED_ARGUMENTS)
		return INVALID_ARGUMENTS;
	else {
		validatedInput = atoi(argv[1]);
		if (validatedInput <= 0)
			return INVALID_ARGUMENTS;
		return validatedInput;
	}
}

void *simpleThread(void *threadArgs) {
	int which = (int)(threadArgs);
	int num, val;

	for (num = 0; num < 20; num++) {
		if (random() > RAND_MAX / 2)
			usleep(500);

		val = sharedVariable;
		printf("***Thread %d sees value %d\n", which, val);
		sharedVariable = val + 1;
	}

	val = sharedVariable;
	printf("Thread %d sees final value %d\n", which, val);
}
