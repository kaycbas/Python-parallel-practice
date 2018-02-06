#define _POSIX_C_SOURCE 199309L

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>


clock_t startm, stopm;
#define START if ( (startm = clock()) == -1) {printf("Error calling clock");exit(1);}
#define STOP if ( (stopm = clock()) == -1) {printf("Error calling clock");exit(1);}


int exchange(int *array, int i, int j) {
	int temp = array[i]; 
	array[i] = array[j]; 
	array[j] = temp;
}

#define		size	   16*1024

int partition(int *array, int start, int end) {
	int pivot;
	int i, j;
	int temp;
	pivot = array[start];
	i = start; 
	j = end+1;

	while(i < j) {
		++i; 
		while(array[i] <= pivot && i <= end) ++i;
		--j; 
		while(array[j] > pivot && j >= start) --j;
		if(i < j) {
			exchange(array, i, j);
		}
	}
	exchange(array, start, j);
	return j;
}


void quickSort(int *array, int start, int end){
	int index;
	if(start < end) {
	   index = partition(array, start, end);
	   quickSort(array, start, index-1);
	   quickSort(array, index+1, end);
	}
}

int main(void){
	int i, j, tmp;
	struct timespec start, stop; 
	double exe_time;
	srand(time(NULL)); 
	int * m = (int *) malloc (sizeof(int)*size);
	for(i=0; i<size; i++){
		m[i]=size-i;
	}
	
	//if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}
	////////**********Your code goes here***************//
	START;
	//randPartition(m, 0, size-1);
	quickSort(m, 0, size-1);
			
	///////******************************////
	STOP;
	//( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}		
	exe_time = ((double)stopm-startm)/CLOCKS_PER_SEC;
	
	for(i=0;i<16;i++) printf("%d ", m[i]);		
	printf("\nExecution time = %f sec\n",  exe_time);		
}	