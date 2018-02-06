#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <mutex>

int shelf;
bool producerDone;
int consumerDone;
pthread_mutex_t shelfMutex;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *producerFunc(void *filler) {
	bool cookieOnShelf;
	int totalPut = 0;
	bool broadcast;
	while (totalPut<30 && !(shelf==10 && consumerDone==0)) {
		cookieOnShelf = false;
		broadcast = false;
		while (!cookieOnShelf) {
			pthread_mutex_lock(&shelfMutex);
			while (shelf==10)
    			pthread_cond_wait (&cond, &shelfMutex);
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
			} if (shelf == 2) {
				broadcast = true;
			}
			pthread_mutex_unlock (&shelfMutex);
			if (broadcast) pthread_cond_broadcast(&cond);

		}
	}
	producerDone = true;

	pthread_exit(NULL);
}

void *consumerFunc(void *id_t) {
	int id = (int) id_t;
	bool cookieTaken;
	int totalTaken = 0;
	bool notify;
	while (totalTaken<15 && !(shelf==0 && producerDone)) {
		cookieTaken = false;
		notify = false;
		while (!cookieTaken) {
			pthread_mutex_lock(&shelfMutex);
			while (shelf==0)
    			pthread_cond_wait (&cond, &shelfMutex);
			if (shelf > 0) {
				shelf--;
				totalTaken++;
				cookieTaken = true;
				std::cout << "Consumer " << id << " has taken 1 cookie; # cookies on shelf changes from " <<
				shelf+1 << " to " << shelf << ".\n";
			} if (shelf == 9) notify = true;  
			pthread_mutex_unlock (&shelfMutex);
			if (notify) pthread_cond_signal(&cond);
		}
	}
	consumerDone--;

	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	shelf = 0;
	producerDone = false;
	consumerDone = 2;

	pthread_mutex_init(&shelfMutex, NULL);
	pthread_cond_init(&cond, NULL);

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