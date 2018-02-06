#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <mutex>
#include <pthread.h>

int shelf;

void *producerFunc(void *filler) {

	pthread_exit(NULL);
}

void *consumerFunc(void *filler) {

	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	shelf = 0;

	pthread_t *threads = new pthread_t[3];

	int rc;
	int filler;
	for (int t=0; t<3; t++) {
		if (t==2) {rc = pthread_create(&threads[t], NULL, consumerFunc, (void*) filler);}
		else {rc = pthread_create(&threads[t], NULL, producerFunc, (void*) filler);}
		 
		if (rc) {
			std::cout << "creating error" <<"\n";
			exit(-1);
		}
	}

	return 0;
}

