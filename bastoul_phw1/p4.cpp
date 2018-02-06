#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <chrono>

using namespace std::chrono;

struct pt {
	double x;
	double y;
};

int main(int argc, char *argv[]){
	int NUM_OF_POINTS = 100;

	//std::cout << ((double) rand() / (RAND_MAX)) << "\n";
	
	for ( ; NUM_OF_POINTS<=1000000; NUM_OF_POINTS*=10 ) {
		pt *points = new pt[NUM_OF_POINTS]; 

		high_resolution_clock::time_point t1 = high_resolution_clock::now();

		for (int i = 0; i < NUM_OF_POINTS; ++i)	{
			points[i].x = ((double) rand() / (RAND_MAX));
			points[i].y = ((double) rand() / (RAND_MAX));
		}

		int numInCircle = 0;
		for (int i = 0; i < NUM_OF_POINTS; ++i)	{

			if ( sqrt( pow((points[i].x - 0.5), 2) + pow((points[i].y - 0.5), 2) ) <= 0.5 ) {
				numInCircle++;
			}
		}
		double area = (double) numInCircle/NUM_OF_POINTS;
		double pi = area/0.25;

		std::cout << "With " << NUM_OF_POINTS << " points...\n";

		high_resolution_clock::time_point t2 = high_resolution_clock::now();

	    auto duration = duration_cast<nanoseconds>( t2 - t1 ).count();
	    double timeSec = (double) duration/1000000000;

		std::cout << "Execution time = " << duration << " nanosecs\n";
	
		std::cout << "Pi ~ " << pi << "\n\n";

	}


	return 0;
}