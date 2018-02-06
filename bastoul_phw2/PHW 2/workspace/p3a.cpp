#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <mutex>

int shelf;
pthread_mutex_t shelfMutex;
bool producerDone;
bool consumerDone;

void *producerFunc(void *filler) {
	bool cookieOnShelf;
	int totalPut = 0;
	while (totalPut<30 && !(shelf==10 && consumerDone)) {
		cookieOnShelf = false;
		while (!cookieOnShelf) {
			pthread_mutex_lock(&shelfMutex);
			if (shelf < 9) {
				shelf+=2;
				totalPut+=2;
				cookieOnShelf = true;
				std::cout << "Producer has put 2 cookies; # cookies on shelf changes from " <<
				shelf-2 << " to " << shelf << ".\n";
			} else if (shelf == 9) {
				shelf++;
				totalPut++;
				cookieOnShelf = true;
				std::cout << "Producer has put 1 cookie; # cookies on shelf changes from " <<
				shelf-1 << " to " << shelf << ".\n";
			}
			pthread_mutex_unlock (&shelfMutex);
		}
	}
	producerDone = true;

	pthread_exit(NULL);
}

void *consumerFunc(void *id_t) {
	int id = (int) id_t;
	bool cookieTaken;
	int totalTaken = 0;
	while (totalTaken<15 && !(shelf==0 && producerDone)) {
		cookieTaken = false;
		while (!cookieTaken) {
			pthread_mutex_lock(&shelfMutex);
			if (shelf > 0) {
				shelf--;
				totalTaken++;
				cookieTaken = true;
				std::cout << "Consumer " << id << " has taken 1 cookie; # cookies on shelf changes from " <<
				shelf+1 << " to " << shelf << ".\n";
			} 
			pthread_mutex_unlock (&shelfMutex);
		}
	}
	consumerDone = true;
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	shelf = 0;
	producerDone = false;
	consumerDone = false;

	pthread_mutex_init(&shelfMutex, NULL);

	pthread_t *threads = new pthread_t[3];

	int rc;
	int id;
	for (int t=0; t<3; t++) {
		id = t+1;
		if (t==2) {rc = pthread_create(&threads[t], NULL, producerFunc, (void*) id);}
		else {rc = pthread_create(&threads[t], NULL, consumerFunc, (void*) id);}
		 
		if (rc) {
			std::cout << "creating error" <<"\n";
			exit(-1);
		}
	}

	for (int t=0; t<3; t++) {
		pthread_join(threads[t], NULL);
	}

	return 0;
}