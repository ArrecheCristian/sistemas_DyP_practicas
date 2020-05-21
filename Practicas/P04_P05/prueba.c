#include "mpi.h"
#include <stdio.h>
#define SIZE 4


static void procesoPadre(int, int, int);
static void procesoHijo(int, int, int);

int main(int argc, char *argv[])  {
    int cantidadDeProcesos, id, sendcount, recvcount, source;


    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &cantidadDeProcesos);

    if (id == 0) {
        // define source task and elements to send/receive, then perform collective scatter
        procesoPadre(id, cantidadDeProcesos, SIZE);
    }
    else{
        procesoHijo(id, cantidadDeProcesos, SIZE);
    }

    MPI_Finalize();
}

static void procesoPadre(int id, int cantidadDeProcesos, int N){
    double recvbuf[SIZE];
    double sendbuf[SIZE][SIZE] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 16}  };
    double resultado[SIZE][SIZE];
    
    double num = 3;

    MPI_Bcast(&num, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);                         //B se manda completa
    MPI_Scatter(sendbuf, SIZE, MPI_DOUBLE, recvbuf, SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    printf("rank= %d  Results: %.f %.f %.f %.f  Broadcast: %.f\n", id, recvbuf[0], recvbuf[1], recvbuf[2], recvbuf[3], num);
    
    printf("\n\n");
    MPI_Gather(recvbuf, SIZE, MPI_DOUBLE, resultado, SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    printf("rank= %d  Results: %.f %.f %.f %.f  Broadcast: %.f\n", id, resultado[3][3], resultado[2][3], resultado[2][2], resultado[3][0], num);


}


static void procesoHijo(int id, int cantidadDeProcesos, int N){
    double recvbuf[SIZE];
    double *sendbuf;
    double num;

    MPI_Scatter(sendbuf, SIZE, MPI_DOUBLE, recvbuf, SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&num, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);                         //B se manda completa

    recvbuf[0] = 9;
    recvbuf[1] = 9;
    recvbuf[2] = 9;
    recvbuf[3] = 9;

    MPI_Gather(recvbuf, SIZE, MPI_DOUBLE, sendbuf, SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    printf("rank= %d  Results: %.f %.f %.f %.f  Broadcast: %.f\n", id, recvbuf[0], recvbuf[1], recvbuf[2], recvbuf[3], num);
}