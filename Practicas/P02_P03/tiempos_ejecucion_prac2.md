### matrices_secuencial: 
    N = 512 --> 2.443711
    N = 1024 --> 20.245921
    N = 2048 --> 160.700886


### matrices_pthread: (con 1 thread)
    N = 512 --> 2.666386
    N = 1024 --> 21.494860
    N = 2048 --> 171.555529

### matrices_pthread: (con 4 threads)
    N = 512 --> 0.675504
    N = 1024 --> 5.479214
    N = 2048 --> 43.717085

## Usando for collapse (2)
### matrices_openmp: (con 4 threads y 2 parallel)
    N = 512 --> 0.650771
    N = 1024 --> 5.326961
    N = 2048 --> 42.737875

### matrices_openmp: (con 4 threads y 1 parallel)
    N = 512 --> 0.688401
    N = 1024 --> 5.540969
    N = 2048 --> 44.243732

## Usando for
### matrices_openmp: (con 4 threads y 2 parallel)
    N = 512 -->  0.656081
    N = 1024 --> 5.416654
    N = 2048 --> 43.504272

### matrices_openmp: (con 4 threads y 1 parallel)
    N = 512 --> 0.668856
    N = 1024 --> 5.434090
    N = 2048 --> 43.873574
