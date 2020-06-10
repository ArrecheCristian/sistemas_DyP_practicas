// OPERACION A RESOLVER   --->   R = ABC + AC + (avg(ABC) * avg(AC) * D)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>


double dwalltime();


int main(int argc, char* argv[]){
    unsigned long N;      
    double *A, *B, *C, *D, *ab, *abc, *ac, *R;      //matrices utilizadas
    int i, j, k;
    double timetick;                                //medicion de tiempo
    double avgABC, avgAC, avgMUL;
    register double aux, aux2;                      //variable temporal con directiva register para las multiplicaciones
    int datos_totales;
    double check = 1;                               //verifica resultado

    if ((argc != 2) || ((atoi(argv[1])) <= 0)){     //verifico parametros
        printf("\nUsar: %s n\n  n: Dimension de la matriz \n", argv[0]);
        exit(1);
    }

    N = atoi(argv [1]);                                     //dimension de la matriz
    datos_totales = N*N;

    //Alocacion de memoria para las matrices. 
    B = (double*) malloc(sizeof(double)*datos_totales);
    C = (double*) malloc(sizeof(double)*datos_totales);
    ab = (double*) malloc(sizeof(double)*datos_totales);      
    ac = (double*) malloc(sizeof(double)*datos_totales);      
    abc = (double*) malloc(sizeof(double)*datos_totales);
    A = (double*) malloc(sizeof(double)*datos_totales);          
    R = (double*) malloc(sizeof(double)*datos_totales);
    D = (double*) malloc(sizeof(double)*datos_totales);

    for (i = 0; i < N; i++){                               //inicializacion de matrices
        for (j = 0; j < N; j++){
            A[i*N + j] = 1;
            B[i + j*N] = 1;
            C[i + j*N] = 1;     
            D[i*N + j] = 1; 
        }
    }

    //COMIENZO DEL PROCESAMIENTO
    timetick = dwalltime();                                     
    
    //Promedios
    avgABC = 0;
    avgAC = 0;

    //Multiplicacion AB junto con AC (se obtuvieron mejores tiempos al acceder 2 veces al mismo valor de A y eliminar otro bloque de 3 for)
    for (i = 0; i < N; i++){                          
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
    for (i = 0; i < N; i++){            
        for (j = 0; j < N; j++){                                
            aux = 0;
            for (k=0; k<N; k++){
                aux += ab[i*N + k] * C[k + j*N]; 
            }
            abc[i*N + j] = aux;
            avgABC += aux;                  //suma su parte de ABC para el calculo del promedio
        }
    }

    avgABC = avgABC/datos_totales;
    avgAC = avgAC/datos_totales;

    avgMUL = avgAC*avgABC; 

    //SUMA Y CALCULO FINAL
    for (i = 0; i < N; i++){            
        for (j = 0; j < N; j++){                                
            R[i*N + j] = abc[i*N + j] + ac[i*N + j] + (D[i*N + j] * avgMUL); 
        }
    }

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

    
    free(A);
    free(B);
    free(C);
    free(D);
    free(ab);
    free(abc);
    free(ac);
    free(R);
    
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
