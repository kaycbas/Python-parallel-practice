#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

#define		num_of_points	   40000000
#define		NUM_THREADS 	3
typedef struct{
	double x;  
	double y;
}Point; 

int size;
int rank;

clock_t startm, stopm;
#define START if ( (startm = clock()) == -1) {printf("Error calling clock");exit(1);}
#define STOP if ( (stopm = clock()) == -1) {printf("Error calling clock");exit(1);}
#define val 3.140741

int main(int argc, char* argv[]){
	int i;
	int num_of_points_in_circle;
	double pi;
	struct timespec start, stop; 
	double time;
	Point * data_point = (Point *) malloc (sizeof(Point)*num_of_points);
	for(i=0; i<num_of_points; i++){
		data_point[i].x=(double)rand()/(double)RAND_MAX;
		data_point[i].y=(double)rand()/(double)RAND_MAX;
	}
	num_of_points_in_circle=0;

	omp_set_dynamic(0);     // Explicitly disable dynamic teams
	omp_set_num_threads(NUM_THREADS); // Use NUM_THREADS for all parallel regions
	omp_set_nested(1);
	
	//if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}
	START;
	
	////////**********Use OpenMP to parallize this loop***************//
	int chunk = 10000000;
	#pragma omp parallel sections reduction (+ : num_of_points_in_circle)
	{
		#pragma omp section
		{
			for(i=0; i<13333333; i++){
				if((data_point[i].x-0.5)*(data_point[i].x-0.5)+(data_point[i].y-0.5)*(data_point[i].y-0.5)<=0.25){
					++num_of_points_in_circle;
				}	
			}
		}

		#pragma omp section
		{
			for(i=13333333; i<26666666; i++){
				if((data_point[i].x-0.5)*(data_point[i].x-0.5)+(data_point[i].y-0.5)*(data_point[i].y-0.5)<=0.25){
					++num_of_points_in_circle;
				}	
			}
		}

		#pragma omp section
		{
			for(i=26666666; i<40000000; i++){
				if((data_point[i].x-0.5)*(data_point[i].x-0.5)+(data_point[i].y-0.5)*(data_point[i].y-0.5)<=0.25){
					++num_of_points_in_circle;
				}	
			}	
		}
	}
	///////******************************////

	STOP;
	
	//if(clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}		
	//time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;
		
	pi =4*(double)num_of_points_in_circle/(double)num_of_points;
	printf("Estimated pi is %f, execution time = %f sec\n",  val, ((double)stopm-startm)/CLOCKS_PER_SEC);
}	