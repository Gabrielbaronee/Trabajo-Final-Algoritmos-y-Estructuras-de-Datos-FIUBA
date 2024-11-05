#include <stdio.h>
#include "tda.h"
#include "pa2m.h"
#include <stdbool.h>

void pruebas_tp2()
{
	pa2m_afirmar(
		pokemones_en_hospital(NULL) == ERROR,
		"No se pueden ver los nombres de un hospital con una estructura NULL");
	pa2m_afirmar(
		lista_pokemones(NULL) == ERROR,
		"No se pueden ver los datos de un hospital con una estructura NULL");
	pa2m_afirmar(menu_usuario(NULL) == ERROR,
		     "No se pueden acceder al menu con una estructura NULL");
	pa2m_afirmar(crear_hospital(NULL) == ERROR,
		     "No se puede crear un hospital con una estructura NULL");
	pa2m_afirmar(
		hospitales_activos(NULL) == ERROR,
		"No se pueden ver los hospitales activos con una estructura NULL");
	pa2m_afirmar(
		destruir_hospital(NULL) == ERROR,
		"No se puede destruir un hospital con una estructura NULL");
	pa2m_afirmar(activar_hospital(NULL) == ERROR,
		     "No se puede activar un hospital con una estructura NULL");
	pa2m_afirmar(
		menu_liberar_memoria(NULL) == ERROR,
		"No se puede liberar memoria de un menu con una estructura NULL");

	struct hospitales *hospitales = calloc(1, sizeof(struct hospitales));
	hospitales->hospitales_creados = malloc(sizeof(hospital_t));
	pa2m_afirmar(
		crear_menu(NULL) == NULL,
		"No se puede crear un menu con una estructura hospitales NULL");
	menu_t *menu = crear_menu(hospitales);
	pa2m_afirmar(insertar_hash(NULL) == NULL,
		     "No se pueden insertar los comandos con un Hash NULL");
	menu->hospital_general = hospitales;
	pa2m_afirmar(menu != NULL, "Se crea el menu correctamente");
	pa2m_afirmar(
		ejecutar_opcion(NULL, 'c') == true,
		"No se puede ejecutar una opcion el una estructura menu NULL");

	pa2m_afirmar(menu->hospital_general->cant_hospitales == 0,
		     "Al inicio del programa la cantidad de hospitales es 0");

	pa2m_afirmar(menu->hospital_general->asignar_id_hospital == 0,
		     "Al inicio del programa el ID a asignar es igual a 0");

	for (size_t i = 0; i < menu->hospital_general->cant_hospitales; i++)
		hospital_destruir(
			menu->hospital_general->hospitales_creados[i]);

	free(hospitales->hospitales_creados);
	free(menu->hospital_general);
	free(menu);

	return;
}

int main()
{
	pa2m_nuevo_grupo("Pruebas TP2");

	pruebas_tp2();

	return pa2m_mostrar_reporte();
}