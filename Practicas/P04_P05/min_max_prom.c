#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

#define ROOT 0

double dwalltime();


int main(int argc, char* argv[]){
    
    int cantidadDeProcesos, id, N;
    double *A;                          //matriz utilizada
    int i, j, k;
    double timetick;                    //medicion de tiempo
    MPI_Status status;

    if ((argc != 2) || ((atoi(argv[1])) <= 0)){                 //verifico parametros
        printf("\nUsar: %s n\n  n: Dimension de la matriz \n", argv[0]);
        exit(1);
    }

	MPI_Init(&argc, &argv);                                  //se inicializa el entorno de ejecucion MPI
	MPI_Comm_rank(MPI_COMM_WORLD,&id);
	MPI_Comm_size(MPI_COMM_WORLD,&cantidadDeProcesos);
    
    N = atoi(argv [1]);                                     //dimension de la matriz
	
    
    
    
    
    
    MPI_Finalize();
	return(0);
}



//Para calcular tiempo
double dwalltime(){
    double sec;
    struct timeval tv;

    gettimeofday(&tv,NULL);
    sec = tv.tv_sec + tv.tv_usec/1000000.0;
    return sec;
}
