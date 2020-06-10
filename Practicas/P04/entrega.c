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
    unsigned long N;      
    int cantidadDeProcesos, id;
    double *A, *B, *C, *D, *ab, *abc, *ac, *R;                  //matrices utilizadas
    int filas_por_proceso, datos_por_proceso, datos_totales;                          
    int i, j, k;
    double timetick;                    //medicion de tiempo
    double avgABC, avgAC, avgABC_global, avgAC_global, avgMUL;
    register double aux, aux2;                   //variable temporal con directiva register para las multiplicaciones
    double check = 1;                      //verifica resultado
    MPI_Status status;

    if ((argc != 2) || ((atoi(argv[1])) <= 0)){                         //verifico parametros
        printf("\nUsar: %s n\n  n: Dimension de la matriz \n", argv[0]);
        exit(1);
    }

	MPI_Init(&argc, &argv);                                  //se inicializa el entorno de ejecucion MPI
	MPI_Comm_rank(MPI_COMM_WORLD,&id);
	MPI_Comm_size(MPI_COMM_WORLD,&cantidadDeProcesos);
    
    N = atoi(argv [1]);                                     //dimension de la matriz
    filas_por_proceso = N/cantidadDeProcesos;               //cantidad de filas que tendra que procesar cada proceso
    datos_por_proceso = filas_por_proceso*N;                //cantidad de datos que tendra que procesar cada proceso
    datos_totales = N*N;                                    //cantidad de datos totales

    //Alocacion de memoria para las matrices. Tanto el root como los demas procesos reservan la misma cantidad de memoria para estas
    B = (double*) malloc(sizeof(double)*datos_totales);
    C = (double*) malloc(sizeof(double)*datos_totales);
    ab = (double*) malloc(sizeof(double)*datos_por_proceso);      //No se necesita la matriz entera, solo la parte que le 
    ac = (double*) malloc(sizeof(double)*datos_por_proceso);      //corresponde a cada proceso
    abc = (double*) malloc(sizeof(double)*datos_por_proceso);

    if (id == ROOT){                                        //bloque de código que solo ejecuta el proceso 0
        A = (double*) malloc(sizeof(double)*datos_totales);           //cambian las alocaciones con los demas procesos
        R = (double*) malloc(sizeof(double)*datos_totales);
        D = (double*) malloc(sizeof(double)*datos_totales);
        
        for (i = 0; i < N; i++){                                //el proceso 0 inicializa las matrices 
            for (j = 0; j < N; j++){
                A[i*N + j] = 1;
                B[i + j*N] = 1;
                C[i + j*N] = 1;     
                D[i*N + j] = 1; 
            }
        }

    }
    else{                                                            
        A = (double*) malloc(sizeof(double)*datos_por_proceso);           //parte de A que me toca procesar
        R = (double*) malloc(sizeof(double)*datos_por_proceso);
        D = (double*) malloc(sizeof(double)*datos_por_proceso);
    }
    
    //barrera en este punto por si los demas procesos (no ROOT) no terminaron de alocar sus matrices, para que estas no sean
    //contempladas en la medicion de tiempo
    MPI_Barrier(MPI_COMM_WORLD);
    
    
    //COMIENZA EL PROCESAMINETO MEDIANTE MPI
    if (id == ROOT){                    //proceso 0 es el encargado de medir el tiempo
        timetick = dwalltime();                                     
    }
    
    MPI_Scatter(A, datos_por_proceso, MPI_DOUBLE, A,                 //Se divide A y D en partes iguales para cada proceso (incluyendo root)
                datos_por_proceso, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);    //quedando almacenada nuevamente en A y D respectivamente
    MPI_Scatter(D, datos_por_proceso, MPI_DOUBLE, D,                 
                datos_por_proceso, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);  
    MPI_Bcast(B, N*N, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);               
    MPI_Bcast(C, N*N, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);               //B y C se mandan completa mediante broadcast para todos los procesos 

    //Promedios
    avgABC = 0;
    avgAC = 0;

    //Multiplicacion AB junto con AC (se obtuvieron mejores tiempos al acceder 2 veces al mismo valor de A y eliminar otro bloque de 3 for)
    for (i = 0; i < filas_por_proceso; i++){                            //cada proceso calcula su parte 
        for (j = 0; j < N; j++){                                
            aux = 0;
            aux2 = 0;
            for (k=0; k<N; k++){
                aux += A[i*N + k] * B[k + j*N]; 
                aux2 += A[i*N + k] * C[k + j*N];
            }
            ab[i*N + j] = aux;
            ac[i*N + j] = aux2;
            avgAC += aux2;                  //suma su parte de AC para el calculo del promedio
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
            avgABC += aux;                  //suma su parte de ABC para el calculo del promedio
        }
    }

    //se suman todos los valores de la matriz completa ABC y AC para luego calcular el promedio global
    MPI_Allreduce(&avgABC, &avgABC_global, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    MPI_Allreduce(&avgAC, &avgAC_global, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    avgABC_global = avgABC_global/datos_totales;
    avgAC_global = avgAC_global/datos_totales;

    avgMUL = avgAC_global*avgABC_global; 


    //SUMA Y CALCULO FINAL
    for (i = 0; i < filas_por_proceso; i++){            
        for (j = 0; j < N; j++){                                
            R[i*N + j] = abc[i*N + j] + ac[i*N + j] + (D[i*N + j] * avgMUL); 
        }
    }

    //Recibo la parte procesada por cada uno de los demas procesos que sera almacenado en orden segun ID en R
    MPI_Gather(R, datos_por_proceso, MPI_DOUBLE, R, datos_por_proceso, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);


    //Por ultimo, nuevamente solo el proceso 0 imprime el tiempo y realiza las verificaciones de resultado
    if (id == ROOT){
    
        printf("Tiempo en segundos: %f\n", dwalltime() - timetick);

        //solo para corroborar la correcta asignacion de R, si es una matriz chica (8, 16) la imprime
        if ( N < 100) {             
            printf("\n\n");
            for (int i = 0; i < N; i++) {
                printf("| ");
                for (int j = 0; j < N; j++) {
                    printf("%.f ", R[i*N + j]);
                }
                printf("|\n");
            }
        }

        double compare = (N*N)+N+(N*N*N);     //resultado calculado a mano para corroborar (teniendo en cuenta que todas las matrices son
                                                // inicializadas en 1)
        //Verifica el resultado
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                check = check && (R[i*N + j] == compare);
            }
        }   

        if(check){
            printf("Multiplicacion de matrices: Resultado correcto\n\n");
        }else{
            printf("Multiplicacion de matrices: Resultado erroneo\n\n");
        }
    }
    
    free(A);
    free(B);
    free(C);
    free(D);
    free(ab);
    free(abc);
    free(ac);
    free(R);
    
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
