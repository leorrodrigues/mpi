#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int** alloc_2d_int(int linhas, int colunas) {
	int *data = (int *)malloc(linhas*colunas*sizeof(int));
	int **array= (int **)malloc(linhas*sizeof(int*));
	for (int i=0; i<linhas; i++)
		array[i] = &(data[colunas*i]);
	return array;
}

void slave(int rank,int linhas, int colunas){
	MPI_Status status;
	int size, tag=0;
	int **matriz=alloc_2d_int(linhas,colunas);
	// recebe o token
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Recv(&(matriz[0][0]), linhas*colunas, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
	int partial=0;
	for(int i=rank-1; i<linhas; i+=size-1) {
		partial=0;
		for(int j=0; j<colunas; j++) {
			partial+=matriz[i][j];
		}
		//printf("Slave %d calculou o produtorio da linha %d = %d\n",rank+1,i,partial);
		MPI_Send(&partial, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
	}
	//com o produtorio calculado, retorna o resultado para o master
	free(matriz[0]);
	free(matriz);
}

int main(int argc, char *argv[]){
	int rank,size;
	if(argc!=3) {
		printf("Erro args\n");
		exit(0);
	}
	int linhas=atoi(argv[1]);
	int colunas=atoi(argv[2]);
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	printf("Worker %d/%d started\n", rank+1, size);
	if(size<2) {
		printf("Deve possuir no minimo 2 processos\n");
	}
	slave (rank,linhas,colunas);
	MPI_Finalize();
	return 0;
}
