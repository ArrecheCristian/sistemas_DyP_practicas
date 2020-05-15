#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static void procesoPadre(int,int);
static void procesoHijo(int,int);

int main(int argc, char* argv[]){
    char *mensaje, *respuesta;
    int cantidadDeProcesos, id;
    int ack, rta;


	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&id);
	MPI_Comm_size(MPI_COMM_WORLD,&cantidadDeProcesos);
    

    //bloque de código que solo ejecuta el proceso 0
    if (id == 0){
        procesoPadre(id, cantidadDeProcesos);
    }
    else{
        procesoHijo(id, cantidadDeProcesos);
    }
    
	MPI_Finalize();
	return(0);
}

static void procesoPadre(int id, int cantidadDeProcesos){
	int ack, rta; 
    MPI_Status status;

    for (int i=1; i<cantidadDeProcesos; i++){                       //envia la matriz a todos los procesos
        ack = 0;
        MPI_Send(&ack, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
    }

    for (int i=1; i<cantidadDeProcesos; i++){                       //envia la matriz a todos los procesos
        printf("Proceso %d --> Espero a %d \n", id, i);
        MPI_Recv(&rta, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
        printf("Proceso %d --> recibi '%d' \n", id, rta); 
    }

}

static void procesoHijo(int id, int cantidadDeProcesos){
	int ack, rta; 
    MPI_Status status;

    MPI_Recv(&ack, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    printf("Proceso %d --> recibi '%d' \n", id, ack); 

    rta = id;    
    MPI_Send(&rta, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    printf("Proceso %d --> Procesado\n", id);

}