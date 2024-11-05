#include "pokemon.h"
#include "pokemon_privado.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pokemon_t *pokemon_crear_desde_string(const char *string)
{
	if (string == NULL)
		return NULL;

	struct _pkm_t *pokemon = malloc(sizeof(pokemon_t));
	if (pokemon == NULL)
		return NULL;

	int leidos = sscanf(string, "%zu,%[^,],%zu,%s", &(pokemon)->id,
			    pokemon->nombre, &(pokemon)->salud,
			    pokemon->nombre_entrenador);

	if (leidos != 4) {
		free(pokemon);
		return NULL;
	}

	return pokemon;
}

pokemon_t *pokemon_copiar(pokemon_t *poke)
{
	if (poke == NULL)
		return NULL;

	struct _pkm_t *copia_p = malloc(sizeof(pokemon_t));
	if (copia_p == NULL)
		return NULL;

	copia_p->id = poke->id;
	copia_p->salud = poke->salud;
	strcpy(copia_p->nombre, poke->nombre);
	strcpy(copia_p->nombre_entrenador, poke->nombre_entrenador);

	return copia_p;
}

bool pokemon_son_iguales(pokemon_t *pokemon1, pokemon_t *pokemon2)
{
	if (pokemon1 == NULL || pokemon2 == NULL)
		return false;

	if ((pokemon1->id != pokemon2->id) ||
	    (pokemon1->salud != pokemon2->salud))
		return false;

	if ((strcmp(pokemon1->nombre, pokemon2->nombre) != 0) ||
	    strcmp(pokemon1->nombre_entrenador, pokemon2->nombre_entrenador) !=
		    0)
		return false;

	return true;
}

char *pokemon_nombre(pokemon_t *pokemon)
{
	if (pokemon == NULL)
		return NULL;

	return pokemon->nombre;
}

char *pokemon_entrenador(pokemon_t *pokemon)
{
	if (pokemon == NULL)
		return NULL;

	return pokemon->nombre_entrenador;
}

size_t pokemon_salud(pokemon_t *pokemon)
{
	if (pokemon == NULL)
		return 0;

	return pokemon->salud;
}

size_t pokemon_id(pokemon_t *pokemon)
{
	if (pokemon == NULL)
		return 0;

	return pokemon->id;
}

void pokemon_destruir(pokemon_t *pkm)
{
	free(pkm);
}
