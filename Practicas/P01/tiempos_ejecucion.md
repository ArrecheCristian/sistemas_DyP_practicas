# 1A

ORIGINAL: 
N = 512  --> 5.481538
N = 1024  --> 60.370201
N = 2048  --> 759.746711


OPTIMIZADA:
N = 512  --> 3.890748
N = 1024  --> 31.298666
N = 2048  --> 250.133195

OPTIMIZADA SIN OVERHEAD DE FUNCIONES (inicializando C en la multiplicacion):
N = 512 --> 1.541226
N = 1024 --> 12.492689
N = 2048 --> 99.795987

OPTIMIZADA SIN OVERHEAD DE FUNCIONES (inicializando C junto con A y B):
N = 512 --> 1.573992
N = 1024 --> 13.460257
N = 2048 --> 103.399145

//En la v2 tarda un poco màs por el hecho de los fallos de cache ya analizados reiteradas veces. Por lo tanto la mejor version
del algoritmo secuencial es la que inicializa C dentro de la multiplicacion (matrices_optimizado.c)


# 1E

## ML 

    N = 512  --> 
        Bytes matriz triangular reducida (sin almacenar los ceros): 1050624
        Bytes matriz triangular completa (almacenando los ceros):   2097152

        Multiplicacion ML REDUCIDA:
            -> Tiempo en segundos: 0.834150 


        Multiplicacion ML COMPLETA:
            -> Tiempo en segundos: 1.536240

    N = 1024  --> 
        Bytes matriz triangular reducida (sin almacenar los ceros): 4198400
        Bytes matriz triangular completa (almacenando los ceros):   8388608

        Multiplicacion ML REDUCIDA:
            -> Tiempo en segundos: 6.695990 


        Multiplicacion ML COMPLETA:
            -> Tiempo en segundos: 12.573619 

    N = 2048  --> 
        Bytes matriz triangular reducida (sin almacenar los ceros): 16785408
        Bytes matriz triangular completa (almacenando los ceros):   33554432

        Multiplicacion ML REDUCIDA:
            -> Tiempo en segundos: 54.509206 


        Multiplicacion ML COMPLETA:
            -> Tiempo en segundos: 99.510527 

## LM
    
    N = 512  --> 
        Bytes matriz triangular reducida (sin almacenar los ceros): 1050624
        Bytes matriz triangular completa (almacenando los ceros):   2097152

        Multiplicacion LM REDUCIDA: 
            -> Tiempo en segundos: 0.795541 

        Multiplicacion LM COMPLETA:
            -> Tiempo en segundos: 1.577524 
    
    N = 1024  --> 
        Bytes matriz triangular reducida (sin almacenar los ceros): 4198400
        Bytes matriz triangular completa (almacenando los ceros):   8388608

        Multiplicacion LM REDUCIDA: 
            -> Tiempo en segundos: 6.519877 

        Multiplicacion LM COMPLETA:
            -> Tiempo en segundos: 12.828516
    
    N = 2048  --> 
        Bytes matriz triangular reducida (sin almacenar los ceros): 16785408
        Bytes matriz triangular completa (almacenando los ceros):   33554432

        Multiplicacion LM REDUCIDA: 
            -> Tiempo en segundos: 52.085495 

        Multiplicacion LM COMPLETA:
            -> Tiempo en segundos: 102.558810 

# 2

N=4

Calculando Fibonacci iterativo para n = 4...
 Resultado = 3
 Tiempo en segundos 0.0000000954 
Calculando Fibonacci recursivo para n = 4...
 Resultado = 3
 Tiempo en segundos 0.0000000954 

N=10

Calculando Fibonacci iterativo para n = 10...
 Resultado = 55
 Tiempo en segundos 0.0000002146 
Calculando Fibonacci recursivo para n = 10...
 Resultado = 55
 Tiempo en segundos 0.0000010014 

N=40

Calculando Fibonacci iterativo para n = 40...
  Resultado = 102334155
  Tiempo en segundos 0.0000003099 
Calculando Fibonacci recursivo para n = 40...
  Resultado = 102334155
  Tiempo en segundos 1.1901185036 