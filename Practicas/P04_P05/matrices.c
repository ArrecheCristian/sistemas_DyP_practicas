#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static void procesoPadre(int, int, int);
static void procesoHijo(int, int, int);

int main(int argc, char* argv[]){
    int cantidadDeProcesos, id;
    int N;

    if ((argc != 2) || ((atoi(argv[1])) <= 0)){                 //verifico parametros
        printf("\nUsar: %s n\n  n: Dimension de la matriz \n", argv[0]);
        exit(1);
    }

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&id);
	MPI_Comm_size(MPI_COMM_WORLD,&cantidadDeProcesos);
    
     N = atoi(argv [1]);

    //bloque de código que solo ejecuta el proceso 0
    if (id == 0){
        procesoPadre(id, cantidadDeProcesos, N);
    }
    else{
        procesoHijo(id, cantidadDeProcesos, N);
    }
    
	MPI_Finalize();
	return(0);
}

static void procesoPadre(int id, int cantidadDeProcesos, int N){
	double *A, *B, *C, *temp;
    int filas_por_proceso;
    int i, j, k;
    MPI_Status status;
    int check = 1;

    filas_por_proceso = N/cantidadDeProcesos;               //cantidad de filas que tendra que procesar cada proceso

    A = (double*) malloc(sizeof(double)*N*N);               //reserva memoria para todas las matrices
    B = (double*) malloc(sizeof(double)*N*N);
    C = (double*) malloc(sizeof(double)*N*N);
    temp = (double*) malloc(sizeof(double)*N*filas_por_proceso);

    for (int i=0; i<N; i++){                                //inicializa las matrices 
        for (int j=0; j<N; j++){
            A[i*N + j] = 1;
            B[i + j*N] = 1;               
            C[i*N + j] = 0;
        }
    }
    
    for (i=1; i<cantidadDeProcesos; i++){                       //envia las matrices a todos los procesos
        A += i * filas_por_proceso;                                                 //muevo el puntero de A para cada proceso
        MPI_Send(A, N*filas_por_proceso, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);         //de A solo manda la parte que necesita
        MPI_Send(B, N*N, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);                         //B manda completa
    }

    for (i = 0; i < filas_por_proceso; i++){                    //El proceso 0 procesa su parte y lo almacena en su porcion de C
        for (j = 0; j < N; j++){
            for (k=0; k<N; k++){
                C[i*N + j] += A[i*N + k] * B[k + j*N]; 
            }
        }
    }

    for (int p=1; p<cantidadDeProcesos; p++){
        printf("Proceso %d --> Espero a %d \n", id, p);
        MPI_Recv(temp, N*filas_por_proceso, MPI_DOUBLE, p, 0, MPI_COMM_WORLD, &status);
        
        int offset = p*filas_por_proceso;
        
        for (i = 0; i < filas_por_proceso; i++){            //copio los valores de temp en la posicion correspondiente de C
            printf("| ");
            for (j = 0; j < N; j++){
                C[(offset + i)*N +j] = temp[i*N +j];        //cada porcion ejecutada por cada proceso es asignada a la matriz final C
                printf("%.f ", C[(offset + i)*N +j]);

            }
            printf("|\n");
            
        }
        
    }

    printf("\n\n");
    if ( N < 17) {
        for (int i = 0; i < N; i++) {
            printf("| ");
            for (int j = 0; j < N; j++) {
                printf("%.f ", C[i*N + j]);
            }
            printf("|\n");
        }
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

}



static void procesoHijo(int id, int cantidadDeProcesos, int N){
	double *A, *B, *temp;
    int filas_por_proceso;
    int i, j, k;
    MPI_Status status;
    int check = 1;

    filas_por_proceso = N/cantidadDeProcesos;

    A = (double*) malloc(sizeof(double)*N*filas_por_proceso);             //se reserva memoria para cada proceso
    B = (double*) malloc(sizeof(double)*N*N);

    MPI_Recv(A, N*filas_por_proceso, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);          //se reciben los datos del proceso 0
    MPI_Recv(B, N*N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);

    temp = (double*) malloc(sizeof(double)*N*filas_por_proceso);        //matriz para almacenar el resultado parcial
                                                                        // que le corresponde a la porcion del proceso
    for (i = 0; i < filas_por_proceso; i++){                            
        for (j = 0; j < N; j++){                                        //inicializa la matriz resultado
            temp[i*N + j] = 0;
        }
    }

    for (i=0; i<filas_por_proceso; i++){                //realiza su porcion de calculo
        for (j=0; j<N; j++){
            for (k=0; k<N; k++){
                temp[i*N + j] += A[i*N +k]*B[k + j*N];
            }
        }            
    }

    MPI_Send(temp, N*filas_por_proceso, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    
    printf("Proceso %d --> Calculo realizado\n", id);

}

/*
Liberar memoria
utilizar las variables de tipo register
agregar la medicion de tiempo
 
*/