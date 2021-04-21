#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "TDA_Mapa/hashmap.h"

typedef struct tipoIdentificacion
{
  int id;
  int idPokedex;
  int ocurrencia;
}tipoIdentificacion;

typedef struct tipoEvoluciones
{
  char evolPrevia[25];
  char evolSiguiente[25];
}tipoEvoluciones;

typedef struct tipoPuntos
{
    int pSalud;
    int pCombate;
}tipoPuntos;

typedef struct tipoPokemon
{
    char nombrePokemon[25];
    char region[25];
    char sexo[25];
    char tipos[100];
    tipoPuntos puntos;
    tipoEvoluciones evol;
    tipoIdentificacion ident;
}tipoPokemon;

void ingresarPokemon(HashMap * mapa, char * lineaLeida)
{
    tipoPokemon * nuevoPokemon = (tipoPokemon *) malloc (sizeof(tipoPokemon));
    char * fragmento;

    //Lectura del ID
    fragmento = strtok(lineaLeida, ",");
    nuevoPokemon->ident.id = strtol(fragmento, NULL, 10);

    //Lectura del nombre
    fragmento = strtok(NULL, ",");
    strcpy(nuevoPokemon->nombrePokemon, fragmento);

    //Lectura de los tipos
    fragmento = strtok(NULL, ",");

    //Se verifica si posee 1 o mas tipos
    if(fragmento[0] == '"')
    {
        //Eliminacion de las primeras comillas
        memmove(fragmento, fragmento + 1, strlen(fragmento));
        strcpy(nuevoPokemon->tipos, fragmento);
        short largo;

        //Reiteracion hasta que no hayan mas tipos
        do
        {
            fragmento = strtok(NULL, ",");
            largo = strlen(fragmento) - 1;
            if(fragmento[largo] != '"') strcat(nuevoPokemon->tipos, fragmento);
        } while (fragmento[largo] != '"');

        //Eliminacion de la comillas finales
        fragmento[largo] = '\0';
        strcat(nuevoPokemon->tipos, fragmento);
    }
    else strcpy(nuevoPokemon->tipos, fragmento);

    //Lectura de los puntos de Combate
    fragmento = strtok(NULL, ",");
    nuevoPokemon->puntos.pCombate = strtol(fragmento, NULL, 10);

    //Lectura de los Puntos de Salud
    fragmento = strtok(NULL, ",");
    nuevoPokemon->puntos.pSalud = strtol(fragmento, NULL, 10);

    //Lectura del sexo del Pokemon
    fragmento = strtok(NULL,",");
    strcpy(nuevoPokemon->sexo, fragmento);

    //Lectura de las evoluciones
    fragmento = strtok(NULL,",");
    strcpy(nuevoPokemon->evol.evolPrevia, fragmento);
    fragmento = strtok(NULL,",");
    strcpy(nuevoPokemon->evol.evolSiguiente, fragmento);

    //Lectura del numero de la Pokedex
    fragmento = strtok(NULL, ",");
    nuevoPokemon->ident.idPokedex = strtol(fragmento, NULL, 10);

    //Lectura de la Region
    fragmento = strtok(NULL,",");
    strcpy(nuevoPokemon->region, fragmento);

	  //Añadir Pokemon al Mapa (Con clave = nombre)
    insertMap(mapa, nuevoPokemon->nombrePokemon, nuevoPokemon);
}

HashMap * importarArchivo(HashMap * mapa)
{
    //Ingreso del nombre del archivo a implementar
    char nombreArchivo[40];
    printf("\nIngrese el nombre del archivo (Incluyendo el .csv)\n\n");
    fflush(stdin);
    scanf("%39[^\n]s", nombreArchivo);

    //Busqueda del archivo
    FILE * archivo = fopen(nombreArchivo, "r");

    if(archivo == NULL)
    {
        printf("\nArchivo NO IMPLEMENTADO!\n");
        return NULL;
    }

    //Lectura del archivo (saltandose la primera linea)
    char lineaLeida[101];
    short lecturaPrimeraLinea = 0;

    while(fgets(lineaLeida, 100, archivo))
    {
        if(size(mapa) >= 100)
        {
          printf("\nHa superado el maximo de Pokemons\n");
          printf("Se han podido implementar %i pokemons!\n", lecturaPrimeraLinea - 1);
          return mapa;
        }

        if(lecturaPrimeraLinea != 0) ingresarPokemon(mapa, lineaLeida);
        lecturaPrimeraLinea++;
    }

    printf("\nArchivo IMPLEMENTADO!\n");
    fclose(archivo);
    return mapa;
}

