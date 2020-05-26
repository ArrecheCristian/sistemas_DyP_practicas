#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <sys/time.h>
#include <time.h>

#define ROOT 0

double dwalltime();


int main(int argc, char* argv[]){
    
    int cantidadDeProcesos, id, N;
    int filas_por_proceso, i, j;
    double *A, *B;                          //matriz utilizada
    double min, max, promedio;
    double min_global, max_global, promedio_global;
    register double aux;
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

        A = (double*) malloc(sizeof(double)*N*N);           //aloca memoria para la matriz A y B
        B = (double*) malloc(sizeof(double)*N*N);
        
        srand (time(NULL));                                 //inicializacion de A con valores random
        
        for (i = 0; i < N; i++){
            for (j = 0; j < N; j++){
                A[i*N + j] = rand() % 99;
            }
        }

        if (N < 17){                        //si es una matriz chica (de prueba) se imprime para corroborar resultados
            printf("\n--> MATRIZ A\n");             
            for (int i = 0; i < N; i++) {
                printf("| ");
                for (int j = 0; j < N; j++) {
                    printf("%.f ", A[i*N + j]);
                }
                printf("|\n");
            }
        }

        timetick = dwalltime();             //el proceso ROOT es el encargado de medir el tiempo
    
    }
    else{                                   //los demas procesos alocan memoria para su parte de la matriz
        A = (double*) malloc(sizeof(double)*filas_por_proceso*N);
        B = (double*) malloc(sizeof(double)*filas_por_proceso*N);
    }
    
    
    //COMIENZAN LAS OPERACIONES MEDIANTE MPI

    //se envia a cada proceso una parte de A
    MPI_Scatter(A, filas_por_proceso*N, MPI_DOUBLE, A, filas_por_proceso*N, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
   
    min = DBL_MAX;                  //minimo, maximo y promedio local a cada proceso en su parte de la matriz
    max = DBL_MIN;
    promedio = 0;

    for (i = 0; i < filas_por_proceso; i++){            //cada proceso calcula su minimo, maximo y promedio local
        for (j = 0; j < N; j++){
            aux = A[i*N + j];                      //aux variable con identificador REGISTER para optimizar accesos y reducir calculos
            if( aux > max){
                max = aux;
            }
            if( aux < min){
                min = aux;
            }
            promedio += aux;
        }
    }

    promedio = promedio/(filas_por_proceso*N);

    //cada proceso obtiene el minimo, maximo y promedio global
    MPI_Allreduce(&min, &min_global, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
    MPI_Allreduce(&max, &max_global, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
    MPI_Allreduce(&promedio, &promedio_global, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    promedio_global = promedio_global/cantidadDeProcesos;


    for (i = 0; i < filas_por_proceso; i++){            //cada proceso se encarga de armar su parte de la matriz B
        for (j = 0; j < N; j++){
            if (A[i*N + j] < promedio_global){
                B[i*N + j] = min_global;
            }
            else{
                if (A[i*N + j] > promedio_global){
                    B[i*N + j] = max_global;
                }
                else{
                    B[i*N + j] = promedio_global;
                }
            }
        }
    }

    //la matriz B queda completa en el proceso ROOT
    MPI_Gather(B, filas_por_proceso*N, MPI_DOUBLE, B, filas_por_proceso*N, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);


    if(id == ROOT){
        printf("\nTiempo en segundos: %f\n", dwalltime() - timetick);

        printf("\nMINIMO: %.f \nMAXIMO: %.f \nPROMEDIO: %.2f\n\n", min_global, max_global, promedio_global);
        
        if (N < 17){                        //si es una matriz chica (de prueba) se imprime para corroborar resultados
            printf("--> MATRIZ B\n");
            for (int i = 0; i < N; i++) {
                printf("| ");
                for (int j = 0; j < N; j++) {
                    printf("%.f ", B[i*N + j]);
                }
                printf("|\n");
            }
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
