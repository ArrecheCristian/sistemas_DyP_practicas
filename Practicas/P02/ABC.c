#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#define ORDENXFILAS 0
#define ORDENXCOLUMNAS 1



double *A;
double *B;
double *C;
double *D;
int N;  

//Retorna el valor de la matriz en la posicion fila y columna segun el orden que este ordenada
double getValor(double *matriz,int fila,int columna,int orden){
 if(orden==ORDENXFILAS){
  return(matriz[fila*N+columna]);
 }else{
  return(matriz[fila+columna*N]);
 }
}

//Establece el valor de la matriz en la posicion fila y columna segun el orden que este ordenada
void setValor(double *matriz,int fila,int columna,int orden,double valor){
 if(orden==ORDENXFILAS){
  matriz[fila*N+columna]=valor;
 }else{
  matriz[fila+columna*N]=valor;
 }
}

//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}


void* multiplicador (void* argum){
    int id =*(int *)argum;
    int i = (id-(id % N ))/N;      //fila desde donde arranco
    int j = (id % N);         //columna desde donde arranco         
    for(int k=0;k<N;k++){
	   C[id] = C[id] + A[i*N + k]*B[k + j*N]; 
    }
    printf("mi id es %d proceso A desde %d y B desde %d y el valor calculado es %fs\n",id,i,j,getValor(C,i,j,ORDENXFILAS));
   
}




int main(int argc, char* argv[]){
    
    int T = atoi(argv[1]);  //cantidad de threads que se utilizaran
    N = atoi(argv[2]); //dimension de las matrices
    int check = 1;
    pthread_t misThreads[T];
    int threads_ids[T];
    printf("N es %d y T es %d\n",N,T);


    //Aloca memoria para las matrices
    A=(double*)malloc(sizeof(double)*N*N);
    B=(double*)malloc(sizeof(double)*N*N);
    C=(double*)malloc(sizeof(double)*N*N);
   
    
    
    //Inicializa las matrices A  B y C en 1
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
           //setValor(A,i,j,ORDENXFILAS,1);
            A[i*N + j] = 1;                         //Para evitar el overhead de funciones, accede directamente a la matriz
                                                    //eliminando el llamado a funciones
            //setValor(B,i,j,ORDENXCOLUMNAS,1);
            B[i + j*N] = 1;
            
            //inicializa la matriz resultado C
            C[i*N + j] = 0;  
        }
    }   
 
    //se encarga de generar los threads con sus respectivos parametros.
    for(int id=0;id<T;id++){
            threads_ids[id]=id;  // se utiliza para no generar inconsistencias con las ids
            pthread_create(&misThreads[id],NULL,&multiplicador,(void*) &threads_ids[id]);
    }


    //espera de fin de ejecucion para los threads
    for(int id=0;id<T;id++){
        pthread_join(misThreads[id],NULL);
    }



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


    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            printf("A[%d.%d]= %lf\n",i,j,A[i*N + j]);
            
        }
    }
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
               printf("B[%d.%d]= %lf \n",i,j,B[i + j*N]);
            
        }
    }
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
           printf("C[%d.%d]= %lf \n",i,j,C[i*N + j]);
            
        }
    }

           



    free(A);
    free(B);
    free(C);

    return 0;
}
