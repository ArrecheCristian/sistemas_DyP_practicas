#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

#define ROOT 0

double dwalltime();


int main(int argc, char* argv[]){
    
    int cantidadDeProcesos, id, N;
    int filas_por_proceso, i, j;
    double *A, *minimo;                          //matriz utilizada
    double timetick;                    //medicion de tiempo
    MPI_Status status;

    if ((argc != 2) || ((atoi(argv[1])) <= 0)){                 //verifico parametros
        printf("\nUsar: %s n\n  n: Dimension de la matriz \n", argv[0]);
        exit(1);
    }

	MPI_Init(&argc, &argv);                                  //se inicializa el entorno de ejecucion MPI
	MPI_Comm_rank(MPI_COMM_WORLD,&id);
	MPI_Comm_size(MPI_COMM_WORLD,&cantidadDeProcesos);
    
    N = atoi(argv [1]);                                     //dimension de la matriz
    filas_por_proceso = N/cantidadDeProcesos;               //cantidad de filas que tendra que procesar cada proceso

    if(id == ROOT){

        A = (double*) malloc(sizeof(double)*N*N);
        minimo = (double*) malloc(sizeof(double)*filas_por_proceso*N);

        for (i = 0; i < N; i++){
            for (j = 0; j < N; j++){
                A[i*N + j] = j;
            }
        }
        

    }
    else{
        A = (double*) malloc(sizeof(double)*filas_por_proceso*N);
    }
    
        
    MPI_Scatter(A, filas_por_proceso*N, MPI_DOUBLE, A, filas_por_proceso*N, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);

    MPI_Reduce(A, minimo, filas_por_proceso*N, MPI_DOUBLE, MPI_MIN, ROOT, MPI_COMM_WORLD);


    if(id == ROOT){
        printf("\n\n");
        for (int i = 0; i < filas_por_proceso; i++) {
            printf("| ");
            for (int j = 0; j < N; j++) {
                printf("%.f ", minimo[i*N + j]);
            }
            printf("|\n");
        }
    }   
    
    
    MPI_Finalize();
	return(0);
}



//Para calcular tiempo
double dwalltime(){
    double sec;
    struct timeval tv;

    gettimeofday(&tv,NULL);
    sec = tv.tv_sec + tv.tv_usec/1000000.0;
    return sec;
}
