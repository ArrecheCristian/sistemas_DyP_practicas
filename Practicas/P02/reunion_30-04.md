## comentarios generales y pthreads

Usar todo lo aprendido en la practica 1. Por ejemplo vimos que la multiplicacion es mucho mas rapido que una division, entonces por ejemplo es mucho mejor multiplicar por 0.5 que dividir por 2. 

No usar llamados a funciones (overhead de funciones)

Un detalle puede ser crear un hilo menos (T-1) y usar el main como un hilo más, llamando a la funcion explicitamente, la funcion tendria
que ser otra porque si el hilo main hace el pthread_exit termina el programa.

Para la ejecucion del caso secuencial podria hacerse un programa aparte (si sobra tiempo, no es necesario), y en el paralelo
ejecutar con 4 y 8 ponele. El algoritmo secuencial tambien correrlo en el cluster.

Tener en cuenta que la mejora de tiempo no sera muy significante cuando hay màs hilos que cores.


## OpenMP

#pragma omp parallel esta linea crea los hilos. Mejor encerrar la seccion de los hilos entre dos llaves asi, porque todo en la misma
linea el compilador se rompe:

#pragma omp parallel
{

}

Si uso "master" tienen que existir hilos, sino no tiene sentido., o sea usarlo adentro.

Para usar el collapse en el for tienen que estar los 3 for inmediatamente juntos. 

En el section entra 1 solo y los demas esperan. 







