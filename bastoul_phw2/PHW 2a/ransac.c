#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NUM_OBS 800000
#define input_file "dataset_ransac.txt"
#define MSS_SIZE 2 //MInimum Sample Set Size is 2
#define THRESHOLD 0.0001

//Declared as global variables as stack size cannot support this much memory requirement.
float observations[NUM_OBS][2]; //x and y co-ordinate of each observation
float inliers[NUM_OBS][2] = {0};

int main() {
	
	int num_inliers = 0;	

	FILE* fp;
	
	if (!(fp=fopen(input_file, "rb"))) {
		printf("can not opern file\n");
		return 1;
	}

	int i=0;
	for (i=0;i < NUM_OBS; i++)
		fscanf(fp,"%f %f\n", &observations[i][0], &observations[i][1]);

	fclose(fp);

	struct timespec start, stop; 
	double exe_time;
	if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}


	//Randomly select points for MSS

	float mss_points[MSS_SIZE][2];
	int rand_index = rand() % NUM_OBS;
	srand(time(NULL));
	mss_points[0][0] = observations[rand_index][0];
	mss_points[0][1] = observations[rand_index][1];

	rand_index = rand() % NUM_OBS;
	while (observations[rand_index][0] == mss_points[0][0]) {
		rand_index = rand() % NUM_OBS;
	}
	
	mss_points[1][0] = observations[rand_index][0];
	mss_points[1][1] = observations[rand_index][1];

	//Fit a model using the MSS
	float m = (mss_points[1][1] - mss_points[0][1])/(mss_points[1][0] - mss_points[0][0]);
	float c = (mss_points[0][1]*mss_points[1][0] - mss_points[1][1]*mss_points[0][0])/(mss_points[1][0] - mss_points[0][0]);


	//Testing Phase
	for (i = 0; i < NUM_OBS; i++) {
		float epsilon = (observations[i][1] - m*observations[i][0] - c)*(observations[i][1] - m*observations[i][0] - c);
		if (epsilon < THRESHOLD) {
			inliers[num_inliers][0] = observations[i][0];
			inliers[num_inliers][1] = observations[i][1];
			num_inliers++;
		}

	} 

	if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}		

	exe_time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;
	
	printf("Number of Inliers: %d, Parameters values: m = %f, c = %f", num_inliers, m, c);
	printf("\nExecution time = %f sec\n",  exe_time);

	return 0;
		
}