HashMap * exportarArchivo(HashMap * mapa)
{
    char nombreArchivo[40];

    printf("\nIngrese el nombre del nuevo archivo (Incluyendo el .csv)\n\n");
    fflush(stdin);
    scanf("%39[^\n]s", nombreArchivo);

    FILE * archivo = fopen(nombreArchivo, "w");

    if(archivo == NULL)
    {
        printf("\nArchivo NO EXPORTADO!\n");
        return NULL;
    }

    printf("\nArchivo EXPORTADO!\n");
    fclose(archivo);
    return mapa;
}

void atraparPokemon(HashMap * mapa)
{
    //Creacion de variable del pokemon atrapado
    tipoPokemon * pokemonAtrapado = malloc(sizeof(tipoPokemon));

    //Ingreso del nombre
    printf("\nIngrese el nombre del Pokemon: ");
    fflush(stdin);
    scanf("%25[^\n]s", pokemonAtrapado->nombrePokemon);

    //Ingreso de los tipos
    printf("\nIngrese el tipo(s) del Pokemon: ");
    fflush(stdin);
    scanf("%100[^\n]s", pokemonAtrapado->tipos);

    //Ingreso de PC y PS
    printf("\nIngrese los PS del Pokemon: ");
    fflush(stdin);
    scanf("%i", &pokemonAtrapado->puntos.pSalud);

    printf("\nIngrese los PC del Pokemon: ");
    fflush(stdin);
    scanf("%i", &pokemonAtrapado->puntos.pCombate);

    //Ingreso del sexo del Pokemon
    printf("\nIngrese el sexo del Pokemon: ");
    fflush(stdin);
    scanf("%25[^\n]s", pokemonAtrapado->sexo);

    //Ingreso de las evoluciones
    printf("\nIngrese la evolucion anterior del Pokemon: ");
    fflush(stdin);
    scanf("%25[^\n]s", pokemonAtrapado->evol.evolPrevia);

    printf("\nIngrese la evolucion siguiente del Pokemon: ");
    fflush(stdin);
    scanf("%25[^\n]s", pokemonAtrapado->evol.evolSiguiente);

    //Ingreso del numero de la Pokedex
    printf("\nIngrese el numero de ubicacion de la Pokedex: ");
    fflush(stdin);
    scanf("%i", &pokemonAtrapado->ident.idPokedex);

    //Ingreso de la region del Pokemon
    printf("\nIngrese la region originaria del Pokemon: ");
    fflush(stdin);
    scanf("%25[^\n]s", pokemonAtrapado->region);

    //Ingreso al mapa
    insertMap(mapa, pokemonAtrapado->nombrePokemon, pokemonAtrapado);
}

void buscarPokemonNombre(HashMap * mapa)
{
    char nombreBuscado[25];
    tipoPokemon * pokemonAuxiliar = firstMap(mapa);
    short existePokemon = 0;

    //Ingreso del nombre del Pokemon
    printf("\nIngrese el nombre del Pokemon a buscar: ");
    fflush(stdin);
    scanf("%25[^\n]s", nombreBuscado);

    /*Se recorre el Mapa ya que no se sabe la cantidad de Pokemons
    atrapados con el mismo nombre, en vez de usar la funcion searchMap,
    ya que esta entrega solo uno de los posibles pokemon*/
    while(pokemonAuxiliar != NULL)
    {
        if(strcmp(pokemonAuxiliar->nombrePokemon, nombreBuscado) == 0)
        {
            printf("\n%s Sexo: %s\n", pokemonAuxiliar->nombrePokemon, pokemonAuxiliar->sexo);
            printf("Tipo(s): %s\n", pokemonAuxiliar->tipos);
            printf("ID: %i Pokedex: %i\n", pokemonAuxiliar->ident.id, pokemonAuxiliar->ident.idPokedex);
            printf("PS: %i PC: %i\n", pokemonAuxiliar->puntos.pSalud, pokemonAuxiliar->puntos.pCombate);
            existePokemon = 1;
        }
        pokemonAuxiliar = nextMap(mapa);
    }

    //Si no se encuentra
    if(existePokemon == 0) printf("\nNo ha atrapado %s\n", nombreBuscado);

}
