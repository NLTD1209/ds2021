
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>

#define MAX_DATA 100
int main( int argc, char **argv ) {
	MPI_Comm server;
	MPI_Status status;
	char port_name[MPI_MAX_PORT_NAME],str[50],ch;
	int i, tag, again;
	int option;

	if (argc < 2) {
		fprintf(stderr, "[Client]server port name required.\n");
		exit(EXIT_FAILURE);
	}

	MPI_Init(&argc, &argv);
	strcpy(port_name, argv[1]);
	printf("[Client]Trying connect to %s\n", argv[1]);
	MPI_Comm_connect(port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &server);
	printf("[Client]Server connected by client\n");

	printf("Choose your options of key-value stored(1-Create 2-update 3-delete 4-get):");
	fflush(stdout);
	scanf("%d\n",&option);
	tag = 2; // action to perform
	//MPI_Send(&option, 1, MPI_INT, 0, tag, server);
	printf("Your option: %d\n",option);

	//while(!1) {
		//send option to server
		switch (option) {
			case 1: printf("Create a key-value:\n");
			MPI_Send(&option, 1, MPI_INT, 0, tag, server); break;
			case 2: printf("Update\n");
			MPI_Send(&option, 1,  MPI_INT, 0, tag, server); break;
			case 3: printf("Delete\n");
			MPI_Send(&option,1,  MPI_INT, 0, tag, server); break;
		case 4: printf("Get\n");
			MPI_Send(&option, 1, MPI_INT, 0, tag, server); break;
		}
	//}


	MPI_Comm_disconnect(&server);
	MPI_Finalize();
	return 0;
}
