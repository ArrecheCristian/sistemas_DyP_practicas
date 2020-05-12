#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void proceso(int,int);

int main(int argc, char* argv[]){
    int myRank, cantidadDeProcesos;
    int N, filas_por_proceso;
    double *A, *B, *C;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&myRank);
	MPI_Comm_size(MPI_COMM_WORLD,&cantidadDeProcesos);
    
    if ((argc != 2) || ((atoi(argv[1])) <= 0)){
        printf("\nUsar: %s n\n  n: Dimension de la matriz \n", argv[0]);
        exit(1);
    }
    
    N = argv [1];
    filas_por_proceso = cantidadDeProcesos/N;

    //bloque de código que solo ejecuta el proceso 0
    if (myRank == 0){
        //reserva memoria e inicializa las matrices
        A = (double*) malloc(sizeof(double)*N*N);
        B = (double*) malloc(sizeof(double)*N*N);
        C = (double*) malloc(sizeof(double)*N*N);

        for (int i=0; i<N; i++){
           for (int j=0; j<N; j++){
               A[i*N + j] = 1;
               B[i + j*N] = 1;               
               C[i*N + j] = 0;
           }
        }

        for (i=1; i<cantidadDeProcesos; i++){
            MPI_Send(&filas_por_proceso, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        
        for (i = 0; i < count; i++){
            for (j = 0; j < count; j++){
                C[i*N + j] += A[i*N + j] * B[i + j*N]; 
            }
        }
        
        for (i=1; i<cantidadDeProcesos; i++){
            MPI_Recv(&filas_por_proceso, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    


	MPI_Finalize();
	return(0);
}


static void proceso(int myRank,int cantidadDeProcesos){
 MPI_Status status;
 int mensaje;
 int i;
 int p;
 char hostname[255];

	gethostname(hostname,255);
	printf("Proceso %i --> LISTO EN HOST %s\n", myRank, hostname);

	// Envia un mensaje a cada uno de los otros procesos
	for(p=0; p<cantidadDeProcesos; p++){
		if(p != myRank){
			MPI_Send(&myRank, 1, MPI_INT, p, 0, MPI_COMM_WORLD);
			printf("Proceso %i --> ENVIO A %i \n", myRank, p);
		}
	}

	// Recibe el mensaje de cada uno de los otros procesos
	for(p=0; p<cantidadDeProcesos; p++){                    //el for era hasta cantidadDeProcesos-1, no se por qué, lo cambié yo jeje
		if(p != myRank){
			MPI_Recv(&mensaje, 1, MPI_INT, p, 0, MPI_COMM_WORLD, &status);
			printf("Proceso %i --> RECIBO DE %i \n", myRank, mensaje);
		}
	}

}
