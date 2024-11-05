#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/tp1.h"
#include "src/tp1.c"
#include "src/pokemon.h"
#include "src/pokemon.c"
#include "hash.h"
#include "hash.c"

#define ERROR -1
#define EXITO 0

typedef struct hospitales {
	size_t cant_hospitales;
	size_t valor_actual;
	hospital_t **hospitales_creados;
	int asignar_id_hospital;
} hospital_general_t;

typedef struct menu {
	char desicion_usuario;
	struct hospitales *hospital_general;
	hash_t *comandos;
	bool programa_finalizado;
} menu_t;

menu_t *crear_menu(struct hospitales *hospital_general)
{
	if (!hospital_general)
		return NULL;

	menu_t *menu = malloc(sizeof(menu_t));
	if (!menu)
		return NULL;

	menu->hospital_general = hospital_general;
	menu->programa_finalizado = false;

	return menu;
}

struct hospitales *crear_hospital_general()
{
	struct hospitales *hospitales = malloc(sizeof(struct hospitales));
	if (!hospitales)
		return NULL;
	hospitales->hospitales_creados = malloc(sizeof(hospital_t));
	if (!hospitales->hospitales_creados) {
		free(hospitales);
		return NULL;
	}
	hospitales->cant_hospitales = 0;
	hospitales->valor_actual = 0;
	hospitales->asignar_id_hospital = 0;

	return hospitales;
}

int menu_usuario(void *contexto)
{
	struct hospitales *hospitales = contexto;

	if (!hospitales)
		return ERROR;

	if (hospitales->cant_hospitales > 0) {
		printf("Hospital actual: %s\n",
		       hospitales->hospitales_creados[hospitales->valor_actual]
			       ->nombre_archivo);
		printf("ID del hospital actual: %i\n",
		       hospitales->hospitales_creados[hospitales->valor_actual]
			       ->numero_id);
	}

	printf("\n");
	printf("Opciones Disponibles: \n");
	printf("-H (ayuda/help)\n");
	printf("-C (Crear un hospital)\n");
	printf("-E (Estado del hospital)\n");
	printf("-A (Activar un hospital)\n");
	printf("-M (Mostrar nombres del hospital)\n");
	printf("-L (Listar datos del hospital)\n");
	printf("-D (Destruir el hospital)\n");
	printf("-S (salir/exit)\n");

	return EXITO;
}

int ayuda_usuario(void *contexto)
{
	printf("Los comandos disponibles son:\n");
	printf("- H (ayuda/help): Muestra un menú de ayuda con los comandos disponibles.\n");
	printf("- C (cargar): Pide un nombre de archivo e intenta cargarlo creando un hospital. El hospital queda identificado con un número y el nombre del archivo.\n");
	printf("- E (estado): Muestra un listado con los hospitales cargados.\n");
	printf("- A (activar): Pide un número de identificación y activa el hospital. El resto de las operaciones se realizan sobre el hospital activo.\n");
	printf("- M (mostrar): Muestra un listado con los nombres de todos los pokemones en el hospital.\n");
	printf("- L (listar): Muestra un listado detallado de todos los pokemones en el hospital.\n");
	printf("- D (destruir): Destruye el hospital activo.\n");
	printf("- S (salir/exit): Sale del programa (obviamente que libera toda la memoria).\n");

	return EXITO;
}

int crear_hospital(void *contexto)
{
	struct hospitales *hospitales = contexto;

	if (!hospitales)
		return ERROR;

	char nombre_hospital[50];
	printf("Ingrese el nombre del archivo (No ingrese un nombre con espacios!): ");
	scanf(" %s", nombre_hospital);

	hospital_t *hospital = hospital_crear_desde_archivo(nombre_hospital);
	if (!hospital)
		return ERROR;

	printf("Creando hospital..\n");
	printf("El hospital se ha creado con exito!\n");
	if (hospitales->cant_hospitales >= 1) {
		hospital_t **hospitales_aux = realloc(
			hospitales->hospitales_creados,
			sizeof(hospital_t) * (hospitales->cant_hospitales + 1));
		if (hospitales_aux == NULL) {
			return ERROR;
		}
		hospitales->hospitales_creados = hospitales_aux;
	}

	strcpy(hospital->nombre_archivo, nombre_hospital);
	hospital->numero_id = hospitales->asignar_id_hospital;
	hospitales->hospitales_creados[hospitales->cant_hospitales] = hospital;
	hospitales->cant_hospitales++;

	return EXITO;
}

