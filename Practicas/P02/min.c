#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

double *A, *minimosT;              //arreglo a utilzar
int N,T,cant_elem_por_hilos;        

void* busqueda(void* argum){

    int id=*(int *)argum;
    double min=99999999999;
    int pos_ini = id*cant_elem_por_hilos;
    int pos_final;
    // si soy el ultimo threads debo procesar hasta el final en caso de ser necesario.
    if(id==T-1){
        pos_final = N;
    }else{
        pos_final = (id+1)*cant_elem_por_hilos;
    }
    for(int i= pos_ini;i<pos_final;i++){
        if(A[i]<min){
            min=A[i];
        }
    }
    minimosT[id]=min;
    printf("soy el hilo %d y mi minimo es %lf \n",id,min);
    printf("soy el hilo %d empiezo en la pos  %d y termino en la pos %d \n",id,pos_ini,pos_final-1);
        
    pthread_exit(NULL);
}
 

int main(int argc, char* argv[]){
      
    double min = 99999;
    T = atoi(argv[1]); //cantidad de threads
    N = atoi(argv[2]); //dimension del arreglo
    
    //Aloca memoria para el arreglo
    A=(double*)malloc(sizeof(double)*N);
    minimosT=(double*)malloc(sizeof(double)*T); 
    
    int threads_ids[T];      //ids para los threads
    pthread_t misThreads[T]; //declaracion
    
    printf("N es %d y T es %d \n",N,T);
    cant_elem_por_hilos = N/T;   //se utiliza para determinar cuantos elementos por hilos se tienen
    printf("vector A \n");
    for(int i=0;i<N;i++){
        A[i] = rand()%10;
        printf("%lf ",A[i]);
    } 
    printf("\n");
    //se encarga de generar los threads con sus respectivos parametros.
    for(int id=0;id<T;id++){
            threads_ids[id]=id;  //se utiliza para no generar inconsistencias con las ids
            pthread_create(&misThreads[id],NULL,&busqueda,(void*) &threads_ids[id]);
    }


    //espera de fin de ejecucion para los threads
    for(int id=0;id<T;id++){
        pthread_join(misThreads[id],NULL);
    }
    //calculo el valor minimo a partir del arreglo final generado por lo threads
    for(int i=0;i<T;i++){
        if(minimosT[i]<min){
            min=minimosT[i];
        }
    }

    printf("valor min %lf",min);
    free(A);
    free(minimosT);
    return 0;
}

