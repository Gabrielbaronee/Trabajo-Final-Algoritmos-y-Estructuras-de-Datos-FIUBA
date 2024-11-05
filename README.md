<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TP2

## Repositorio de (Gabriel Tomas Barone) - (109694) - (gbarone@fi.uba.ar)

- Para compilar:

```bash
gcc tp2.c -o programa
```

- Para ejecutar:

```bash
./programa
```

- Para ejecutar con valgrind:
```bash
valgrind --leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all ./programa
```
---

Funcionamiento del Programa

Hospitales disponibles a probar en el programa: "ejemplos/grande.txt" y "ejemplos/hospital1.txt"

ACLARACION: Al ingresar el nombre de un archivo, ingresarlo sin las comillas, ej: ingrese un nombre de archivo: ejemplos/grande.txt


<div align="center">
<img width="70%" src="https://github.com/GabrielBarone1/tp2-109694/assets/127968237/86290c6a-fe92-496a-af9d-fbfd5c6087ac">
</div>



---

## Respuestas a las preguntas teóricas
El vector dinamico de pokemones del tp1 podria reemplazarse con una lista simplemente enlazada.
Cuando se crea el hospital se ordenan los pokemones por salud, asi que, se tendria que cambiar la funcion de ordenar_pokemones. 

Se podria iterar toda la lista y guardar los pokemones en un vector auxiliar para aplicar el ordenamiento y volver a insertar ya los pokemones de manera
ordenada. La operacion del bubble sort para ordenar por salud de menor a mayor seria O(n^2).

En la funcion hospital_crear_desde_archivo con una lista, los pokemones se irian insertando al final de la lista como pasa con un vector dinamico. La operacion al insertar al final de la lista
teniendo un nodo que apunta al final de la lista seria O(1).

En hospital_aceptar_emergencias con una lista, directamente se podria agregar los pokemones nuevos de la ambulancia al final. Seria una operacion O(n) ya que se iteraria todo el vector nuevo de
pokemones que llegan en la ambulancia.
Despues habria que volver a aplicar la nueva funcion de ordenamiento. 

En la funcion hospital_obtener_pokemon no podemos acceder directamente a la prioridad como hacemos en un vector, asi que deberiamos iterar la lista hasta que la prioridad del pokemon buscado coincida con la posicion. 
Por ejemplo, Si la prioridad es cero el pokemon buscado se va a  encontrar en el nodo del inicio de la lista. Esa operacion seria O(1), pero si hay que iterar por la lista buscando otra prioridad 
la complejidad va a ser O(n).

En la funcion hospital_a_cada_pokemon iriamos iterando hasta el final de la lista cada nodo e iriamos pasando por la funcion el nodo actual. Como iterariamos la lista completa(o hasta que la funcion
 devuelva false) La complejidad seria O(n).

 En la funcion hospital_destruir, al ser una lista, deberiamos hacer una llamada a lista_quitar con todos los nodos, liberando la memoria de cada uno.
 Se iria llamando a la funcion de quitar nodo eliminando siempre el ultimo de la lista hasta que no quede ninguno.
 Eliminar el nodo del final tiene complejidad O(1), pero al hacerlo con toda la lista, haciendo una iteracion hasta que no quede ningun elemento, la complejidad final seria O(n).

El vector dinamico de pokemones se podria reemplazar tambien con un arbol binario de busqueda.
Al crearse el hospital se iria insertando los pokemones en el arbol con una funcion comparadora a elegir(se podria ordenar por id, o por salud, o por la primera letra del nombre del pokemon). La complejidad de insertar en un arbol binario en general es O(log(n)), en el peor de los casos es O(n). 

Pero eventualmente al tener el arbol de pokemones se debe ordenar por su salud. Podriamos crear un vector para almacenar los pokemones en orden de salud haciendo un recorrido INORDEN del arbol 
para insertar en un arbol nuevo y que quede todo ordenado. La complejidad del recorrido inorden seria O(n) ya que tendriamos que recorrer cada nodo del arbol para guardarlo en un vector.

En hospital_aceptar_emergencias se podrian insertar los pokemones nuevos, hacer un recorrido inorden nuevo para determinar la prioridad de la salud de cada pokemon y volver a insertar en un arbol quedando 
asi ordenado. Ahora al volver a tener un vector que su salud va de menor a mayor, al insertar todos los pokemones de vuelta nos quedaria un arbol en forma de lista. La complejidad de esta nueva insercion seria O(n).

En la funcion hospital_obtener_pokemon ahora, con nuestro nuevo arbol en forma de lista, tendriamos que tener un contador que empieze en 0 y vaya iterando hasta que sea igual a la prioridad(que comienza en 0).
Esta operacion seria O(n) porque estamos recorriendo el arbol en forma de lista.

En la funcion hospital_a_cada_pokemon iriamos iterando por el arbol metiendo cada nodo en la funcion. La complejidad seria O(n).

Y en la funcion destruir_hospital, al tener el arbol en forma de lista, destruirlo tendria la complejidad O(n).

Si se implementara la estructura pokemones con una estructura distinta a un vector dinamico(como por ejemplo la lista o el arbol), las pruebas deberian cambiar y adaptarse al nuevo TDA propuesto.