int lista_pokemones(void *contexto)
{
	struct hospitales *hospitales = contexto;

	if (!hospitales)
		return ERROR;

	printf("Lista de Pokemones del Hospital con ID : %zu\n",
	       hospitales->valor_actual);
	printf("\n");
	for (size_t i = 0;
	     i < hospitales->hospitales_creados[hospitales->valor_actual]
			 ->cantidad_pokemon;
	     i++) {
		printf("Nombre del pokemon: %s\n",
		       hospitales->hospitales_creados[hospitales->valor_actual]
			       ->pokemones[i]
			       ->nombre);
		printf("ID del pokemon: %zu\n",
		       hospitales->hospitales_creados[hospitales->valor_actual]
			       ->pokemones[i]
			       ->id);
		printf("Salud del pokemon: %zu\n",
		       hospitales->hospitales_creados[hospitales->valor_actual]
			       ->pokemones[i]
			       ->salud);
		printf("Nombre del entrenador: %s\n",
		       hospitales->hospitales_creados[hospitales->valor_actual]
			       ->pokemones[i]
			       ->nombre_entrenador);
		printf("\n");
	}

	return EXITO;
}

int pokemones_en_hospital(void *contexto)
{
	struct hospitales *hospitales = contexto;

	if (!hospitales)
		return ERROR;

	printf("\n");
	printf("Nombres de todos los Pokemones del Hospital\n");
	for (size_t i = 0;
	     i < hospitales->hospitales_creados[hospitales->valor_actual]
			 ->cantidad_pokemon;
	     i++) {
		printf("- %s\n",
		       hospitales->hospitales_creados[hospitales->valor_actual]
			       ->pokemones[i]
			       ->nombre);
	}

	return EXITO;
}

int hospitales_activos(void *contexto)
{
	struct hospitales *hospitales = contexto;

	if (!hospitales)
		return ERROR;

	printf("\n");
	printf("Hospitales Disponibles:\n");
	for (size_t i = 0; i < hospitales->cant_hospitales; i++) {
		printf("Nombre Hospital: %s",
		       hospitales->hospitales_creados[i]->nombre_archivo);
		if (i == hospitales->valor_actual) {
			printf(" (Hospital actualmente activo)");
		}
		printf("\n");
		printf("ID del hospital: %i\n",
		       hospitales->hospitales_creados[i]->numero_id);
		printf("\n");
	}

	return EXITO;
}

int destruir_hospital(void *contexto)
{
	struct hospitales *hospitales = contexto;
	if (!hospitales)
		return ERROR;

	if (hospitales->cant_hospitales == 1) {
		hospital_destruir(hospitales->hospitales_creados[0]);
		hospitales->cant_hospitales--;
	} else if (hospitales->cant_hospitales > 1) {
		hospital_t *hospital_aux =
			hospitales->hospitales_creados
				[hospitales->cant_hospitales - 1];
		hospital_destruir(
			hospitales
				->hospitales_creados[hospitales->valor_actual]);
		hospitales->hospitales_creados[hospitales->valor_actual] =
			hospital_aux;
		hospitales->cant_hospitales--;
	}

	bool id_encontrado = false;
	int valor_hospital;
	size_t nueva_posicion;

	if (hospitales->cant_hospitales > 0) {
		while (id_encontrado == false) {
			printf("Ingrese un ID de hospital que quiere activar: ");
			scanf(" %i", &valor_hospital);
			for (size_t i = 0; i < hospitales->cant_hospitales;
			     i++) {
				if (hospitales->hospitales_creados[i]
					    ->numero_id == valor_hospital) {
					id_encontrado = true;
					nueva_posicion = i;
				}
			}

			if (id_encontrado == false) {
				printf("Ingrese un ID de un hospital valido\n");
			}
		}
		printf("El hospital con ID %i esta activo!\n", valor_hospital);
		hospitales->valor_actual = nueva_posicion;
	}

	return EXITO;
}

int activar_hospital(void *contexto)
{
	struct hospitales *hospitales = contexto;
	if (!hospitales)
		return ERROR;

	bool id_encontrado = false;
	int valor_hospital;
	size_t nueva_posicion;

	while (id_encontrado == false) {
		printf("Ingrese un ID de hospital que quiere activar: ");
		scanf(" %i", &valor_hospital);
		for (size_t i = 0; i < hospitales->cant_hospitales; i++) {
			if (hospitales->hospitales_creados[i]->numero_id ==
			    valor_hospital) {
				id_encontrado = true;
				nueva_posicion = i;
			}
		}

		if (id_encontrado == false) {
			printf("Ingrese un ID de un hospital valido\n");
		}
	}

	printf("El hospital con ID %i esta activo!\n", valor_hospital);
	hospitales->valor_actual = nueva_posicion;

	return EXITO;
}

