/*
 * Thread_Timer.c
 *
 * This is a program which spawns as many threads as defined and each will
 * Increment an inernal value form zero to the defined amount. The total amount
 * of time taken for the thread counters is output to the console.
 * 
 * Rony Vargas
 */

#include <pthread.h>
#include <stdio.h>
//#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <linux/types.h>
#include <inttypes>
//#include <stdint.h>

#define TOTAL_THREADS (1000) /* Defines total number of thread to create */
#define COUNT_TIL (10000000) /* Defines Number each thread should count to */


 /* This function takes in 2 timeval structs and returns the difference */
uint64_t lapsed_usec(struct timeval * start, struct timeval *end){
	uint64_t bt, be;  /* start, end times as 64 bit integers */

	bt =  (uint64_t) start->tv_sec *  (uint64_t)(1000000) + (uint64_t )start->tv_usec;
	be =  (uint64_t) end->tv_sec *  (uint64_t)(1000000) + (uint64_t ) end->tv_usec;

	if (be >= bt) { /* make sure we don't return a negative time */
		return (be-bt);
	}
	else {
		printf("server: lapsed_usec: warning, negative time interval\n");
		return 0;
	}
}

 /* This function counts from 0 to the defined amoount */
void *counter(void * none)
{
	/* increment x to COUNT_TILL */
	int x = 0;
	while(++x < COUNT_TIL);
	return NULL;
}

int main()
{

	int i = 0;

	/* start, end time to compute bandwidth */
	struct timeval start_time; 
	struct timeval end_time;
	uint64_t lapsed_useconds;
	double lapsed_seconds;


	printf("Program Start, Attempting to create threads \n");
	pthread_t thread_array[TOTAL_THREADS];

	/* Save the current time as start time */
	gettimeofday(&start_time, (struct timezone *) NULL);


	/* Create threads on each block of array */
	for (i=0; i < TOTAL_THREADS; i++){
		if (pthread_create((pthread_t *)&thread_array[i], NULL, counter, NULL)){
			fprintf(stderr, "Error creating thread\n");
			return 1;
		}
	}

	printf("Threads created waiting on return \n");

	/* Join all threads that were spawned */
	for (i=0; i < TOTAL_THREADS; i++){
		pthread_join(thread_array[i], NULL);

	}

	gettimeofday(&end_time, (struct timezone *) NULL);
	lapsed_useconds = lapsed_usec(&start_time, &end_time);
	lapsed_seconds = (double) lapsed_useconds / (double) 1000000;
	printf("Total Run Time: %lf sec \n" , lapsed_seconds );

	return 0;
}
