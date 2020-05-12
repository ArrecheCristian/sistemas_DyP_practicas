#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>


//Para calcular tiempo
double dwalltime(){
    double sec;
    struct timeval tv;

    gettimeofday(&tv,NULL);
    sec = tv.tv_sec + tv.tv_usec/1000000.0;
    return sec;
}


int main (int argc, char* argv[]){

    double *A, *B, *C, *ab, *abc;               //matrices a utilzar
    int N, T, i, j, k;                          //argumentos e indices
    register int aux;
    double timetick;                            //para calcular tiempos de ejecucion
    int check = 1;                              //para verificar resultado


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

    //Inicializa todas las matrices
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
    timetick = dwalltime();

    //Usando 1 solo parallel
    #pragma omp parallel private(i,j,k, aux)
    { 
        #pragma omp for
            for ( i = 0; i < N; i++){
                for ( j = 0; j < N; j++){
                    aux = 0;
                    for ( k = 0; k < N; k++){
                        aux += A[i*N +k] * B[k + j*N];
                    }
                    ab[i*N +j] = aux;
                }
            }       //barrera implicita (JOIN) para cada hilo

        #pragma omp for
            for ( i = 0; i < N; i++){
                for ( j = 0; j < N; j++){
                    aux = 0;
                    for ( k = 0; k < N; k++){
                        aux += ab[i*N +k] * C[k + j*N];
                    }
                    abc[i*N +j] = aux;
                }
            }       //barrera implicita (JOIN) para cada hilo
    }

    printf("Tiempo en segundos %f\n", dwalltime() - timetick);
    
    
    //Verifica el resultado
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            //check = check && (getValor(C,i,j,ORDENXFILAS) == N);
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

    return 0;

}