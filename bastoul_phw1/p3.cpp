#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <iostream>
#include <chrono>

using namespace std::chrono;

#define h  800 
#define w  800

#define input_file  "input.raw"
#define output_file "output.raw"

int main(int argc, char** argv){
    //int i;
    FILE *fp;

  	unsigned char *a = (unsigned char*) malloc (sizeof(unsigned char)*h*w);
    
  	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	// the matrix is stored in a linear array in row major fashion
	if (!(fp=fopen(input_file, "rb"))) {
		printf("can not open file\n");
		return 1;
	}

	fread(a, sizeof(unsigned char), w*h, fp);
	fclose(fp);
    

	int kmeans[4] = {0, 85, 170, 155};
	int sum[4];
	int count[4];
	int min;

	int cluster;
	bool last = false;
	

	for (int itr = 0; itr < 31; itr++) {
		if (itr==30) {
			last = true;
		}

		for (int i = 0; i<4; ++i) {
			sum[i] = 0;
			count[i] = 0;
		}


		for (int i = 0; i < 640000; ++i) {
			min = 255;
			for (int j = 0; j < 4; ++j) {
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

		for (int i = 0; i<4; i++) {
			kmeans[i] = sum[i]/count[i]; 
		}
		
	}
	
	
	//////
	
	
	
	if (!(fp=fopen(output_file,"wb"))) {
		printf("can not open file\n");
		return 1;
	}	
	fwrite(a, sizeof(unsigned char),w*h, fp);
    fclose(fp);

    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    auto duration = duration_cast<nanoseconds>( t2 - t1 ).count();
    double timeSec = (double) duration/1000000000;

	std::cout << "Execution time = " << duration << " nanoseconds\n";
    
    return 0;
}