#ifndef POKEMON_H
#define POKEMON_H

typedef struct tipoPokemon tipoPokemon;

HashMap * importarArchivo(HashMap *);
HashMap * exportarArchivo(HashMap *);
void atraparPokemon(HashMap *);
void buscarPokemonNombre(HashMap *);
void buscarpokemonpornombrepokedex(HashMap *);

#endif /* pokemon.h */