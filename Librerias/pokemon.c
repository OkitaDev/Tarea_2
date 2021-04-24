#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "TDA_Mapa/hashmap.h"
#include "Interfaz/interfaz.h"

typedef struct tipoNumericos
{
  int id;
  int idPokedex;
  int ocurrencia;
  int cantidadTipos;
}tipoNumericos;

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
    char tipos[25][50];
    tipoPuntos puntos;
    tipoEvoluciones evol;
    tipoNumericos ident;
}tipoPokemon;

/*Funcion para indicar si hay mas Pokemons de la misma especie
y verificar si poseen el mismo id de la Pokedex*/
short elPokemonYaExiste(HashMap * mapa, tipoPokemon * pokemon)
{
  tipoPokemon * aux = searchMap(mapa, pokemon->nombrePokemon);
  if(aux != NULL)
  {
    if(pokemon->ident.idPokedex == aux->ident.idPokedex)
    {
      aux->ident.ocurrencia += 1;
      pokemon->ident.ocurrencia += aux->ident.ocurrencia;
    }
    else
    {
      printf("\nEl numero de la Pokedex es erroneo\n");
      printf("El pokemon %s con ID %i no sera ingresado\n", pokemon->nombrePokemon, pokemon->ident.id);
      return 1;
    }
  }
  else
  {
    pokemon->ident.ocurrencia = 1;
  }

  return 0;
}

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
        strcpy(nuevoPokemon->tipos[0], fragmento);
        short largo;
        short cantidad = 1;

        //Reiteracion hasta que no hayan mas tipos
        do
        {
            fragmento = strtok(NULL, ",");
            largo = strlen(fragmento) - 1;
            if(fragmento[largo] != '"') strcat(nuevoPokemon->tipos[cantidad], fragmento);
            cantidad++;
        } while (fragmento[largo] != '"');

        //Eliminacion de la comillas finales
        fragmento[largo] = '\0';
        strcat(nuevoPokemon->tipos[cantidad - 1], fragmento);
        nuevoPokemon->ident.cantidadTipos = cantidad;
    }
    else
    {
        strcpy(nuevoPokemon->tipos[0], fragmento);
        nuevoPokemon->ident.cantidadTipos = 1;
    }

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

    //Aumentar la cantidad de veces que posee un Pokemon y si el nro de Pokedex del Pokemon es el mismo
    if(elPokemonYaExiste(mapa, nuevoPokemon) == 1) return;

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
        printf("\nArchivo NO IMPORTADO!\n");
        return NULL;
    }

    char lineaLeida[101];
    fgets(lineaLeida, 100, archivo);

    //Lectura del archivo (saltandose la primera linea)
    while(fgets(lineaLeida, 100, archivo))
    {
        if(size(mapa) >= 100)
        {
            printf("\nHa superado el maximo de Pokemons\n");
            printf("Se ha podido algunos implementar pokemons!\n");
            return mapa;
        }

        //If para aumentar el tamaño del mapa
        if(size(mapa) == round(capacity(mapa) * 0.7)) enlarge(mapa);

        ingresarPokemon(mapa, lineaLeida);
    }

    printf("\nArchivo IMPORTADO!\n");
    fclose(archivo);
    return mapa;
}

void exportarArchivo(HashMap * mapa)
{
    if(size(mapa) == 0)
    {
      printf("\nNo ha atrapado Pokemons\n");
      return;
    }
    char nombreArchivo[40];

    //Lectura del nombre del archivo
    printf("\nIngrese el nombre del nuevo archivo (Incluyendo el .csv)\n\n");
    fflush(stdin);
    scanf("%39[^\n]s", nombreArchivo);

    //Acceso al archivo
    FILE * archivo = fopen(nombreArchivo, "w");
    if(archivo == NULL)
    {
        printf("\nArchivo NO EXPORTADO!\n");
        return;
    }

    //Lectura de la primera linea
    fprintf(archivo, "id,nombre,tipos,pc,ps,sexo,evolucion previa,evolucion posterior,numero pokedex,region\n");

    //Lectura de los Pokemons almacenados
    tipoPokemon * auxPokemon = firstMap(mapa);

    while(auxPokemon != NULL)
    {
      fprintf(archivo, "%i,%s,", auxPokemon->ident.id, auxPokemon->nombrePokemon, 34);
      if(auxPokemon->ident.cantidadTipos != 1) fprintf(archivo, "%c", 34);
      for(int i = 0; i < auxPokemon->ident.cantidadTipos; i++)
      {
        fprintf(archivo, "%s",auxPokemon->tipos[i]);
        if(i < auxPokemon->ident.cantidadTipos - 1)
        {
          fprintf(archivo, "%c", 44);
        }
      }
      if(auxPokemon->ident.cantidadTipos != 1) fprintf(archivo, "%c", 34);
      fprintf(archivo, ",%i,%i,%s",auxPokemon->puntos.pCombate ,auxPokemon->puntos.pSalud, auxPokemon->sexo);
      fprintf(archivo, ",%s,%s",auxPokemon->evol.evolPrevia, auxPokemon->evol.evolSiguiente);
      fprintf(archivo, ",%i,%s", auxPokemon->ident.idPokedex, auxPokemon->region);
      auxPokemon = nextMap(mapa);
    }

    printf("\nArchivo EXPORTADO!\n");
    fclose(archivo);
}

