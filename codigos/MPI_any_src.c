#include <stdio.h>
#include <mpi.h>
#include <string.h>

int main(int argc, char **argv){
	int rank,size,i;
	int tag=0;
	MPI_Status status;
	char msg[12];
	char recvmsg[12];
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if(rank == 0) {
		strcpy(msg,"Hello World!\n");
		for(i=1;i<size;i++) {
			//printf("rank[0] enviando para [%d]\n", i);
			MPI_Send(msg,12,MPI_CHAR,i,tag, MPI_COMM_WORLD);
		}
		for(i=1;i<size;i++) {
			MPI_Recv(recvmsg,12,MPI_CHAR,MPI_ANY_SOURCE,MPI_ANY_TAG, MPI_COMM_WORLD,&status);
			printf("rank[0] recebendo de [%d]\n", status.MPI_SOURCE);
		}
		
	} else {
		//printf("rank[%d] esperando de rank[0]\n", rank);
		MPI_Recv(recvmsg,12,MPI_CHAR,0,tag, MPI_COMM_WORLD, &status);
		//printf("rank[%d] recebeu: [%s]\n", rank, recvmsg);
		strcpy (msg, recvmsg);
		//printf("rank(%d) retorna msg p/ rank[0]\n", rank);
		MPI_Send(msg,12,MPI_CHAR,status.MPI_SOURCE, tag, MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}


