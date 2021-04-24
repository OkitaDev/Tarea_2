#ifndef POKEMON_H
#define POKEMON_H

typedef struct tipoPokemon tipoPokemon;

//Funcion para importar un archivo.csv
HashMap * importarArchivo(HashMap *);

//Funcion para exportar un archivo.csv
void exportarArchivo(HashMap *);

//Funcion para ingresar un pokemon al PC
void atraparPokemon(HashMap *);

/*Funcion para evolucionar a 1 pokemon, buscandolo
a través de su ID*/
void evolucionarPokemon(HashMap *);

//Buscar todos los Pokemons que posean el mismo Tipo
void busquedaPorTipo(HashMap *);

//Funcion para buscar por Nombre a 1 o + Pokemons
void buscarPokemonNombre(HashMap *);

//Funcion para buscar por Nombre a la especie de 1 Pokemon
void buscarpokemonpornombrepokedex(HashMap *);

//Funcion para mostrar toda la informacion que se posea en la Pokedex
void mostrarPokedex(HashMap *);

/*Funcion para mostrar a todos los Pokemons ordenados del que
posee mayor PC al que tiene menor PC*/
void mostrarPokemonsOrdenadosPC(HashMap *);

//Funcion para liberar 1 pokemon a través de su ID
void liberarPokemon(HashMap *);

//Mostrar todos los pokemons de una region
void mostrarPokemonRegion(HashMap *);

#endif /* pokemon.h */
