#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>

#define NUM_OBS 400000
#define input_file "dataset_hough.txt"
#define NUM_THETA 11
#define PI 3.14159265

//Declared as global variables as stack size cannot support this much memory requirement.
float observations[NUM_OBS][2]; //x and y co-ordinate of each observation

int main() {

	FILE* fp;
	float accumulator[NUM_THETA][3];
	int i,j;	

	//Initialize accumulator Array
	for (i = 0; i < NUM_THETA; i++) {
		accumulator[i][0] = 15.0*i;
		accumulator[i][1] = 0.0;
		accumulator[i][2] = 0.0;
	}
	
	if (!(fp=fopen(input_file, "rb"))) {
		printf("can not opern file\n");
		return 1;
	}

	for (i=0;i < NUM_OBS; i++)
		fscanf(fp,"%f %f\n", &observations[i][0], &observations[i][1]);

	fclose(fp);

	struct timespec start, stop; 
	double exe_time;
	if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}

	//Iterate through each point
	for (i = 0; i < NUM_OBS; i++) {
		//Iterate through each theta and update the accumulator array
		for (j = 0; j < NUM_THETA; j++) {
			float r = observations[i][0]*cos(15.0*j/PI) + observations[i][1]*sin(15.0*j/PI);
			accumulator[j][1] += r;
			accumulator[j][2] += r*r;
		}
	}

	//Iterate through each entry of accumulator array and find the entry with least MSE
	float best_mse = FLT_MAX;	
	float best_r = -1;
	float best_theta = -1;
	for (i = 0; i < NUM_THETA; i++) {
		float epsilon = accumulator[i][2]/NUM_OBS - (accumulator[i][1]/NUM_OBS)*(accumulator[i][1]/NUM_OBS);
		if (epsilon < best_mse) {
			best_mse = epsilon;
			best_r = accumulator[i][2]/NUM_OBS;
			best_theta = 15.0*i;
		}
	}


	if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}		

	exe_time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;


	printf("Best R is %f and best Theta is %f\n", best_r, best_theta);
	printf("\nExecution time = %f sec\n",  exe_time);

	return 0;
}	
