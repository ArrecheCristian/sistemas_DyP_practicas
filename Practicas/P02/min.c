#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdbool.h>
#include <math.h>

double *A, *minimosT;    //arreglo a utilzar
int N,T,cant_elem_por_hilos;
int ejecutados=0;        
pthread_mutex_t miMutex; //definición mutex
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

int id=*(int *)argum; //id del hilo
bool condicion = true; // es utilizada para mantener el hilo mientras sea necesario.
double min=99999999999; //variable auxiliar para calcular minimo
int pos_ini = id*cant_elem_por_hilos; //posicion desde la que el hilo procesa
int pos_final = (id+1)*cant_elem_por_hilos; // idem anterior pero hasta que finaliza.
int hilos_en_iter=T; //es utilizada como variable auxiliar para acceder a las pos corresp
                        //a medida que se procesa.
 
for(int i= pos_ini;i<pos_final;i++){
        if(A[i]<min){
            min=A[i];
        }
    }
    
minimosT[id]=min;
while( condicion ){
  
    if ( id < hilos_en_iter/2 ){
     	pthread_mutex_lock(&miMutex);
        printf("soy %d y estoy esperando a %d \n",id,id+hilos_en_iter/2);
        pthread_cond_wait(&sem[id],&miMutex);
        pthread_mutex_unlock(&miMutex);
        printf("soy %d y termine de esperar a %d \n",id,id+hilos_en_iter/2);

	if( minimosT[id] > minimosT[id + hilos_en_iter/2] ){
		minimosT[id] = minimosT[id + hilos_en_iter/2];
	}
    hilos_en_iter=hilos_en_iter/2;
    }else{
        pthread_mutex_lock(&miMutex);
        printf("soy %d despertando a %d \n",id,id-hilos_en_iter/2);
        pthread_cond_signal(&sem[id-hilos_en_iter/2]);
        pthread_mutex_unlock(&miMutex);    
        condicion=false;
	}

    

   }
   /// printf("soy el hilo %d y mi minimo es %lf \n",id,minimosT[id]);
    //printf("empiezo en la pos  %d y termino en la pos %d \n",pos_ini,pos_final-1);
    pthread_exit(NULL);
}
 

int main(int argc, char* argv[]){
      
    T = atoi(argv[1]); //cantidad de threads
    N = atoi(argv[2]); //dimension del arreglo
    double timetick;                //para calcular tiempos de ejecucion

    //Aloca memoria para el arreglo
    A=(double*)malloc(sizeof(double)*N); //contendra los datos originales.
    minimosT=(double*)malloc(sizeof(double)*T); //se utilizara como auxiliar para compartir minimos
    sem=(pthread_cond_t*)malloc(sizeof(pthread_cond_t)*T); //arreglo de variables condicion
    int threads_ids[T];      //ids para los threads
    pthread_t misThreads[T]; //declaracion threads
    pthread_mutex_init(&miMutex, NULL); //inicialización mutex

    
    printf("N es %d y T es %d \n",N,T);
    cant_elem_por_hilos = N/T;   //se utiliza para determinar cuantos elementos por hilos se tienen
    printf("vector A \n");
    for(int i=0;i<N;i++){
        A[i] =pow((-rand()%10),3);
        printf("%lf ",A[i]);
    } 
    printf("\n");
      for(int id = 0; id < T; id++){
                    pthread_cond_init(&sem[id],NULL); //se aprovecha el for para inicializar sema    }
    
    }
    //tomo el tiempo
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

    printf("Tiempo en segundos: %f\n", dwalltime() - timetick);

    //por naturaleza de la solucion en la primer posicion siempre queda el minimo.
    printf("valor min %lf",minimosT[0]);
    free(A);
    free(minimosT);
    return 0;
}

