Análisis de tiempos para ver de qué manera realizamos el ejercicio para entregar. Es decir, cuál es la forma más óptima de realizar las multiplicaciones, si mediante operaciones punto a punto o mediante operaciones colectivas.

# Multiplicación con operaciones punto a punto
## 1 proceso
N = 512 --> 1.198218
N = 1024 --> 10.018874
N = 2048 --> 80.444299
## 2 procesos
N = 512 --> 0.605188
N = 1024 --> 5.089371
N = 2048 --> 40.569648
## 4 procesos
N = 512 --> 0.316190
N = 1024 --> 2.647106
N = 2048 --> 20.742114

# Multiplicación con operaciones colectivas
## 1 proceso
N = 512 --> 1.132992
N = 1024 --> 10.004423
N = 2048 --> 79.444427
## 2 procesos
N = 512 --> 0.569572
N = 1024 --> 4.952024
N = 2048 --> 40.035495
## 4 procesos
N = 512 --> 0.303392
N = 1024 --> 2.542188
N = 2048 --> 20.060370

# Conclusión
Bueno la diferencia no es mucha, pero en todos los casos, por más mínimo que sea, conviene usar operaciones colectivas. Y por suerte coincide con lo que dijo el profe, que las operaciones colectivas eran más óptimas.


# Tiempos con modificaciones en el codigo para lograr mayor optimizacion

## 2 procesos
N = 512 --> 1.788713
N = 1024 --> 15.099385
N = 2048 --> 121.161589

## 2 procesos (modificando alocaciones y agregando 'datos_por_proceso')
N = 512 --> 1.546923
N = 1024 --> 14.750064
N = 2048 --> 120.134461

## 2 procesos (haciendo la multiplicacion AC junto con AB, en el mismo bloque for)
N = 512 --> 1.472392
N = 1024 --> 12.121693
N = 2048 --> 95.781365


