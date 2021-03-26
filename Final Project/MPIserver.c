
#include <stdio.h>
#include <unistd.h>
#include <mpi.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATA    100
#define KVS_SPACE_INCREMENT 8

typedef int KVScompare(const char *a, const char *b);

typedef const void KVSkey;

typedef void KVSvalue;

typedef struct {
    KVSkey *key;
    KVSvalue *value;
} KVSpair;

typedef struct KVSstore KVSstore;

KVSstore *kvs_create(KVScompare *compare);
void kvs_destroy(KVSstore *store);
void kvs_put(KVSstore *store, KVSkey *key, KVSvalue *value);
KVSvalue *kvs_get(KVSstore *store, KVSkey *key);
void kvs_remove(KVSstore *store, KVSkey *key);
size_t kvs_length(KVSstore *store);
KVSpair *kvs_pair(KVSstore *store, size_t index);


int main(int argc, char **argv) {
	MPI_Comm client;
	MPI_Status status;
	char port_name[MPI_MAX_PORT_NAME],str[50];
	int size, again, i,j, option;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (size != 1) {
		fprintf(stderr, "[Server]Server too big");
		exit(EXIT_FAILURE);
	}
	MPI_Open_port(MPI_INFO_NULL, port_name);
	printf("[Server]Server available at port: %s\n", port_name);


	i=0;
	while (1) {
		MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &client);
		printf("[Server]client connected the server\n");

		again = 1;
		while (again) {
      MPI_Recv(&option, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, client, &status);
			printf("[Server]server received sth!\n");
			switch (status.MPI_TAG) {
			case 0:
				MPI_Comm_free(&client);
				MPI_Close_port(port_name);
				MPI_Finalize();
				return 0;
			case 1:
        printf("case 1:\n");
				MPI_Comm_disconnect(&client);
				again = 0;
				i=0;
				break;
			case 2:
        printf("option: %d\n",option);
				break;
			default:
				/* Unexpected message type */
				MPI_Abort(MPI_COMM_WORLD, 1);
			}
		}
	}

}
struct KVSstore {
    KVSpair *pairs;
    KVScompare *compare;
    size_t length;
    size_t space;
};

static const size_t kvs_pair_size = sizeof(KVSpair);

static const size_t kvs_store_size = sizeof(KVSstore);
