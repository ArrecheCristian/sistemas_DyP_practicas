Mejor version secuencial del algoritmo de referencia (matrices_optimizado.c):

OPTIMIZADA SIN OVERHEAD DE FUNCIONES: (secuencial matrices_optimizado.c)
    N = 512 --> 1.217065
    N = 1024 --> 10.122610
    N = 2048 --> 81.2619977


matrices_pthread: (con 4 threads)
    N = 512 --> 0.675504
    N = 1024 --> 5.479214
    N = 2048 --> 43.717085


matrices_openmp: (con 4 threads)
    N = 512 --> 0.650771
    N = 1024 --> 5.326961
    N = 2048 --> 42.737875

