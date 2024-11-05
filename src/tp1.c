#include "tp1.h"
#include "pokemon.h"
#include "pokemon_privado.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

struct _hospital_pkm_t {
	pokemon_t **pokemones;
	size_t cantidad_pokemon;
	size_t cantidad_entrenadores;
	char nombre_archivo[50];
	int numero_id;
};

void ordenar_pokemones(hospital_t *hospital)
{
	for (unsigned i = 0; i < hospital->cantidad_pokemon - 1; i++) {
		for (unsigned j = 0; j < hospital->cantidad_pokemon - 1 - i;
		     j++) {
			if (pokemon_salud(hospital->pokemones[j]) >
			    pokemon_salud(hospital->pokemones[j + 1])) {
				size_t aux =
					pokemon_salud(hospital->pokemones[j]);
				(hospital->pokemones[j])->salud = pokemon_salud(
					hospital->pokemones[j + 1]);
				(hospital->pokemones[j + 1])->salud = aux;

				size_t aux_id =
					pokemon_id(hospital->pokemones[j]);
				(hospital->pokemones[j])->id =
					pokemon_id(hospital->pokemones[j + 1]);
				(hospital->pokemones[j + 1])->id = aux_id;

				char nombre_entrenador_aux[MAX_NOMBRE];
				strcpy(nombre_entrenador_aux,
				       pokemon_entrenador(
					       hospital->pokemones[j]));
				strcpy((hospital->pokemones[j])
					       ->nombre_entrenador,
				       pokemon_entrenador(
					       hospital->pokemones[j + 1]));
				strcpy((hospital->pokemones[j + 1])
					       ->nombre_entrenador,
				       nombre_entrenador_aux);

				char nombre_aux[MAX_NOMBRE];
				strcpy(nombre_aux,
				       pokemon_nombre(hospital->pokemones[j]));
				strcpy((hospital->pokemones[j])->nombre,
				       pokemon_nombre(
					       hospital->pokemones[j + 1]));
				strcpy((hospital->pokemones[j + 1])->nombre,
				       nombre_aux);
			}
		}
	}
}

hospital_t *hospital_crear_desde_archivo(const char *nombre_archivo)
{
	if (nombre_archivo == NULL)
		return NULL;

	FILE *leer_pokemones = fopen(nombre_archivo, "r");

	if (leer_pokemones == NULL)
		return NULL;

	struct _hospital_pkm_t *hospital = malloc(sizeof(hospital_t));

	if (hospital == NULL) {
		fclose(leer_pokemones);
		return NULL;
	}

	hospital->pokemones = malloc(sizeof(pokemon_t *));

	if (hospital->pokemones == NULL) {
		free(hospital);
		fclose(leer_pokemones);
		return NULL;
	}

	int pokemones_creados = 0;
	hospital->cantidad_entrenadores = 0;
	hospital->cantidad_pokemon = 0;

	size_t id, salud;
	char nombre[MAX_NOMBRE];
	char nombre_entrenador[MAX_NOMBRE];
	int i = 0;
	int leidos = fscanf(leer_pokemones, "%zu,%[^,],%zu,%[^\n]\n", &id,
			    nombre, &salud, nombre_entrenador);

	if (leidos != 4) {
		free(hospital->pokemones);
		free(hospital);
		fclose(leer_pokemones);
		return NULL;
	}

	char linea[400];
	snprintf(linea, sizeof(linea), "%zu,%s,%zu,%s", id, nombre, salud,
		 nombre_entrenador);

	while (leidos == 4) {
		snprintf(linea, sizeof(linea), "%zu,%s,%zu,%s", id, nombre,
			 salud, nombre_entrenador);
		hospital->pokemones[i] = pokemon_crear_desde_string(linea);
		pokemones_creados++;
		hospital->cantidad_entrenadores++;
		hospital->cantidad_pokemon++;
		i++;
		struct _pkm_t **pokemon_2 =
			realloc(hospital->pokemones,
				(((size_t)i + 1) * sizeof(pokemon_t *)));
		if (pokemon_2 == NULL) {
			free(hospital->pokemones);
			free(hospital);
			fclose(leer_pokemones);
			return NULL;
		}
		hospital->pokemones = pokemon_2;
		leidos = fscanf(leer_pokemones, "%zu,%[^,],%zu,%[^\n]\n", &id,
				nombre, &salud, nombre_entrenador);
	}

	ordenar_pokemones(hospital);
	fclose(leer_pokemones);

	return hospital;
}

size_t hospital_cantidad_pokemones(hospital_t *hospital)
{
	if (hospital == NULL)
		return 0;

	return hospital->cantidad_pokemon;
}

size_t hospital_a_cada_pokemon(hospital_t *hospital,
			       bool (*funcion)(pokemon_t *p, void *aux),
			       void *aux)
{
	size_t invocaciones = 0;
	if (hospital == NULL || funcion == NULL)
		return invocaciones;

	bool estado_funcion = true;
	int i = 0;
	while (invocaciones < hospital->cantidad_pokemon) {
		estado_funcion = funcion((hospital)->pokemones[i], aux);
		i++;
		invocaciones++;
		if (estado_funcion == false)
			return invocaciones;
	}

	return invocaciones;
}

int hospital_aceptar_emergencias(hospital_t *hospital,
				 pokemon_t **pokemones_ambulancia,
				 size_t cant_pokes_ambulancia)

{
	if (hospital == NULL || pokemones_ambulancia == NULL)
		return ERROR;

	size_t cant_aux = hospital->cantidad_pokemon;
	hospital->cantidad_pokemon += cant_pokes_ambulancia;

	struct _pkm_t **hospital_aux =
		realloc(hospital->pokemones,
			hospital->cantidad_pokemon * sizeof(pokemon_t *));

	if (hospital_aux == NULL)
		return ERROR;

	hospital->pokemones = hospital_aux;
	size_t j = 0;

	for (size_t i = cant_aux; i < hospital->cantidad_pokemon; i++) {
		hospital->pokemones[i] = pokemones_ambulancia[j];

		hospital->cantidad_entrenadores++;
		j++;
	}

	for (size_t i = 0; i < hospital->cantidad_pokemon - 1; i++) {
		for (size_t j = 0; j < hospital->cantidad_pokemon - 1 - i;
		     j++) {
			if (pokemon_salud(hospital->pokemones[j]) >
			    pokemon_salud(hospital->pokemones[j + 1])) {
				struct _pkm_t *poke_aux =
					hospital->pokemones[j];
				hospital->pokemones[j] =
					hospital->pokemones[j + 1];
				hospital->pokemones[j + 1] = poke_aux;
			}
		}
	}

	return EXITO;
}

pokemon_t *hospital_obtener_pokemon(hospital_t *hospital, size_t prioridad)
{
	if (hospital == NULL || prioridad >= hospital->cantidad_pokemon)
		return NULL;

	return hospital->pokemones[prioridad];
}

void hospital_destruir(hospital_t *hospital)
{
	if (hospital != NULL) {
		for (size_t i = 0; i < hospital->cantidad_pokemon; i++)
			pokemon_destruir(hospital->pokemones[i]);

		free(hospital->pokemones);
		free(hospital);
	}
}
