#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

double *A, *minimosT;                   //arreglo a utilzar
int N, T, cant_elem_por_hilos;

pthread_mutex_t *miMutex;               //definición mutex
pthread_cond_t *sem;


//Para calcular tiempo
double dwalltime(){
    double sec;
    struct timeval tv;

    gettimeofday(&tv,NULL);
    sec = tv.tv_sec + tv.tv_usec/1000000.0;
    return sec;
}


void* busqueda(void* argum){

    int id = *(int *)argum;                             //id del hilo
    int condicion = TRUE;                              // es utilizada para mantener el hilo mientras sea necesario.
    double min = 99999999999;                           //variable auxiliar para calcular minimo
    int pos_ini = id*cant_elem_por_hilos;               //posicion desde la que el hilo procesa
    int pos_final = (id+1)*cant_elem_por_hilos;         // idem anterior pero hasta que finaliza.
    int mitad_actual = T*0.5;                              //es utilizada como variable auxiliar para acceder a las pos corresp
                                                        //a medida que se procesa.
    
    for(int i= pos_ini;i<pos_final;i++){
        if(A[i]<min){
            min=A[i];
        }
    }

    minimosT[id] = min;                         //Queda armado el vector de minimos de cada hilo
    printf("ID: %d     -> El minimo es %f\n", id, min);

    if ( id < mitad_actual && (T != 1)){    
    
        while ( id < mitad_actual){
            
            if (mitad_actual > 0){
                pthread_join(id + mitad_actual, NULL);
            }

            if( minimosT[id] > minimosT[id + mitad_actual] ){
                minimosT[id] = minimosT[id + mitad_actual];
            }
            mitad_actual = mitad_actual*0.5;
            printf("\n VALOR MITAD_ACTUAL %d\n", mitad_actual);
            
        }
   }
    pthread_exit(NULL);
}
 
/*m = 1
    0   1   2   3   4   5   6   7   8
    04 15 26 37 48
    02 13

*/


int main(int argc, char* argv[]){
      
    T = atoi(argv[1]);                      //cantidad de threads
    N = atoi(argv[2]);                      //dimension del arreglo
    double timetick;                        //para calcular tiempos de ejecucion

    //Aloca memoria para el arreglo
    A = (double*)malloc(sizeof(double)*N);                          //contendra los datos originales.
    minimosT = (double*)malloc(sizeof(double)*T);                   //se utilizara como auxiliar para compartir minimos
    int threads_ids[T];                                             //ids para los threads
    pthread_t misThreads[T];                                        //declaracion threads
                               //inicialización mutex

    
    printf("N es %d y T es %d \n",N,T);
    cant_elem_por_hilos = N/T;                               //se utiliza para determinar cuantos elementos por hilos se tienen

    for(int i=0;i<N;i++){
        A[i] = (rand()%10);
        printf("%lf ",A[i]);
    } 
    printf("\n");


    //INICIA LA OPERACION
    timetick = dwalltime();

    if ( T == 1){
        int mi_id = 0;
        busqueda(&mi_id);            //Version secuencial
    }
    else{ 
           //se encarga de generar los threads con sus respectivos parametros.
        for(int id=0;id<T;id++){
                threads_ids[id]=id;  //se utiliza para no generar inconsistencias con las ids

                pthread_create(&misThreads[id],NULL,&busqueda,(void*) &threads_ids[id]);
        }
        
        //espera de fin de ejecucion para los threads

    }

    pthread_join(0,NULL);


    printf("Tiempo en segundos: %f\n", dwalltime() - timetick);
    //por naturaleza de la solucion en la primer posicion siempre queda el minimo.
    printf("valor min %lf",minimosT[0]);
    
    free(A);
    free(minimosT);
    
    return 0;
}
