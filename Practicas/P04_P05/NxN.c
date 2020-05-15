#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char* argv[]){
    int myRank, cantidadDeProcesos;
    int N, filas_por_proceso;
    double *A, *B, *C, *temp;                          //matrices a utilizar
    int indice_inicio, indice_fin;
    int i, j, k;
    int check = 1;
    MPI_Status status;


	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&myRank);
	MPI_Comm_size(MPI_COMM_WORLD,&cantidadDeProcesos);
    
    if ((argc != 2) || ((atoi(argv[1])) <= 0)){                 //verifico parametros
        printf("\nUsar: %s n\n  n: Dimension de la matriz \n", argv[0]);
        exit(1);
    }
    
    N = atoi(argv [1]);

    //bloque de código que solo ejecuta el proceso 0
    if (myRank == 0){
        A = (double*) malloc(sizeof(double)*N*N);               //reserva memoria para todas las matrices
        B = (double*) malloc(sizeof(double)*N*N);
        C = (double*) malloc(sizeof(double)*N*N);

        for (int i=0; i<N; i++){                                //inicializa las matrices 
           for (int j=0; j<N; j++){
               A[i*N + j] = 1;
               B[i + j*N] = 1;               
               C[i*N + j] = 0;
           }
        }
        for (i=1; i<cantidadDeProcesos; i++){                       //envia la matriz a todos los procesos
            MPI_Send(A, N*N, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(B, N*N, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        printf("Proceso 0 --> Datos enviados\n");

        filas_por_proceso = N/cantidadDeProcesos;

        for (i = 0; i < filas_por_proceso; i++){
            for (j = 0; j < N; j++){
                for (k=0; k<N; k++){
                    C[i*N + j] += A[i*N + j] * B[i + j*N]; 
                }
            }
        }

        //for (i=1; i<cantidadDeProcesos; i++){
        printf("Proceso %d --> Espero a %d \n", myRank, i);
            MPI_Recv(temp, N*filas_por_proceso, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        //}
        printf("Proceso %d --> Me fui\n", myRank);

    }

    else{
        A = (double*) malloc(sizeof(double)*N*N);              
        B = (double*) malloc(sizeof(double)*N*N);

        MPI_Recv(A, N*N, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(B, N*N, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        filas_por_proceso = N/cantidadDeProcesos;               //cantidad de filas que tendra que procesar cada proceso
        indice_inicio = myRank*filas_por_proceso;
        indice_fin = (myRank + 1)*filas_por_proceso;

        temp = (double*) malloc(sizeof(double)*N*filas_por_proceso);



        for (i=indice_inicio; i<indice_fin; i++){
            for (j=0; j<N; j++){
                temp[i*N + j] = 0;
                for (k=0; k<N; k++){
                    temp[i + j*N] += A[i*N +j]*B[i + j*N];
                }
            }
            
        }
        
        MPI_Send(temp, N*filas_por_proceso, MPI_INT, 0, 0, MPI_COMM_WORLD);

        printf("Proceso %d --> Calculo realizado\n", myRank);

    }
    
    //Verifica el resultado
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            //check = check && (getValor(C,i,j,ORDENXFILAS) == N);
            check = check && (C[i*N + j] == N);
        }
    }   

    if(check){
        printf("Multiplicacion de matrices: Resultado correcto\n");
    }else{
        printf("Multiplicacion de matrices: Resultado erroneo\n");
    }

	MPI_Finalize();
	return(0);
}

