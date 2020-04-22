/**
Multiplicación de matrices ML.
Donde M es una matriz de N*N y L es una matriz triangular inferior. 
Analizar los tiempos de ejecución para la solución que almacena los ceros de la triangular respecto a la que no los almacena.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

//cantidad de ceros acumulados hasta el momento para recorrer las columnas de la matriz traingular
#define CEROS_HASTA_ACA(i) ( (i * (i + 1)) / 2 )


void llenar_matriz(double* m, const int N);
int get_elementos_triangular(const int N);
double dwalltime();
void matrix_print(const double * m, const int N);


int main(int argc, char** argv) {
    printf("\n\n");
    // Verifica que se recibe el tamaño de la matriz
    if ( argc != 2 ) {
        printf("Falta argumento N, dimension de la matriz \n");
        return -1;
    }

    int N = atoi(argv[1]);      //Tamaño de la matriz
    double *M,                  //Matriz M de N*N
           *L,                  //Matriz L triangular inferior con los ceros SIN almacenar
           *C,                  //Matriz C triangular inferior con los ceros almacenados 
           *O;                  //Matriz resultado
    double time;                //Tiempo de ejecución

    // Aloca espacio para las matrices
    M = (double*) malloc(sizeof(double) * N * N);
    C = (double*) malloc(sizeof(double) * N * N);
    O = (double*) malloc(sizeof(double) * N * N);
    L = (double*) malloc(sizeof(double) * get_elementos_triangular(N));

    printf("Bytes matriz triangular reducida (sin almacenar los ceros): %ld\n", (sizeof(double) * get_elementos_triangular(N)));
    printf("Bytes matriz triangular completa (almacenando los ceros):   %ld\n\n", (sizeof(double) * N * N));

    // Inicializa las matrices
    for (int i = 0; i < N * N; i++) M[i] = 1;                           //Matriz M completa con 1
    for (int i = 0; i < get_elementos_triangular(N); i++) L[i] = 1;     //Matriz L completa con 1
    for (int i = 0; i < N * N; i++) O[i] = 0;                           //Matriz O completa con 0

    for (int i = 0; i < N; i++) {                                       //Matriz C completa con 0 y 1
        for (int j = 0; j < N; j++) {
            if ( j - i <= 0) C[i + N * j] = 1;
            else C[i + N * j] = 0;
        }
    }

    // MULTIPLICACION ML: sin almacenar los ceros de la triangular 
    time = dwalltime();

    printf("Multiplicacion ML REDUCIDA:\n");
    for (int i = 0; i < N; i++) {               // Recorre por filas
        for (int j = 0; j < N; j++) {           // Recorre por columna
            for ( int k = j; k < N; k++) {      // Multiplica a partir de la diagonal

                //recorre M por filas y L por columnas para una mejor optimizacion
                O[i*N+j] = O[i*N+j] + M[i*N+k] * L[k + j * N - CEROS_HASTA_ACA(j) ];
            }
        }  
    }

    printf("    -> Tiempo en segundos: %f \n", dwalltime() - time);
    matrix_print(O, N);

    printf("\n");

    //MULTIPLICACION ML: almacenando los ceros de la matriz triangular
    for (int i = 0; i < N * N; i++) O[i] = 0;       //Vuelvo a inicializar la matriz resultado
    
    time = dwalltime();
    printf("Multiplicacion ML COMPLETA:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                O[i*N + j] = O[i*N + j] + M[i*N + k] * C[k  + N * j];
            }
        }
    }

    printf("    -> Tiempo en segundos: %f \n", dwalltime() - time);
    matrix_print(O, N);

    return 0;
} // fin main


//calcula la cantidad de elementos de la matriz triangular, restando los ceros.
int get_elementos_triangular(const int N) {
    return (N * N) - (( (N-1) * ( (N-1) + 1) ) / 2);
}


double dwalltime(){
	double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}


//funcion que se utilizo en un principio solo para corroborar que la multiplicacion se estaba realizando correctamente, por eso se imprime solo para matrices pequeñas
void matrix_print(const double * m, const int N) {

    if ( N < 15) {

        for (int i = 0; i < N; i++) {
            printf("| ");
            for (int j = 0; j < N; j++) {
                printf("%.2f ", m[i*N + j]);
            }
            printf("|\n");
        }
    }
    printf("\n");

}