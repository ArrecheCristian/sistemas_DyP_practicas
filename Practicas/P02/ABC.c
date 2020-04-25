#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


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


void function(void *arg){
    int tid=*(int*arg);// aca hay que determinar como carajos obtenemos la fila y la col segun id
    int i= tid*N; //fila desde donde arranco
    int j=tid*N;
    for(k=0;k<N;k++){
	setValor(C,i,j,ORDENXFILAS, getValor(C,i,j,ORDENXFILAS) + 
    getValor(A,i,k,ORDENXFILAS)*getValor(B,k,j,ORDENXCOLUMNAS));

}




int main(int argc, char* argv[]){
    
    int T= atoi(argv[1]);  //cantidad de threads que se utilizaran
    N= atoi(argv[2]); //dimension de las matrices

    pthread_t misThreads[T];
    int trhead_ids[T];
    

    //Aloca memoria para las matrices
    A=(double*)malloc(sizeof(double)*N*N);
    B=(double*)malloc(sizeof(double)*N*N);
    C=(double*)malloc(sizeof(double)*N*N);
    D=(double*)malloc(sizeof(double)*N*N);
 
    
    //Inicializa las matrices A  B y C en 1
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            setValor(A,i,j,ORDENXFILAS,1);
            setValor(B,i,j,ORDENXCOLUMNAS,1);   
            setValor(C,i,j,ORDENXCOLUMNAS,1);    
        }
    }   
 
    //se encarga de generar los threads con sus respectivos parametros.
    for(int id=0;id<T;id++){
            threads_ids[id]=id;  // se utiliza para no generar inconsistencias con las ids
            pthread_create(&misThrads[id],NULL,&funcion,(void*)&threads_id[id]);
    }


    //espera de fin de ejecucion para los threads
    for(int id=0;id<T;id++){
        pthread_join(misThreads[id],NULL);
    }
    return 0;
}