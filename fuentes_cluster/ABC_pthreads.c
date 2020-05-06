#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>


double *A, *B, *C, *ab, *abc;              //matrices a utilzar
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
    int id = *(int *)id_t;                                  //ID del thread actual
    int i,j,k;
    register int aux;
    int indice_inicio = id * filas_x_thread;                //dependiendo del ID y de la cantidad de filas que le corresponde a
    int indice_final = (id+1) * filas_x_thread;             //cada thread, seteo los indices de recorridos para las filas

    for(i = indice_inicio; i < indice_final; i++){          //solo recorren y calculan la parte que le corresponde a ese thread
        for(j = 0; j < N; j++){
            aux = 0;
            for(k = 0; k < N; k++){
                aux += A[i*N + k]*B[k + j*N];    //primera parte de la multiplicacion A*B
            }
            ab[i*N +j] = aux;
        }
    }
    
    for(i = indice_inicio; i < indice_final; i++){
        for(j = 0; j < N; j++){   
            aux = 0;                                      //como ab se esta recorriendo por filas, no hay problema de inconsistencias
            for(k = 0; k < N; k++){                         //junto con el bucle anterior, los datos que se estàn leyendo ya fueron     
                aux += ab[i*N + k]*C[k + j*N];     //actualizados por el mismo thread.
            }
            abc[i*N +j] = aux;
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
    A = (double*)malloc(sizeof(double)*N*N);
    B = (double*)malloc(sizeof(double)*N*N);
    C = (double*)malloc(sizeof(double)*N*N);
    ab = (double*)malloc(sizeof(double)*N*N);
    abc = (double*)malloc(sizeof(double)*N*N);

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
 
    //Se crean los threads, donde se asigna la funcion a ejecutar con su ID como parametro
    for(int id = 0; id < T; id++){
            threads_ids[id] = id;  
            pthread_create(&misThreads[id], NULL, &multiplicador, (void*) &threads_ids[id]);
    }

    //Espera que todos los threads terminen su ejecucion
    for(int id = 0; id < T; id++){
        pthread_join(misThreads[id], NULL);
    }

    printf("Tiempo en segundos: %f\n", dwalltime() - timetick);


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
