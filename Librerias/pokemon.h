#ifndef POKEMON_H
#define POKEMON_H

typedef struct tipoPokemon tipoPokemon;

HashMap * importarArchivo(HashMap *);

HashMap * exportarArchivo(HashMap *);

void buscarPokemonNombre(HashMap *);

#endif /* pokemon.h */
