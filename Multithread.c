#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // Needed on my machine to use usleep(), may be removed later

#define THREAD_AMOUNT 4 // For testing, later get from command line arguments

int sharedVariable = 0;

void *simpleThread(void *threadArgs) {
	int which = *((int*)threadArgs);
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

int main() {
	pthread_t threadID;
	int curThread;
	
	setbuf(stdout, NULL); // Delete stdout buffer, similar to fflush after printf
	for (curThread = 0; curThread < THREAD_AMOUNT; curThread++)
		pthread_create(&threadID, NULL, simpleThread, (void*) &curThread);
	pthread_exit(0);
}
