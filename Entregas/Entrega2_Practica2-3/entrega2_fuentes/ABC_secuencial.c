#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>


//Dimension de las matrices
int N = 100;

//Para calcular tiempo
double dwalltime(){
    double sec;
    struct timeval tv;

    gettimeofday(&tv,NULL);
    sec = tv.tv_sec + tv.tv_usec/1000000.0;
    return sec;
}


int main(int argc,char*argv[]){
    double *A, *B, *C, *ab, *abc;
    register double aux;
    int i, j, k;
    int check = 1;
    double timetick;

    //Controla los argumentos al programa
    if ((argc != 2) || ((N = atoi(argv[1])) <= 0)){
        printf("\nUsar: %s n\n  n: Dimension de la matriz \n", argv[0]);
        exit(1);
    }

    //Aloca memoria para las matrices
    A = (double*) malloc(sizeof(double)*N*N);
    B = (double*) malloc(sizeof(double)*N*N);
    C = (double*) malloc(sizeof(double)*N*N);
    ab = (double*) malloc(sizeof(double)*N*N);
    abc = (double*) malloc(sizeof(double)*N*N);

    //Inicializa las matrices A, B y C en 1, el resultado de la multiplicaciòn sera una matriz con todos sus valores en N*N
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            
            A[i*N + j] = 1;                                                       
            B[i*N + j] = 1;
            C[i*N + j] = 1;
        }
    }   

    //Realiza la multiplicacion
    timetick = dwalltime();

    for(i=0;i<N;i++){                       
        for(j=0;j<N;j++){                   
            aux = 0;                                //se hace uso de la variable aux con el identificador "register" para una 
            for(k=0;k<N;k++){                       //mejor optimizacion
                aux += A[i*N + k]*B[k + j*N];
            }
            ab[i*N + j] = aux;
        }
    }   

     for(i=0;i<N;i++){
        for(j=0;j<N;j++){                   
            aux = 0;                                
            for(k=0;k<N;k++){
                aux += ab[i*N + k]*C[k + j*N];
            }
            abc[i*N + j] = aux;
        }
    }  

    printf("Tiempo en segundos %f\n", dwalltime() - timetick);


    //Verifica el resultado final de la matriz ABC
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            check = check && (abc[i*N + j] == N*N);
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
    free(ab);
    free(abc);

    return(0);
}
