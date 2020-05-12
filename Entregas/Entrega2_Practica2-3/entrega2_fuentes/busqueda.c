#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

short int *A, *minimosT, *maximosT;
double *espera;     
int N, T, cant_elem_por_hilos;

pthread_mutex_t *miMutex;                   //definición mutex
pthread_cond_t *c;                          //definicion variable condicion

//Para calcular tiempo
double dwalltime()
{
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

void* busqueda(void* id_t){

    int id = *(int *) id_t;     
    int continuar = TRUE;                              //es utilizada para mantener el hilo mientras sea necesario.
    short int min = 9999;                              //auxiliares para calcular minimo y maximo
    short int max = -1;
    int pos_ini = id*cant_elem_por_hilos;               //indices para recorrido de los hilos
    int pos_final = (id+1)*cant_elem_por_hilos;         
    int mitad_actual = T*0.5;                   //variable auxiliar para acceder a las pos correspondiente
                                                //a medida que se procesa. Multiplicar por 0.5 es mas optimo que dividir por 2

    for(int i= pos_ini;i<pos_final;i++){        //cada hilo calcula el minimo y maximo de su porcion 
        if( A[i] < min ){
            min=A[i];
        }
        if( A[i] > max ){
            max=A[i];
        }
    }
    minimosT[id] = min;     //Quedan armado los vectores de min y max con los datos que proceso cada hilo
    maximosT[id] = max;


    if ( T==1 ){               //version secuencial
        return 0;
    }

    // version paralela
    while( continuar ){
      
        if ( id < mitad_actual ){                        //los primeros hilos continuan trabajando de a pares
           
            pthread_mutex_lock(&miMutex[id+mitad_actual]);
            if ( espera[id+mitad_actual] ){        //si mi vecino no termino, lo espero
                pthread_cond_wait(&c[id+mitad_actual],&miMutex[id+mitad_actual]);                
            }
            pthread_mutex_unlock(&miMutex[id+mitad_actual]);
       
            if( minimosT[id] > minimosT[id + mitad_actual] ){       //comparo mi minimo con el de mi vecino
                minimosT[id] = minimosT[id + mitad_actual];
            }
            if (maximosT[id] < maximosT[id+mitad_actual] ) {        //comparo mi maximo con el de mi vecino
                maximosT[id] = maximosT[id+mitad_actual];
            }
            mitad_actual = mitad_actual*0.5;      
        }else{
         
            pthread_mutex_lock(&miMutex[id]);
            pthread_cond_signal(&c[id]);      //si el hilo vecino estaba dormido, lo despierta
            espera[id] = FALSE;         //le avisa al hilo vecino que su dato ya esta disponible. No tiene que esperarlo mas
            pthread_mutex_unlock(&miMutex[id]);  
            continuar = FALSE;              //este hilo ya termino su procesamiento
        }
    }
    pthread_exit(NULL);
}
 


int main(int argc, char* argv[]){
      
    //Controla los argumentos al programa
    if ((argc != 3) || ((atoi(argv[1])) <= 0)){
        printf("\nUsar: %s t n\n  t: Cantidad de Threads \n  n: Dimension del vector \n", argv[0]);
        exit(1);
    }

    T = atoi(argv[1]);                      //cantidad de threads
    N = atoi(argv[2]);                      //dimension del arreglo
    double timetick;                       //para calcular tiempos de ejecucion

    //Aloca memoria para el arreglo
    A = (short int*)malloc(sizeof(short int)*N);                          //contendra los datos originales.
    minimosT = (short int*)malloc(sizeof(short int)*T);                   //se utilizara como auxiliar para compartir minimos
    maximosT = (short int*)malloc(sizeof(short int)*T);
    espera = (double*)malloc(sizeof(double)*T);                   //se utilizara como auxiliar para compartir minimos

    c = (pthread_cond_t*)malloc(sizeof(pthread_cond_t)*T);        //arreglo de variables condicion
    miMutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t)*T);
    int threads_ids[T];                                             //ids para los threads
    pthread_t misThreads[T];                                        //declaracion threads
                             
    cant_elem_por_hilos = N/T;                               //se utiliza para determinar cuantos elementos por hilos se tienen

    //inicializa vector aleatorio
    srand (time(NULL));
    
    for(int i=0; i<N; i++){
        A[i] = rand();
        if (N < 15){                  //si es un vector chico lo imprimos para corroborar el correcto funcionamiento
            printf ("%d |", A[i]);
        }
    } 

    printf("\n");
    for(int id=0; id<T; id++){                  //inicializacion de mutexes y variables condicion
        espera[id] = TRUE;
        pthread_mutex_init(&miMutex[id], NULL); 
        pthread_cond_init(&c[id],NULL);
    }

    
    //VERSION SECUENCIAL
    if ( T == 1){
        printf("Comienza busqueda\n");
        timetick = dwalltime();

        int mi_id = 0;
        busqueda(&mi_id);       //llama explicitamente a la funcion 
        
        printf("Tiempo en segundos: %.10lf\n", dwalltime() - timetick);
        printf("Finaliza busqueda\n");
    }
    else{ 
    //VERSION PARALELA
        printf("Comienza busqueda\n");
        timetick = dwalltime();

        //se crean los threads con sus respectivos parametros.
        for(int id=0; id<T; id++){
                threads_ids[id]=id;  //se utiliza para no generar inconsistencias con las ids
                pthread_create(&misThreads[id],NULL,&busqueda,(void*) &threads_ids[id]);
        }
        
        //espera de fin de ejecucion para los threads
        for(int id=0;id<T;id++){
            pthread_join(misThreads[id],NULL);
        }

        printf("Tiempo en segundos: %.10lf\n", dwalltime() - timetick);
        printf("Finaliza busqueda\n");
    }

    
    //El minimo y maximo quedan almacenados en la primer posicion del vector
    printf("\nMinimo ---> %d", minimosT[0]);
    printf("\nMaximo ---> %d\n",maximosT[0]);
    

    //Libera espacios y destruye los mutexes y variables condicion
    for(int id=0; id<T; id++){    
        pthread_mutex_destroy(&miMutex[id]); 
        pthread_cond_destroy(&c[id]);
    }

    free(A);
    free(minimosT);
    free(maximosT);
    free(espera);
    free(c);
    free(miMutex);
    
    return 0;
}

