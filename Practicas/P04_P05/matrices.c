#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>


static void procesoPadre(int, int, int);
static void procesoHijo(int, int, int);
double dwalltime();


int main(int argc, char* argv[]){
    int cantidadDeProcesos, id;
    int N;
    double timetick;

    if ((argc != 2) || ((atoi(argv[1])) <= 0)){                 //verifico parametros
        printf("\nUsar: %s n\n  n: Dimension de la matriz \n", argv[0]);
        exit(1);
    }

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&id);
	MPI_Comm_size(MPI_COMM_WORLD,&cantidadDeProcesos);
    
     N = atoi(argv [1]);            //dimension de la matriz

    if (id == 0){                                           //bloque de código que solo ejecuta el proceso 0
        timetick = dwalltime();                             //el proceso 0 es el encargado de medir el tiempo
        procesoPadre(id, cantidadDeProcesos, N);
        printf("Tiempo en segundos: %f\n", dwalltime() - timetick);
    }
    else{                                                 //bloque de codigo que ejecutan los demas procesos
        procesoHijo(id, cantidadDeProcesos, N);
    }
    
	MPI_Finalize();
	return(0);
}



static void procesoPadre(int id, int cantidadDeProcesos, int N){
	double *A, *B, *C, *temp;
    int filas_por_proceso;                          //cantidad de filas que debera calcular cada proceso
    int i, j, k;
    register double aux;
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
        }
    }
    
    for (i=1; i<cantidadDeProcesos; i++){                       //envia las matrices a todos los procesos (se excluye el proceso 0)
        MPI_Send(&A[i*filas_por_proceso], N*filas_por_proceso, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);         //de A solo manda la parte que necesita
        MPI_Send(B, N*N, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);                         //B se manda completa
    }

    for (i = 0; i < filas_por_proceso; i++){                    //El proceso 0 procesa su parte y lo almacena en su porcion de C
        for (j = 0; j < N; j++){
            aux = 0;
            for (k=0; k<N; k++){
                aux += A[i*N + k] * B[k + j*N]; 
            }
            C[i*N + j] = aux;
        }
    }

    for (int p=1; p<cantidadDeProcesos; p++){               //recibo las porciones de matrices de todos los procesos 
                                                            //en la porcion que corresponde de C
        MPI_Recv(&C[p*filas_por_proceso*N], N*filas_por_proceso, MPI_DOUBLE, p, 0, MPI_COMM_WORLD, &status);
    }

    //solo para corroborar la correcta asignacion de C, si es una matriz chica (8, 16) la imprime
    if ( N < 17) {             
        printf("\n\n");
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

    free(A);
    free(B);
    free(C);
    free(temp);

}



static void procesoHijo(int id, int cantidadDeProcesos, int N){
	double *A, *B, *temp;
    int filas_por_proceso;
    int i, j, k;
    register double aux;
    MPI_Status status;
    int check = 1;

    filas_por_proceso = N/cantidadDeProcesos;                         //cantidad de filas que debera procesar

    A = (double*) malloc(sizeof(double)*N*filas_por_proceso);             //se reserva memoria para cada proceso
    B = (double*) malloc(sizeof(double)*N*N);

    MPI_Recv(A, N*filas_por_proceso, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);          //se reciben los datos del proceso 0
    MPI_Recv(B, N*N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);

    temp = (double*) malloc(sizeof(double)*N*filas_por_proceso);        //matriz para almacenar el resultado de su porcion

    for (i=0; i<filas_por_proceso; i++){                                //realiza su porcion de calculo
        for (j=0; j<N; j++){
            aux = 0;
            for (k=0; k<N; k++){
                aux += A[i*N +k]*B[k + j*N];
            }
            temp[i*N + j] = aux;
        }            
    }

    MPI_Send(temp, N*filas_por_proceso, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);      //envia su porcion al proceso 0
    
    free(A);
    free(B);
    free(temp);
}



//Para calcular tiempo
double dwalltime(){
    double sec;
    struct timeval tv;

    gettimeofday(&tv,NULL);
    sec = tv.tv_sec + tv.tv_usec/1000000.0;
    return sec;
}