void atraparPokemon(HashMap * mapa)
{
    if(size(mapa) > 100)
    {
        printf("\nHa superado el maximo de Pokemons\n");
        printf("Se ha podido algunos implementar pokemons!\n");
        return;
    }

    //Creacion de variable del pokemon atrapado
    tipoPokemon * pokemonAtrapado = malloc(sizeof(tipoPokemon));
    char tipo[50];
    int i;

    //Ingreso del ID
    pokemonAtrapado->ident.id = size(mapa) + 1;

    //Ingreso del nombre
    printf("\nIngrese el nombre del Pokemon: ");
    fflush(stdin);
    scanf("%25[^\n]s", pokemonAtrapado->nombrePokemon);

    //Ingreso del numero de la Pokedex
    printf("\nIngrese el numero de ubicacion de la Pokedex: ");
    fflush(stdin);
    scanf("%i", &pokemonAtrapado->ident.idPokedex);

    //Revision de Ocurrencia
    if(elPokemonYaExiste(mapa, pokemonAtrapado) == 1) return;

    //Ingreso de los tipos
    for(i = 0; i < 25; i++)
    {
      printf("\nIngrese el tipo %i del Pokemon (Ingrese no para parar): ", i + 1);
      fflush(stdin);
      scanf("%49s", tipo);
      if(strcmp("No", convertirEstandar(tipo)) == 0) break;
      strcat(tipo, " ");
      strcpy(pokemonAtrapado->tipos[i], tipo);
    }

    pokemonAtrapado->ident.cantidadTipos = i;
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


    //Ingreso de la region del Pokemon
    printf("\nIngrese la region originaria del Pokemon: ");
    fflush(stdin);
    scanf("%25[^\n]s", pokemonAtrapado->region);
    strcat(pokemonAtrapado->region, "\n");
    //Ingreso al mapa
    insertMap(mapa, pokemonAtrapado->nombrePokemon, pokemonAtrapado);
}

void evolucionarPokemon(HashMap * mapa)
{

}

void busquedaPorTipo(HashMap * mapa)
{

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
            printf("Tipo(s): ");
            for(int i = 0; i < pokemonAuxiliar->ident.cantidadTipos; i++)
              printf("%s", pokemonAuxiliar->tipos[i]);

            printf("\nID: %i Pokedex: %i\n", pokemonAuxiliar->ident.id, pokemonAuxiliar->ident.idPokedex);
            printf("PS: %i PC: %i\n", pokemonAuxiliar->puntos.pSalud, pokemonAuxiliar->puntos.pCombate);
            existePokemon = 1;
        }
        pokemonAuxiliar = nextMap(mapa);
    }

    //Si no se encuentra
    if(existePokemon == 0) printf("\nNo ha atrapado %s\n", nombreBuscado);

}

void buscarpokemonpornombrepokedex(HashMap * mapa)
{
    char nombrebuscado[25];
    tipoPokemon * pokemon;

    //Ingreso nombre de pokemón
    printf("\nIngrese el pokemon a buscar: \n\n");
    fflush(stdin);
    scanf("%25[^\n]s", nombrebuscado);
    nombrebuscado[0]=(toupper(nombrebuscado[0]));

    //Se busca el pokemon y se ingresa en el auxiliar
    pokemon=searchMap(mapa,nombrebuscado);

    //De no ser el auxiliar NULL, se imprimen sus datos por pantalla
    if(pokemon!=NULL)
    {
        printf("\n%s Sexo: %s\n", pokemon->nombrePokemon, pokemon->sexo);
        printf("Tipo(s): %s\n", pokemon->tipos);
        printf("ID: %i Pokedex: %i\n", pokemon->ident.id, pokemon->ident.idPokedex);
    }else
    {
        printf("No se encuentra el pokemon ingresado.\n");
    }
 }

void mostrarPokedex(HashMap * mapa)
{

}

void mostrarPokemonsOrdenadosPC(HashMap * mapa)
{

}

void liberarPokemon(HashMap * mapa)
{

 }

void mostrarPokemonRegion(HashMap * mapa)
{

}
