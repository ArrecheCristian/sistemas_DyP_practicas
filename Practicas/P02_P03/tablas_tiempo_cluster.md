# Matriz ABC - Secuencial

|                     |                 | **TAMAÑO N**   |                 |
|     :------:        |    :------:     |   :------:     |    :------:     |    
| **PROCESADORES**    | **N = 512**     |**N = 1024**    |**N = 2048**     |
| **T = 1**           |     1.952274    |    15.931320   |     127.489117  |





# Matriz ABC - Pthreads

|                     |                 | **TAMAÑO N**   |                 |
|     :------:        |    :------:     |   :------:     |    :------:     |    
| **PROCESADORES**    | **N = 512**     |**N = 1024**    |**N = 2048**     |
| **T = 2**           |      1.022975   |  8.363744      | 66.805081       |
| **T = 4**           |      0.513714   |  4.187032      | 33.565554       |
| **T = 8**           |      0.257757   |  2.092490      | 16.792256       |






# Matriz ABC - OpenMP

## 1 paralell con collapse 2

|                     |                 | **TAMAÑO N**   |                 |
|     :------:        |    :------:     |   :------:     |    :------:     |    
| **PROCESADORES**    | **N = 512**     |**N = 1024**    |**N = 2048**     |
| **T = 2**           |      1.145464   |  9.353984      | 74.971007       |
| **T = 4**           |      0.574770   |  4.696501      | 37.548378       |
| **T = 8**           |      0.289003   |  2.342039      | 18.796209       |


## 1 paralell sin collapse

|                     |                 | **TAMAÑO N**   |                 |
|     :------:        |    :------:     |   :------:     |    :------:     |    
| **PROCESADORES**    | **N = 512**     |**N = 1024**    |**N = 2048**     |
| **T = 2**           |      1.153188   |  9.374563      | 74.938275       |
| **T = 4**           |      0.576712   |  4.692198      | 37.564469       |
| **T = 8**           |      0.289309   |  2.342772      | 18.798650       |









## 2 paralell con collapse 2

|                     |                 | **TAMAÑO N**   |                 |
|     :------:        |    :------:     |   :------:     |    :------:     |    
| **PROCESADORES**    | **N = 512**     |**N = 1024**    |**N = 2048**     |
| **T = 2**           |      valor1     |  valor2        | valor3          |
| **T = 4**           |      0.575823   |  4.693059      | 37.574763       |
| **T = 8**           |      valor1     |  valor2        | valor3          |


## 2 paralell sin collapse

|                     |                 | **TAMAÑO N**   |                 |
|     :------:        |    :------:     |   :------:     |    :------:     |    
| **PROCESADORES**    | **N = 512**     |**N = 1024**    |**N = 2048**     |
| **T = 2**           |      valor1     |  valor2        | valor3          |
| **T = 4**           |      0.576753   |  4.702969      | 37.540922       |
| **T = 8**           |      valor1     |  valor2        | valor3          |






# Mínimo y máximo - Pthreads 

|                     |                     | **TAMAÑO N**      |                   |
|     :------:        |    :------:         |   :------:        |    :------:       |    
| **PROCESADORES**    | **N = 500000000**   |**N = 1000000000** |**N = 2000000000** |
| **T = 1**           |     3.057723        |  6.112944         | 12.219912         |
| **T = 4**           |     0.764439        |  1.529612         | 3.057116          |
| **T = 8**           |     0.385179        |  0.767107         | 1.534254          |
