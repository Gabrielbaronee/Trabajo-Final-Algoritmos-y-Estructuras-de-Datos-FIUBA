#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hash.h"

#define FACTOR_CARGA_MAXIMO 0.7
#define CAPACIDAD_MINIMA 3

struct nodo {
	char *clave;
	void *valor;
	int (*funcion)(void *);
	struct nodo *siguiente;

} nodo_t;

struct hash {
	struct nodo **vector;
	size_t capacidad;
	size_t cantidad;
};

unsigned long funcion_hash(const char *str)
{
	unsigned long hash = 5381;
	int c;

	while ((c = *(str++))) {
		hash = ((hash << 5) + hash) + (unsigned)c;
	}

	return hash;
}

hash_t *rehash(hash_t *hash)
{
	if (hash == NULL)
		return NULL;

	hash_t *hash_nuevo = hash_crear((hash->capacidad) * 2);
	if (hash_nuevo == NULL)
		return NULL;

	for (size_t posicion = 0; posicion < hash->capacidad; posicion++) {
		struct nodo *nodo_actual = hash->vector[posicion];

		while (nodo_actual != NULL) {
			hash_insertar(hash_nuevo, nodo_actual->clave,
				      nodo_actual->valor, NULL,
				      nodo_actual->funcion);
			nodo_actual = nodo_actual->siguiente;
		}
	}

	hash_t aux = *hash;
	*hash = *hash_nuevo;
	*hash_nuevo = aux;

	hash_destruir(hash_nuevo);

	return hash;
}

hash_t *hash_crear(size_t capacidad)
{
	hash_t *hash = malloc(sizeof(hash_t));

	if (hash == NULL)
		return NULL;

	if (capacidad < CAPACIDAD_MINIMA)
		capacidad = CAPACIDAD_MINIMA;

	hash->cantidad = 0;
	hash->capacidad = capacidad;
	hash->vector = calloc(1, sizeof(struct nodo *) * hash->capacidad);
	if (hash->vector == NULL) {
		free(hash);
		return NULL;
	}

	return hash;
}

hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento,
		      void **anterior, int (*funcion)(void *))
{
	if (!hash || !clave)
		return NULL;

	float carga = (float)(hash->cantidad) / (float)(hash->capacidad);

	if (carga >= FACTOR_CARGA_MAXIMO) {
		hash = rehash(hash);
		if (hash == NULL)
			return NULL;
	}

	long unsigned int posicion = funcion_hash(clave) % (hash->capacidad);

	if (hash->vector[posicion] == NULL) {
		struct nodo *nuevo_nodo = malloc(sizeof(nodo_t));
		if (nuevo_nodo == NULL)
			return NULL;

		nuevo_nodo->clave = malloc(strlen(clave) * 2);
		if (nuevo_nodo->clave == NULL) {
			free(nuevo_nodo);
			return NULL;
		}
		strcpy(nuevo_nodo->clave, clave);
		nuevo_nodo->valor = elemento;
		nuevo_nodo->siguiente = NULL;
		nuevo_nodo->funcion = funcion;

		if (anterior != NULL)
			*anterior = NULL;

		hash->vector[posicion] = nuevo_nodo;
		hash->cantidad++;
		return hash;
	}

	struct nodo *actual = hash->vector[posicion];
	while (actual != NULL) {
		if (strcmp(actual->clave, clave) == 0) {
			if (anterior != NULL)
				*anterior = actual->valor;
			actual->valor = elemento;
			return hash;
		}

		actual = actual->siguiente;
	}

	struct nodo *nuevo_nodo = malloc(sizeof(nodo_t));
	if (nuevo_nodo == NULL)
		return NULL;

	nuevo_nodo->clave = malloc(strlen(clave) * 2);
	if (nuevo_nodo->clave == NULL) {
		free(nuevo_nodo);
		return NULL;
	}
	strcpy(nuevo_nodo->clave, clave);
	nuevo_nodo->valor = elemento;
	nuevo_nodo->funcion = funcion;
	nuevo_nodo->siguiente = hash->vector[posicion];
	hash->vector[posicion] = nuevo_nodo;
	if (anterior != NULL)
		*anterior = NULL;
	hash->cantidad++;

	return hash;
}

void *hash_quitar(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;

	long unsigned int posicion = funcion_hash(clave) % (hash->capacidad);

	struct nodo *nodo_actual = hash->vector[posicion];
	if (nodo_actual == NULL)
		return NULL;

	if (strcmp(nodo_actual->clave, clave) == 0) {
		void *elemento_a_devolver = nodo_actual->valor;
		hash->vector[posicion] = nodo_actual->siguiente;
		free(nodo_actual->clave);
		free(nodo_actual);
		hash->cantidad--;
		return elemento_a_devolver;
	}

	while (nodo_actual != NULL) {
		if (strcmp((nodo_actual->siguiente)->clave, clave) == 0) {
			struct nodo *nodo_aux = nodo_actual->siguiente;
			void *elemento_a_devolver = nodo_aux->valor;
			nodo_actual->siguiente = nodo_aux->siguiente;
			free(nodo_aux->clave);
			free(nodo_aux);
			hash->cantidad--;
			return elemento_a_devolver;
		}

		nodo_actual = nodo_actual->siguiente;
	}

	return NULL;
}

struct nodo *hash_obtener(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;

	long unsigned int posicion = funcion_hash(clave) % hash->capacidad;
	struct nodo *nodo_actual = hash->vector[posicion];

	while (nodo_actual != NULL) {
		if (strcmp(nodo_actual->clave, clave) == 0) {
			return nodo_actual;
		}
		nodo_actual = nodo_actual->siguiente;
	}

	return NULL;
}

bool hash_contiene(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return false;

	long unsigned int posicion = funcion_hash(clave) % hash->capacidad;
	struct nodo *nodo_actual = hash->vector[posicion];

	while (nodo_actual != NULL) {
		if (strcmp(nodo_actual->clave, clave) == 0) {
			return true;
		}

		nodo_actual = nodo_actual->siguiente;
	}

	return false;
}

size_t hash_cantidad(hash_t *hash)
{
	if (!hash)
		return 0;

	return hash->cantidad;
}

void hash_destruir(hash_t *hash)
{
	if (!hash)
		return;

	hash_destruir_todo(hash, NULL);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (!hash)
		return;

	struct nodo *vector[hash->capacidad];
	int j = 0;
	for (size_t i = 0; i < hash->capacidad; i++) {
		struct nodo *nodo_actual = hash->vector[i];
		while (nodo_actual != NULL) {
			vector[j] = nodo_actual;
			if (destructor != NULL)
				destructor(nodo_actual->valor);

			j++;
			nodo_actual = nodo_actual->siguiente;
		}
	}

	for (int i = 0; i < j; i++) {
		free(vector[i]->clave);
		free(vector[i]);
	}

	free(hash->vector);
	free(hash);
}

size_t hash_con_cada_clave(hash_t *hash,
			   bool (*f)(const char *clave, void *valor, void *aux),
			   void *aux)
{
	size_t n = 0;

	if (!hash || !f)
		return n;

	bool continua_iterando = true;

	for (size_t posicion = 0; posicion < hash->capacidad; posicion++) {
		struct nodo *nodo_actual = hash->vector[posicion];

		while (nodo_actual != NULL) {
			continua_iterando =
				f(nodo_actual->clave, nodo_actual->valor, aux);
			n++;
			if (continua_iterando == false)
				return n;

			nodo_actual = nodo_actual->siguiente;
		}
	}

	return n;
}
