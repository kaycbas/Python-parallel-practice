#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <iostream>
#include <chrono>
#include <pthread.h>

using namespace std::chrono;

#define h  1024 
#define w  1024
#define input_size h*w

#define input_file  "input.raw"
#define output_file "output.raw"

bool last;

struct Thread_data {
	int *kmeans_t;
	int *sum_t;
	int *count_t;
	unsigned char* a_t;
	int start_t;
	int run_t;
};

void *threadFunc(void *threadArg) {
	struct Thread_data * t_data;
	t_data = (struct Thread_data*) threadArg;

	int *kmeans = t_data->kmeans_t;
	int *sum = t_data->sum_t;
	int *count = t_data->count_t;
	unsigned char* a = t_data->a_t;
	int start = t_data->start_t;
	int run = t_data->run_t;

	int min;
	int cluster;

	for (int i = start; i < start+run; ++i) {
		min = 255;
		for (int j = 0; j < 6; ++j) {
			if (abs(a[i] - kmeans[j]) < min) {
				min = abs(a[i] - kmeans[j]);
				cluster = j;
			}
		}
		sum[cluster] += a[i];
		count[cluster]++;
		if (last) {
			a[i] = kmeans[cluster];
		}
	}
	pthread_exit(NULL);
}


int main(int argc, char** argv){
    //int i;
    FILE *fp;

  	unsigned char *a = (unsigned char*) malloc (sizeof(unsigned char)*h*w);

  	// getting cmd line input for num threads

	if (argc < 2) {
		std::cout << "Please retry with numThreads as a command line parameter.\n";
		return 0;
	} 
	int numThreads = atoi(argv[1]);
	if (numThreads != 1 && numThreads != 4 && numThreads != 8 && numThreads != 1024) {
		std::cout << "Please retry with a value of 4 or 8.\n";
		return 0;
	}

	int kmeans[6] = {0, 65, 100, 125, 190, 255};
	int sum[6];
	int count[6];
	int min;

	int cluster;
	last = false;

	// initialize thread array

	pthread_t *threads = new pthread_t[numThreads];

	//
	// initialize thread_data_array

	Thread_data *thread_data_array = new Thread_data[numThreads];
	for (int t=0; t<numThreads; t++) {
		thread_data_array[t].kmeans_t = kmeans;
		thread_data_array[t].sum_t = sum;
		thread_data_array[t].count_t = count;
		//thread_data_array[t].a_t = a;
		thread_data_array[t].start_t = t*(input_size/numThreads);
		thread_data_array[t].run_t = input_size/numThreads;
	}

	//
	
	//

	// the matrix is stored in a linear array in row major fashion
	if (!(fp=fopen(input_file, "rb"))) {
		printf("can not open file\n");
		return 1;
	}


	fread(a, sizeof(unsigned char), w*h, fp);
	fclose(fp);

	for (int t=0; t<numThreads; t++) {
		thread_data_array[t].a_t = a;
	}
    

	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	for (int itr = 0; itr < 51; itr++) {
		if (itr==50) {
			last = true;
		}

		for (int i = 0; i<6; ++i) {
			sum[i] = 0;
			count[i] = 0;
		}


		int rc;
		for (int t=0; t<numThreads; t++) {
			rc = pthread_create(&threads[t], NULL, threadFunc, (void*) &thread_data_array[t]); 
			if (rc) {
				std::cout << "creating error" <<"\n";
				exit(-1);
			}
		}

		for (int t=0; t<numThreads; t++) {
			pthread_join(threads[t], NULL);
		}

		
		for (int i = 0; i<6; i++) {
			kmeans[i] = sum[i]/count[i]; 
		}
		
	}
	
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	
	if (!(fp=fopen(output_file,"wb"))) {
		printf("can not open file\n");
		return 1;
	}	
	fwrite(a, sizeof(unsigned char),w*h, fp);
    fclose(fp);

    auto duration = duration_cast<nanoseconds>( t2 - t1 ).count();
    double timeSec = (double) duration/1000000000;

	std::cout << "Execution time = " << duration << " nanoseconds\n";

    //pthread_exit(NULL);
    
    return 0;
}