// OPERACION A RESOLVER   --->   R = ABC + AC + (avg(ABC) * avg(AC) * D)

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
    double *A, *B, *C, *D, *ab, *abc, *ac, *R;                  //matrices utilizadas
    int filas_por_proceso;                          
    int i, j, k;
    double timetick;                    //medicion de tiempo
    double avgABC, avgAC, avgABC_global, avgAC_global, avgMul;
    register double aux;                //variable temporal con directiva register
    int check = 1;                      //verifica resultado
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

    if (id == ROOT){                                           //bloque de código que solo ejecuta el proceso 0
        A = (double*) malloc(sizeof(double)*N*N);           //cambian las alocaciones con los demas procesos
        B = (double*) malloc(sizeof(double)*N*N);
        C = (double*) malloc(sizeof(double)*N*N);
        D = (double*) malloc(sizeof(double)*N*N);

        ab = (double*) malloc(sizeof(double)*N*filas_por_proceso);
        ac = (double*) malloc(sizeof(double)*N*N);
        abc = (double*) malloc(sizeof(double)*N*N);
        R = (double*) malloc(sizeof(double)*N*N);
   



        for (i = 0; i < N; i++){                                //el proceso 0 inicializa las matrices 
            for (j = 0; j < N; j++){
                A[i*N + j] = 1;
                B[i + j*N] = 1;
                C[i + j*N] = 1;     
                D[i + j*N] = 1; 
            }
        }
        //COMIENZA LA MULTIPLICACION MEDIANTE MPI
        timetick = dwalltime();                                     //proceso 0 es el encargado de medir el tiempo
    }
    else{                                                       //distintas alocaciones para los demas procesos
        A = (double*) malloc(sizeof(double)*N*filas_por_proceso);           //parte de A que me toca procesar
        B = (double*) malloc(sizeof(double)*N*N);                           //B la recibo completa
        C = (double*) malloc(sizeof(double)*N*N);           
        D = (double*) malloc(sizeof(double)*N*N);
        ab = (double*) malloc(sizeof(double)*N*filas_por_proceso);
        ac = (double*) malloc(sizeof(double)*N*filas_por_proceso);         
        abc = (double*) malloc(sizeof(double)*N*filas_por_proceso);
        R = (double*) malloc(sizeof(double)*N*filas_por_proceso);
        

        
    }

    //bloque que ejecutan todos los procesos
    MPI_Scatter(A, filas_por_proceso*N, MPI_DOUBLE, A,                  //Se divide A en partes iguales para cada proceso (incluyendo root)
                filas_por_proceso*N, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);    //quedando almacenada nuevamente en A
    MPI_Bcast(B, N*N, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);                //B se manda completa mediante broadcast para todos los procesos
    MPI_Bcast(C, N*N, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);                //C se manda completa
    MPI_Bcast(D, N*N, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);  


    //Multiplicacion AB
    for (i = 0; i < filas_por_proceso; i++){                            //cada proceso calcula su parte 
        for (j = 0; j < N; j++){                                
            aux = 0;
            for (k=0; k<N; k++){
                aux += A[i*N + k] * B[k + j*N]; 
            }
            ab[i*N + j] = aux;
        }
    }

    //multiplicacion ABC
    for (i = 0; i < filas_por_proceso; i++){            
        for (j = 0; j < N; j++){                                
            aux = 0;
            for (k=0; k<N; k++){
                aux += ab[i*N + k] * C[k + j*N]; 
            }
            abc[i*N + j] = aux;
        }
    }

    //multiplicacion AC
    for (i = 0; i < filas_por_proceso; i++){            
        for (j = 0; j < N; j++){                                
            aux = 0;
            for (k=0; k<N; k++){
                aux += A[i*N + k] * C[k + j*N]; 
            }
            ac[i*N + j] = aux;
        }
    }

    
    // AVG
    //promedio local a cada proceso en su parte de la matriz
    avgABC = 0;
    avgAC = 0;

    for (i = 0; i < filas_por_proceso; i++){            //cada proceso calcula su promedio local
        for (j = 0; j < N; j++){
           avgABC += abc[i*N + j]; 
           avgAC += ac[i*N + j];
        }
    }

    avgABC = avgABC/(filas_por_proceso*N);
    avgAC = avgAC/(filas_por_proceso*N);
    

    MPI_Allreduce(&avgABC, &avgABC_global, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    MPI_Allreduce(&avgAC, &avgAC_global, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);


    avgABC_global = avgABC_global/cantidadDeProcesos;
    avgAC_global = avgAC_global/cantidadDeProcesos;

    avgMul = avgAC_global*avgABC_global; 

    //suma ABC + AC + avgABC*avgAC*D
    for (i = 0; i < filas_por_proceso; i++){            
        for (j = 0; j < N; j++){                                
            R[i*N + j] = abc[i*N + j] + ac[i*N + j]+avgMul*D[i*N+j] ; 
        }
    }

    //Recibo el tramo procesado por cada uno de los demas procesos
     //que sera almacenado en orden segun ID en C
    MPI_Gather(R, filas_por_proceso*N, MPI_DOUBLE, R, filas_por_proceso*N, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);


    //Por ultimo, nuevamente solo el proceso 0 imprime el tiempo y realiza las verificaciones de resultado
    if (id == ROOT){
    
        printf("Tiempo en segundos: %f\n", dwalltime() - timetick);

        //solo para corroborar la correcta asignacion de C, si es una matriz chica (8, 16) la imprime
        if ( N < 17) {             
            printf("\n\n");
            for (int i = 0; i < N; i++) {
                printf("| ");
                for (int j = 0; j < N; j++) {
                    printf("%.f ", R[i*N + j]);
                }
                printf("|\n");
            }
        }
    
        //Verifica el resultado
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                check = check && (R[i*N + j] == (N*N)+N+avgMul);
            }
        }   

        if(check){
            printf("Multiplicacion de matrices: Resultado correcto\n");
        }else{
            printf("Multiplicacion de matrices: Resultado erroneo\n");
        }
    }
    
    free(A);
    free(B);
    free(C);
    free(D);
    free(ab);
    free(abc);
    
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
