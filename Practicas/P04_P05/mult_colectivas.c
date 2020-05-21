#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>


static void procesoPadre(int, int, int);
static void procesoHijo(int, int, int);
double dwalltime();


int main(int argc, char* argv[]){
    
    int cantidadDeProcesos, id, N;

    if ((argc != 2) || ((atoi(argv[1])) <= 0)){                 //verifico parametros
        printf("\nUsar: %s n\n  n: Dimension de la matriz \n", argv[0]);
        exit(1);
    }

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&id);
	MPI_Comm_size(MPI_COMM_WORLD,&cantidadDeProcesos);
    
    N = atoi(argv [1]);            //dimension de la matriz

    if (id == 0){                                           //bloque de código que solo ejecuta el proceso 0
        procesoPadre(id, cantidadDeProcesos, N);    
    }
    else{                                                 //bloque de codigo que ejecutan los demas procesos
        procesoHijo(id, cantidadDeProcesos, N);
    }
    
	MPI_Finalize();
	return(0);
}



static void procesoPadre(int id, int cantidadDeProcesos, int N){
	double *A, *B, *C, *temp;
    int filas_por_proceso;                          
    int i, j, k;
    double timetick;
    register double aux;
    int check = 1;
    MPI_Status status;

    filas_por_proceso = N/cantidadDeProcesos;               //cantidad de filas que tendra que procesar cada proceso

    A = (double*) malloc(sizeof(double)*N*N);               //reserva memoria para las matrices
    B = (double*) malloc(sizeof(double)*N*N);
    C = (double*) malloc(sizeof(double)*N*N);
    temp = (double*) malloc(sizeof(double)*filas_por_proceso*N);


    for (i = 0; i < N; i++){                                //inicializa las matrices 
        for (j = 0; j < N; j++){
            A[i*N + j] = 1;
            B[i + j*N] = 1;     
        }
    }
    
    //SE REALIZA LA MULTIPLICACION MEDIANTE MPI
    timetick = dwalltime();                                     //el proceso 0 es el encargado de medir el tiempo

    MPI_Scatter(A, filas_por_proceso*N, MPI_DOUBLE, temp,               //Se divide A en partes iguales para cada proceso
                filas_por_proceso*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);    //En temp queda la parte de A que le toca procesar a cada uno (incluyendo el proceso 0)
    MPI_Bcast(B, N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);           //B se manda completa mediante broadcast

    for (i = 0; i < filas_por_proceso; i++){                    //El proceso 0 procesa su parte y lo almacena directamente en C
        for (j = 0; j < N; j++){                                
            aux = 0;
            for (k=0; k<N; k++){
                aux += temp[i*N + k] * B[k + j*N]; 
            }
            C[i*N + j] = aux;
        }
    }

    //Desde el proceso 0 mando la parte de C procesada por él a sí mismo. Luego, también en C, recibo el tramo procesado
    //por cada uno de los demas procesos
    MPI_Gather(C, filas_por_proceso*N, MPI_DOUBLE, C, filas_por_proceso*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      
    printf("Tiempo en segundos: %f\n", dwalltime() - timetick);



    //solo para corroborar la correcta asignacion de C, si es una matriz chica (8, 16) la imprime
    if ( N < 17) {             
        printf("\n\n");
        for (int i = 0; i < N; i++) {
            printf("| ");
            for (int j = 0; j < N; j++) {
                printf("%.f ", C[i*N + j]);
            }
            printf("|\n");
        }
    }
  
    //Verifica el resultado
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            check = check && (C[i*N + j] == N);
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
    free(temp);
}



static void procesoHijo(int id, int cantidadDeProcesos, int N){
	double *A, *B, *temp, *resultado;
    int filas_por_proceso;
    int i, j, k;
    register double aux;
    MPI_Status status;
    int check = 1;

    filas_por_proceso = N/cantidadDeProcesos;                         //cantidad de filas que debera procesar

    resultado = (double*) malloc(sizeof(double)*N*filas_por_proceso);           //para almacenar el resultado
    temp = (double*) malloc(sizeof(double)*N*filas_por_proceso);                //en temp recibo la parte de A que me toca procesar
    B = (double*) malloc(sizeof(double)*N*N);


    MPI_Scatter(A, filas_por_proceso*N, MPI_DOUBLE, temp, filas_por_proceso*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);                         //B la recibo completa por broadcast

    for (i=0; i<filas_por_proceso; i++){                                //realiza su porcion de calculo
        for (j=0; j<N; j++){
            aux = 0;
            for (k=0; k<N; k++){
                aux += temp[i*N +k]*B[k + j*N];
            }
            resultado[i*N + j] = aux;
        }            
    }    
    //Envio mi porcion de calculo al proceso 0 que las almacenara en orden segun id
    MPI_Gather(resultado, filas_por_proceso*N, MPI_DOUBLE, A, filas_por_proceso*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    free(B);
    free(temp);
    free(resultado);
}



//Para calcular tiempo
double dwalltime(){
    double sec;
    struct timeval tv;

    gettimeofday(&tv,NULL);
    sec = tv.tv_sec + tv.tv_usec/1000000.0;
    return sec;
}
