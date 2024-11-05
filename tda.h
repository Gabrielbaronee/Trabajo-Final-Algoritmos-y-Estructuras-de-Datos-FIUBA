#ifndef TDA_H_
#define TDA_H_

#include <stdlib.h>

#include "tda.c"

//Crea y devuelve la estructura menu para el programa.
menu_t *crear_menu(struct hospitales *hospital_general);

//Crea y devuelve la estructura hospitales(el hospital general del programa que almacena los hospitales).
struct hospitales *crear_hospital_general();

//Muestra la interfaz del menu para que el usuario se guie como usar el programa.
//Retorna -1 en caso de error o retorna 0 en caso de exito.
int menu_usuario(void *contexto);

//Muestra todas las opciones disponibles detalladamente para que el usuario sepa como usar el programa.
int ayuda_usuario(void *contexto);

//Crea un hospital y lo agrega a la estructura global donde se guardan todos los hospitales.
//Retorna -1 en caso de error o retorna 0 en caso de exito.
int crear_hospital(void *contexto);

//Muestra todos los detalles de los pokemones almacenados en el hospital activo.
//Retorna -1 en caso de error o retorna 0 en caso de exito.
int lista_pokemones(void *contexto);

//Muestra todos los nombres de los pokemones almacenados en el hospital activo.
//Retorna -1 en caso de error o retorna 0 en caso de exito.
int pokemones_en_hospital(void *contexto);

//Muestra todos los hospitales disponibles y el activo.
//Retorna -1 en caso de error o retorna 0 en caso de exito.
int hospitales_activos(void *contexto);

//Destruye el hospital activo y le pregunta al usuario que hospital desea activar.
//Retorna -1 en caso de error o retorna 0 en caso de exito.
int destruir_hospital(void *contexto);

//Activa el hospital que el usuario desee.
//Retorna -1 en caso de error o retorna 0 en caso de exito.
int activar_hospital(void *contexto);

//Ejecuta la opcion que el usuario eligio por pantalla, devuelve un booleano para determinar si
//seguir o no con el programa.
bool ejecutar_opcion(menu_t *menu, char desicion_usuario);

//Libera toda la memoria del programa.
//Retorna -1 en caso de error o retorna 0 en caso de exito.
int menu_liberar_memoria(void *contexto);

//Recibe un hash de comandos e inserta todas las funciones que se van a usar con sus respectivas claves.
//Devuelve el hash.
hash_t *insertar_hash(hash_t *comandos);

#endif // TDA_H_