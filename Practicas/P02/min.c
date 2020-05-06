#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

short int *A, *minimosT, *maximosT;
double *espera;      //arreglo a utilzar
int N, T, cant_elem_por_hilos;

pthread_mutex_t *miMutex;    //definición mutex
pthread_cond_t *sem;

//Para calcular tiempo
double dwalltime()
{
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

void* busqueda(void* argum){

    int id = *(int *)argum;                             //id del hilo
    int condicion = TRUE;                               //es utilizada para mantener el hilo mientras sea necesario.
    short int min =   9999;                           //variable auxiliar para calcular minimo
    short int max = -1;
    int pos_ini = id*cant_elem_por_hilos;               //posicion desde la que el hilo procesa
    int pos_final = (id+1)*cant_elem_por_hilos;         // idem anterior pero hasta que finaliza.
    int mitad_actual = T*0.5;                           //es utilizada como variable auxiliar para acceder a las pos corresp
                                                        //a medida que se procesa.
    
    for(int i= pos_ini;i<pos_final;i++){
        if(A[i]<min){
            min=A[i];
        }
        if(A[i]>max){
            max=A[i];
        }
    }
    minimosT[id] = min;     //Queda armado el vector de minimos de cada hilo
    maximosT[id] = max;

    if (T==1)
    {
        return 0;
    }
      
   
    while( condicion && (T != 1)){
      
    
        if ( id < mitad_actual){
           

            pthread_mutex_lock(&miMutex[id+mitad_actual]);
            
            if (espera[id+mitad_actual] == FALSE){
                pthread_cond_wait(&sem[id+mitad_actual],&miMutex[id+mitad_actual]);
       
                
            }
            espera[id] = FALSE;
            pthread_mutex_unlock(&miMutex[id+mitad_actual]);
       

            if( minimosT[id] > minimosT[id + mitad_actual] ){
                minimosT[id] = minimosT[id + mitad_actual];
            }
            if (maximosT[id] < maximosT[id+mitad_actual] ) {
                maximosT[id] = maximosT[id+mitad_actual];
            }
            mitad_actual = mitad_actual*0.5;
          
         
        }else{
         
           pthread_mutex_lock(&miMutex[id]);
        
            pthread_cond_signal(&sem[id]);
            espera[id] = TRUE; 
            pthread_mutex_unlock(&miMutex[id]);  
            condicion=FALSE;
            
        }
   }


    pthread_exit(NULL);
}
 

int main(int argc, char* argv[]){
      
    T = atoi(argv[1]);                      //cantidad de threads
    N = atoi(argv[2]);                      //dimension del arreglo
    double timetick;                       //para calcular tiempos de ejecucion

    //Aloca memoria para el arreglo
    A = (short int*)malloc(sizeof(short int)*N);                          //contendra los datos originales.
    minimosT = (short int*)malloc(sizeof(short int)*T);                   //se utilizara como auxiliar para compartir minimos
    maximosT = (short int*)malloc(sizeof(short int)*T);
    espera = (double*)malloc(sizeof(double)*T);                   //se utilizara como auxiliar para compartir minimos

    sem = (pthread_cond_t*)malloc(sizeof(pthread_cond_t)*T);        //arreglo de variables condicion
    miMutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t)*T);
    int threads_ids[T];                                             //ids para los threads
    pthread_t misThreads[T];                                        //declaracion threads
                             
    
    printf("N es %d y T es %d \n",N,T);
    cant_elem_por_hilos = N/T;                               //se utiliza para determinar cuantos elementos por hilos se tienen
   // printf("vector A \n");

    for(int i=0;i<N;i++){
        A[i] = (rand()%10);
//printf("%d ",A[i]);
    } 
    printf("\n");
    for(int id=0;id<T;id++){    
        espera[id] = FALSE;
        pthread_cond_init(&sem[id],NULL);
        pthread_mutex_init(&miMutex[id], NULL); 
    }

    

    if ( T == 1){
        //INICIA LA OPERACION
        timetick = dwalltime();
        
        int mi_id = 0;
        busqueda(&mi_id);            //Version secuencial
        
        printf("Tiempo en segundos: %.10lf\n", dwalltime() - timetick);
    }
    else{ 
        //INICIA LA OPERACION
        timetick = dwalltime();

           //se encarga de generar los threads con sus respectivos parametros.
        for(int id=0;id<T;id++){
                
                threads_ids[id]=id;  //se utiliza para no generar inconsistencias con las ids
                pthread_create(&misThreads[id],NULL,&busqueda,(void*) &threads_ids[id]);
        }
        
        //espera de fin de ejecucion para los threads
    
        for(int id=0;id<T;id++){
            pthread_join(misThreads[id],NULL);
        }
        printf("Tiempo en segundos: %.10lf\n", dwalltime() - timetick);
    }

    
    //en la primer posicion siempre queda el minimo.
    printf("valor min %d \n", minimosT[0]);
    printf("valor maximo %d\n",maximosT[0]);
    
    free(A);
    free(minimosT);
    
    return 0;
}

