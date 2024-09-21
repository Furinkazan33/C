#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>

void *f1(void* vargp) {
	sleep(2);
	printf("Printing from Thread \n");
	return NULL;
}

void *f2(void* vargp) {
	sleep(4);
	printf("Printing from Thread \n");
	return NULL;
}

int main()
{
	pthread_t pids[2];

	pthread_create(&pids[0], NULL, f1, 0);
	pthread_create(&pids[1], NULL, f2, 0);

	for(size_t i = 0; i < 2; i++) {
		pthread_join(pids[i], NULL);
		printf("join of %ld\n", pids[i]);
	}

	exit(0);
}


