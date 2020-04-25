/**
   Implementar una solución a la multiplicación de matrices AA donde la matriz A es de
   N*N. Plantear dos estrategias:
    
   1. Ambas matrices A están ordenadas en memoria por el mismo criterio (filas o
      columnas) .
   2. La matriz A de la izquierda sigue un criterio de ordenación en memoria. La
      matriz A de la derecha se construye a partir de la transposición de la matriz
      anterior. Se debe tener en cuenta el tiempo de la transposición.
   ¿Cuál de las dos estrategias alcanza mejor rendimiento?
 */

#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>

#define ORDENXFILAS 1
#define ORDENXCOLUMNAS 0

/* Time in seconds from some point in the past */
double dwalltime();

double getValor(double *matriz,int fila,int columna,int orden);

void setValor(double *matriz,int fila,int columna,int orden,double valor);

int N;

int main(int argc,char*argv[]){
   double *A,*B,*AT;
   int i,j,k;
   int check=1;
   double timetick;

   //Controla los argumentos al programa
   if (argc < 2){
      printf("\n Falta un argumento:: N dimension de la matriz \n");
      return -1;
   }
      
   N = atoi(argv[1]);

   A=(double*)malloc(sizeof(double)*N*N);
   B=(double*)malloc(sizeof(double)*N*N);
   AT=(double*)malloc(sizeof(double)*N*N);



   //inicializamos la matriz en 1
   for(i=0;i<N;i++){
      for(j=0;j<N;j++){
	      A[i*N+j]=1;
         AT[i*N+j]=1;
         B[i*N+j]=0;
      }
   }   

   timetick = dwalltime();

   //Hacemos la multiplicacion
   for(i=0;i<N;i++){
      for(j=0;j<N;j++){
         setValor(B,i,j,ORDENXFILAS,0);
         for(k=0;k<N;k++){
	         setValor(B,i,j,ORDENXFILAS, getValor(B,i,j,ORDENXFILAS) + getValor(A,i,k,ORDENXFILAS)*getValor(A,k,j,ORDENXFILAS));
         }
      }
   }      

   printf("Tiempo en segundos %f\n", dwalltime() - timetick);

   //Verifica el resultado
   for(i=0;i<N;i++){
      for(j=0;j<N;j++){
	      check=check&&(getValor(B,i,j,ORDENXFILAS)==N);
      }
   }         

   if(check){
      printf("Multiplicacion de matrices resultado correcto\n");
   }else{
      printf("Multiplicacion de matrices resultado erroneo\n");
   }

   //Version transponiendo la matriz

   timetick = dwalltime();

   //formamos A traspuesta
   for(i=0;i<N;i++){
      for(j=0;j<N;j++){
	      AT[i*N+j]=A[i+j*N];
      }
   }

   //Hacemos la multiplicacion
   for(i=0;i<N;i++){
      for(j=0;j<N;j++){
         setValor(B,i,j,ORDENXFILAS,0);
         for(k=0;k<N;k++){
	         setValor(B,i,j,ORDENXFILAS, getValor(B,i,j,ORDENXFILAS) + getValor(A,i,k,ORDENXFILAS)*getValor(AT,k,j,ORDENXCOLUMNAS));
         }
      }
   }      

   printf("Tiempo en segundos para trapueta %f\n", dwalltime() - timetick);

   //Verifica el resultado
   for(i=0;i<N;i++){
      for(j=0;j<N;j++){
	      check=check&&(getValor(B,i,j,ORDENXFILAS)==N);
      }
   }         

   if(check){
      printf("Multiplicacion de matrices resultado correcto\n");
   }else{
      printf("Multiplicacion de matrices resultado erroneo\n");
   }



   free(A);
   free(B);
   return(0);
}


//Retorna el valor de la matriz en la posicion fila y columna segun el orden que este ordenada
double getValor(double *matriz,int fila,int columna,int orden){
    if(orden==ORDENXFILAS){
        return(matriz[fila*N+columna]);
    } else {
        return(matriz[fila+columna*N]);
    }
}

//Establece el valor de la matriz en la posicion fila y columna segun el orden que este ordenada
void setValor(double *matriz,int fila,int columna,int orden,double valor){
    if(orden==ORDENXFILAS){
        matriz[fila*N+columna]=valor;
    } else{
        matriz[fila+columna*N]=valor;
    }
}

double dwalltime(){
	double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}