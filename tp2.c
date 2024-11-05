#include <stdio.h>
#include "tda.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hash.h"

#define ERROR -1
#define EXITO 0

int main()
{
	struct hospitales *hospitales = crear_hospital_general();
	if (!hospitales)
		return ERROR;

	hash_t *comandos = hash_crear(10);
	if (!comandos) {
		free(hospitales);
		return ERROR;
	}
	comandos = insertar_hash(comandos);

	menu_t *menu = crear_menu(hospitales);
	if (!menu) {
		free(hospitales);
		free(comandos);
		return ERROR;
	}

	menu->comandos = comandos;

	printf("Bienvenido, ingrese una opcion valida, si no conoce los comandos presione 'H'\n");

	bool salir_programa = false;
	while (salir_programa == false) {
		struct nodo *comando = hash_obtener(menu->comandos, "menu");
		comando->funcion(menu->hospital_general);
		printf("\nIngrese una opcion valida: ");
		scanf(" %c", &menu->desicion_usuario);
		salir_programa = ejecutar_opcion(menu, menu->desicion_usuario);
	}

	struct nodo *comando = hash_obtener(menu->comandos, "memoria");
	comando->funcion(menu);

	return EXITO;
}