int menu_liberar_memoria(void *contexto)
{
	menu_t *menu = contexto;

	if (!menu)
		return ERROR;

	if (menu->hospital_general->cant_hospitales > 0) {
		for (size_t i = 0; i < menu->hospital_general->cant_hospitales;
		     i++) {
			if (menu->hospital_general->hospitales_creados[i] !=
			    NULL) {
				hospital_destruir(
					menu->hospital_general
						->hospitales_creados[i]);
			}
		}
	}

	free(menu->hospital_general->hospitales_creados);
	free(menu->hospital_general);
	hash_destruir(menu->comandos);
	free(menu);

	return EXITO;
}

bool ejecutar_opcion(menu_t *menu, char desicion_usuario)
{
	if (!menu)
		return true;

	system("clear");

	if (desicion_usuario == 's' || desicion_usuario == 'S') {
		menu->programa_finalizado = true;

	} else if (desicion_usuario == 'h' || desicion_usuario == 'H') {
		printf("\n");
		struct nodo *comando = hash_obtener(menu->comandos, "ayuda");
		comando->funcion(menu->hospital_general);

	} else if (desicion_usuario == 'c' || desicion_usuario == 'C') {
		struct nodo *comando = hash_obtener(menu->comandos, "crear");
		int resultado_crear = comando->funcion(menu->hospital_general);

		if (resultado_crear == ERROR) {
			printf("Hubo un error al cargar el archivo, asegurese que sea valido\n");
		} else if (resultado_crear == EXITO) {
			menu->hospital_general->asignar_id_hospital += 1;
		}
	} else if (desicion_usuario == 'e' || desicion_usuario == 'E') {
		printf("\n");
		if (menu->hospital_general->cant_hospitales == 0) {
			printf("No hay ningun hospital cargado!\n");
		} else if (menu->hospital_general->cant_hospitales > 0) {
			struct nodo *comando =
				hash_obtener(menu->comandos, "estado");
			comando->funcion(menu->hospital_general);
		}
	} else if (desicion_usuario == 'a' || desicion_usuario == 'A') {
		printf("\n");
		if (menu->hospital_general->cant_hospitales > 0) {
			struct nodo *comando =
				hash_obtener(menu->comandos, "activar");
			comando->funcion(menu->hospital_general);
		} else if (menu->hospital_general->cant_hospitales == 0) {
			printf("No hay ningun hospital disponible para activar!\n");
		}

	} else if (desicion_usuario == 'm' || desicion_usuario == 'M') {
		if (menu->hospital_general->cant_hospitales > 0) {
			struct nodo *comando =
				hash_obtener(menu->comandos, "mostrar");
			comando->funcion(menu->hospital_general);
		} else if (menu->hospital_general->cant_hospitales == 0) {
			printf("\n");
			printf("No hay ningun hospital disponible para mostrar!\n");
		}
	} else if (desicion_usuario == 'l' || desicion_usuario == 'L') {
		if (menu->hospital_general->cant_hospitales > 0) {
			struct nodo *comando =
				hash_obtener(menu->comandos, "listar");
			comando->funcion(menu->hospital_general);
		} else if (menu->hospital_general->cant_hospitales == 0) {
			printf("\n");
			printf("No hay ningun hospital disponible para mostrar!\n");
		}
	} else if (desicion_usuario == 'd' || desicion_usuario == 'D') {
		if (menu->hospital_general->cant_hospitales > 0) {
			struct nodo *comando =
				hash_obtener(menu->comandos, "destruir");
			comando->funcion(menu->hospital_general);
		} else if (menu->hospital_general->cant_hospitales == 0) {
			printf("No hay ningun hospital disponible para destruir!");
		}
		printf("\n");
	}

	return menu->programa_finalizado;
}

hash_t *insertar_hash(hash_t *comandos)
{
	if (!comandos)
		return NULL;

	hash_insertar(comandos, "ayuda", NULL, NULL, ayuda_usuario);
	hash_insertar(comandos, "estado", NULL, NULL, hospitales_activos);
	hash_insertar(comandos, "activar", NULL, NULL, activar_hospital);
	hash_insertar(comandos, "mostrar", NULL, NULL, pokemones_en_hospital);
	hash_insertar(comandos, "listar", NULL, NULL, lista_pokemones);
	hash_insertar(comandos, "destruir", NULL, NULL, destruir_hospital);
	hash_insertar(comandos, "menu", NULL, NULL, menu_usuario);
	hash_insertar(comandos, "memoria", NULL, NULL, menu_liberar_memoria);
	hash_insertar(comandos, "crear", NULL, NULL, crear_hospital);

	return comandos;
}
