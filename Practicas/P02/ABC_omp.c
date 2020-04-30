#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>



int main (int argc, char* argv[]){

    double *A, *B, *C, *ab, *abc;              //matrices a utilzar
    int N, T, i, j, k;

    //Controla los argumentos al programa
    if ((argc != 3) || ((atoi(argv[1])) <= 0)){
        printf("\nUsar: %s t n\n  t: Cantidad de Threads \n  n: Dimension de la matriz \n", argv[0]);
        exit(1);
    }

    N = atoi(argv[2]);              //dimension de las matrices
    T = atoi(argv[1]);              //cantidad de threads
    omp_set_num_threads(T);         //Crea los T threads 

    //Aloca memoria para las matrices
    A = (double*) malloc(sizeof(double)*N*N);
    B = (double*) malloc(sizeof(double)*N*N);
    C = (double*) malloc(sizeof(double)*N*N);
    ab = (double*) malloc(sizeof(double)*N*N);
    abc = (double*) malloc(sizeof(double)*N*N);

    //Inicializa las matrices A  B y C
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            A[i*N + j] = 1;                        
            B[i + j*N] = 1;
            C[i + j*N] = 1;
            ab[i*N + j] = 0;
            abc[i*N + j] = 0;  
        }
    }   


    //REALIZA LA MULTIPLICACION

    #pragma omp parallel private (i,j,k)
    {
        
    }





}



