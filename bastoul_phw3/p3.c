#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv){
    int size;
    int rank;
    int AGGREGATE;
    int recvBuf;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);




	if(rank == 0){
        //printf("rank 0\n");
        AGGREGATE = 0.1 + 0.09*rank;
        MPI_Send(&AGGREGATE, 1, MPI_INT, 5, 0, MPI_COMM_WORLD);
        ///
        MPI_Recv(&recvBuf, 1, MPI_INT, 5, 100, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        AGGREGATE += recvBuf;
        ///
        printf("Process %d: AGGR %d \n",  rank, AGGREGATE);
	} else if (rank == 1) {
        //printf("rank 1\n");
        AGGREGATE = 0.1 + 0.09*rank;
        MPI_Send(&AGGREGATE, 1, MPI_INT, 5, 1, MPI_COMM_WORLD);
        MPI_Send(&AGGREGATE, 1, MPI_INT, 7, 6, MPI_COMM_WORLD);
        ///
        MPI_Recv(&recvBuf, 1, MPI_INT, 5, 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        AGGREGATE += recvBuf;
        MPI_Recv(&recvBuf, 1, MPI_INT, 7, 106, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        AGGREGATE += recvBuf;
        ///
        printf("Process %d: AGGR %d \n",  rank, AGGREGATE);
    } else if (rank == 2) {
        //printf("rank 2\n");
        AGGREGATE = 0.1 + 0.09*rank;
        MPI_Send(&AGGREGATE, 1, MPI_INT, 5, 2, MPI_COMM_WORLD);
        MPI_Send(&AGGREGATE, 1, MPI_INT, 6, 5, MPI_COMM_WORLD);
        MPI_Send(&AGGREGATE, 1, MPI_INT, 7, 7, MPI_COMM_WORLD);
        ///
        MPI_Recv(&recvBuf, 1, MPI_INT, 5, 102, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        AGGREGATE += recvBuf;
        MPI_Recv(&recvBuf, 1, MPI_INT, 6, 103, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        AGGREGATE += recvBuf;
        MPI_Recv(&recvBuf, 1, MPI_INT, 7, 107, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        AGGREGATE += recvBuf;
        ///
        printf("Process %d: AGGR %d \n",  rank, AGGREGATE);
    } else if (rank == 3) {
        //printf("rank 3\n");
        AGGREGATE = 0.1 + 0.09*rank;
        MPI_Send(&AGGREGATE, 1, MPI_INT, 6, 3, MPI_COMM_WORLD);
        MPI_Send(&AGGREGATE, 1, MPI_INT, 7, 8, MPI_COMM_WORLD);
        ///
        MPI_Recv(&recvBuf, 1, MPI_INT, 6, 104, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        AGGREGATE += recvBuf;
        MPI_Recv(&recvBuf, 1, MPI_INT, 7, 108, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        AGGREGATE += recvBuf;
        ///
        printf("Process %d: AGGR %d \n",  rank, AGGREGATE);
    } else if (rank == 4) {
        //printf("rank 4\n");
        AGGREGATE = 0.1 + 0.09*rank;
        MPI_Send(&AGGREGATE, 1, MPI_INT, 6, 4, MPI_COMM_WORLD);
        ///
        MPI_Recv(&recvBuf, 1, MPI_INT, 6, 105, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        AGGREGATE += recvBuf;
        ///
        printf("Process %d: AGGR %d \n",  rank, AGGREGATE);
    } else if (rank == 5) {
        AGGREGATE = 1;
        MPI_Recv(&recvBuf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        AGGREGATE *= recvBuf;
        MPI_Recv(&recvBuf, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        AGGREGATE *= recvBuf;
        MPI_Recv(&recvBuf, 1, MPI_INT, 2, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        AGGREGATE *= recvBuf;
        ///
        MPI_Send(&AGGREGATE, 1, MPI_INT, 0, 100, MPI_COMM_WORLD);
        MPI_Send(&AGGREGATE, 1, MPI_INT, 1, 101, MPI_COMM_WORLD);
        MPI_Send(&AGGREGATE, 1, MPI_INT, 2, 102, MPI_COMM_WORLD);
    } else if (rank == 6) {
        AGGREGATE = 1;
        MPI_Recv(&recvBuf, 1, MPI_INT, 2, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        AGGREGATE *= recvBuf;
        MPI_Recv(&recvBuf, 1, MPI_INT, 3, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        AGGREGATE *= recvBuf;
        MPI_Recv(&recvBuf, 1, MPI_INT, 4, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        AGGREGATE *= recvBuf;
        ///
        MPI_Send(&AGGREGATE, 1, MPI_INT, 2, 103, MPI_COMM_WORLD);
        MPI_Send(&AGGREGATE, 1, MPI_INT, 3, 104, MPI_COMM_WORLD);
        MPI_Send(&AGGREGATE, 1, MPI_INT, 4, 105, MPI_COMM_WORLD);
    } else if (rank == 7) {
        AGGREGATE = 1;
        MPI_Recv(&recvBuf, 1, MPI_INT, 1, 6, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        AGGREGATE *= recvBuf;
        MPI_Recv(&recvBuf, 1, MPI_INT, 2, 7, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        AGGREGATE *= recvBuf;
        MPI_Recv(&recvBuf, 1, MPI_INT, 3, 8, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        AGGREGATE *= recvBuf;
        ///
        MPI_Send(&AGGREGATE, 1, MPI_INT, 1, 106, MPI_COMM_WORLD);
        MPI_Send(&AGGREGATE, 1, MPI_INT, 2, 107, MPI_COMM_WORLD);
        MPI_Send(&AGGREGATE, 1, MPI_INT, 3, 108, MPI_COMM_WORLD);
    }
    
    

    //printf("\n\nProgram done\n\n");

    MPI_Finalize();
    return 0;
}