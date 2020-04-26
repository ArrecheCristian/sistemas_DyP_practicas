#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>


double *A, *B, *C;              //matrices a utilzar
int N, T, filas_x_thread;


//Para calcular tiempo
double dwalltime(){
    double sec;
    struct timeval tv;

    gettimeofday(&tv,NULL);
    sec = tv.tv_sec + tv.tv_usec/1000000.0;
    return sec;
}


void* multiplicador (void* id_t){
    int id = *(int *)id_t;
    int i,j,k;
    int indice_inicio = id * filas_x_thread;
    int indice_final = (id+1) * filas_x_thread;

    for(i = indice_inicio; i < indice_final; i++){
        for(j = 0; j < N; j++){
            for(k = 0; k < N; k++){
                C[i*N +j] = C[i*N +j] + A[i*N + k]*B[k + j*N]; 
            }
        }
    }

    pthread_exit(NULL);
}
 

int main(int argc, char* argv[]){
    
    //Controla los argumentos al programa
    if ((argc != 3) || ((atoi(argv[1])) <= 0)){
        printf("\nUsar: %s t n\n  t: Cantidad de Threads \n  n: Dimension de la matriz \n", argv[0]);
        exit(1);
    }

    int check = 1;                  //para verificar resultado
    double timetick;                //para calcular tiempos de ejecucion

    T = atoi(argv[1]);              //cantidad de threads
    N = atoi(argv[2]);              //dimension de las matrices

    filas_x_thread = N/T;           // cantidad de filas que le corresponde multiplicar a cada thread
    int threads_ids[T];             //arreglo para almacenar los IDs de los threads
    pthread_t misThreads[T];        //arreglo de threads, cantidad pasada por parametro

    //Aloca memoria para las matrices
    A=(double*)malloc(sizeof(double)*N*N);
    B=(double*)malloc(sizeof(double)*N*N);
    C=(double*)malloc(sizeof(double)*N*N);
   
    //Inicializa las matrices A  B y C
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            A[i*N + j] = 1;                        
            B[i + j*N] = 1;
            C[i*N + j] = 0;  
        }
    }   


    //REALIZA LA MULTIPLICACION
    timetick = dwalltime();
 
    //Se crean los threads, donde se asigna la funcion a ejecutar con su ID como parametro
    for(int id = 0; id < T; id++){
            threads_ids[id] = id;  
            pthread_create(&misThreads[id], NULL, &multiplicador, (void*) &threads_ids[id]);
    }

    //Espera que todos los threads terminen su ejecucion
    for(int id=0;id<T;id++){
        pthread_join(misThreads[id],NULL);
    }

    printf("Tiempo en segundos %f\n", dwalltime() - timetick);


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

    return 0;
}
