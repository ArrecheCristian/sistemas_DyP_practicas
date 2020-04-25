#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>

#define ORDENXFILAS 0
#define ORDENXCOLUMNAS 1


//Dimension por defecto de las matrices
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
    double *A, *B, *C;
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

    //Inicializa las matrices A y B en 1, el resultado de la multiplicaciòn sera una matriz con todos sus valores en N
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            //setValor(A,i,j,ORDENXFILAS,1);
            A[i*N + j] = 1;                         //Para evitar el overhead de funciones, accede directamente a la matriz
                                                    //eliminando el llamado a funciones
            //setValor(B,i,j,ORDENXCOLUMNAS,1);
            B[i + j*N] = 1;
        }
    }   

    //Realiza la multiplicacion
    timetick = dwalltime();

    for(i=0;i<N;i++){
        for(j=0;j<N;j++){

            C[i*N + j] = 0;             //inicializa la matriz resultado C
            for(k=0;k<N;k++){
                //setValor(C,i,j,ORDENXFILAS, getValor(C,i,j,ORDENXFILAS) + getValor(A,i,k,ORDENXFILAS)*getValor(B,k,j,ORDENXCOLUMNAS));
                C[i*N + j] = C[i*N + j] + A[i*N + k]*B[k + j*N]; 
            }
        }
    }   

    printf("Tiempo en segundos %f\n", dwalltime() - timetick);

    //Verifica el resultado
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
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
    return(0);
}
