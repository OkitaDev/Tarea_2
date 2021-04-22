#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"

typedef struct Pair Pair;
typedef struct HashMap HashMap;
int enlarge_called=0;

struct Pair
{
    char * key;
    void * value;
};

struct HashMap
{
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair(char * key,  void * value)
{
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity)
{
    unsigned long hash = 0;
    char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++)
	{
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2)
{
    if(key1 == NULL || key2 == NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}

void insertMap(HashMap * map, char * key, void * value)
{
	long indice = hash(key, map->capacity);

	while(map->buckets[indice] != NULL && map->buckets[indice]->key != 0)
	{
		indice++;
		indice %= map->capacity;
	}

	map->buckets[indice] = createPair(key, value);
	map->size++;
	map->current = indice;
}

void enlarge(HashMap * map)
{
	enlarge_called = 1; //no borrar (testing purposes)
	Pair ** oldBuckets = map->buckets;

	map->capacity *= 2;
	map->buckets = (Pair **) calloc (map->capacity, sizeof(Pair *));
	map->size = 0;

	for(long i = 0; i < map->capacity/2; i++)
	{
		if(oldBuckets[i] != NULL)
			insertMap(map, oldBuckets[i]->key, oldBuckets[i]->value);
	}

}

HashMap * createMap(long capacity)
{
	HashMap * map = (HashMap *) malloc (sizeof(HashMap));
	if(map == NULL) return NULL;

	//Inicializacion de variables
	map->buckets = (Pair **) calloc (capacity, sizeof(Pair*));
	map->capacity = capacity;
	map->size = 0;
	map->current = -1;

	return map;
}

void eraseMap(HashMap * map,  char * key)
{
	long indice = hash(key, map->capacity);

	while(map->buckets[indice] != NULL)
	{
		if(is_equal(key, map->buckets[indice]->key))
		{
			map->buckets[indice]->key = NULL;
			map->size--;
			return;
		}
		indice++;
		indice %= map->capacity;
	}
}

void * searchMap(HashMap * map,  char * key) {
	long indice = hash(key, map->capacity);

	while(map->buckets[indice] != NULL)
	{
		if(is_equal(map->buckets[indice]->key, key))
		{
			map->current = indice;
			return map->buckets[indice]->value;
		}
		indice++;
		indice %= map->capacity;
	}

  return NULL;
}

void * firstMap(HashMap * map)
{
	long indice = map->current + 1;

	while(map->buckets[indice] == NULL)
	{
		indice++;
		indice %= map->capacity;
	}

	if(map->buckets[0] != NULL) indice = 0;

	map->current = indice;
	return map->buckets[indice]->value;
}

void * nextMap(HashMap * map)
{
	for(int i = map->current + 1; i < map->capacity; i++)
	{
		if(map->buckets[i] != NULL)
		{
			map->current = i;
			return map->buckets[i]->value;
		}
	}
	return NULL;
}

long size (HashMap * map)
{
	return map->size;
}

long capacity (HashMap * map)
{
	return map->capacity;
}
