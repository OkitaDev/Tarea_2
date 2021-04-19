#ifndef POKEMON_H
#define POKEMON_H

typedef struct tipoIdentificacion tipoIdentificacion;
typedef struct tipoEvoluciones tipoEvoluciones;
typedef struct tipoPuntos tipoPuntos;
typedef struct tipoTipos tipoTipos;
typedef struct tipoPokemon tipoPokemon;

void ingresarPokemon(HashMap *, char *);
HashMap * importarArchivo(HashMap *);
HashMap * exportarArchivo(HashMap *);

#endif /* pokemon.h */