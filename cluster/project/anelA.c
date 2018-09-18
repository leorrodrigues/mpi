#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
	int rank,size;
	int tag=0;
	int token;
	MPI_Status status;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	printf("Worker %d/%d started\n", rank, size);

	if (rank == 0) {
		// envia para o primeiro
		token = 0;
		MPI_Send(&token, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);
		printf("%d enviou token = %d\n", rank, token);
		// recebe do ultimo
		MPI_Recv(&token, 1, MPI_INT, size - 1, tag, MPI_COMM_WORLD, &status);
		printf("%d com token final = %d\n", rank, token);
	} else {
		// recebe o token
		MPI_Recv(&token, 1, MPI_INT, rank - 1, tag, MPI_COMM_WORLD, &status);
		printf("%d recebeu token = %d\n", rank, token);
		token += rank;
		// envia para o proximo no anel
		if (rank + 1 == size) {
			MPI_Send(&token, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
		} else {
			MPI_Send(&token, 1, MPI_INT, rank + 1, tag, MPI_COMM_WORLD);
		}	
	}


	MPI_Finalize();
	return 0;
}
