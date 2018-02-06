#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <pthread.h>


void *printHello(void * thread_id) {
	long tid = (long) thread_id;
	std::cout << "Hello from thread " << tid << "\n";
	pthread_exit(NULL);
}

int main() {
	pthread_t threads[5];
	int rc;

	pthread_attr_t attr;
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for (int t=0; t<5; t++) {
		std::cout << "Creating thread " << t << "\n";
		rc = pthread_create(&threads[t], NULL, printHello, (void *) t);
		if (rc) {
			std::cout << "error" <<"\n";
			exit(-1);
		}
		rc = pthread_join(threads[t], NULL);
		if (rc) {
			std::cout << "joining error" <<"\n";
			exit(-1);
		}
	}
	pthread_attr_destroy(&attr);
	pthread_exit(NULL);
	return 0;
}