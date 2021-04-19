#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "TDA_Mapa/hashmap.h"

typedef struct tipoIdentificacion
{
  int id;
  int idPokedex;
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

typedef struct tipoTipos
{
    char tipo1[25];
    char tipo2[25];
}tipoTipos;

typedef struct tipoPokemon
{
    char nombrePokemon[25];
    char region[25];
    char sexo[10];
    tipoTipos elemento;
    tipoPuntos puntos;
    tipoEvoluciones evol;
    tipoIdentificacion ident;
}tipoPokemon;

void ingresarPokemon(HashMap * mapa, char * lineaLeida)
{
    tipoPokemon * nuevoPokemon = malloc (sizeof(tipoPokemon));
    char * fragmento;
    
    //Lectura del ID
    fragmento = strtok(lineaLeida, ",");
    nuevoPokemon->ident.id = strtol(fragmento, NULL, 10);

    //Lectura del Nombre
    fragmento = strtok(NULL, ",");
    strcpy(nuevoPokemon->nombrePokemon, fragmento);

    //Lectura del Tipo 1
    fragmento = strtok(NULL, ",");
    strcpy(nuevoPokemon->elemento.tipo1, fragmento);
    
    //Eliminacion comillas iniciales
    if(fragmento[0] == '"')
    {
        memmove(nuevoPokemon->elemento.tipo1, nuevoPokemon->elemento.tipo1 + 1, strlen(nuevoPokemon->elemento.tipo1));
    }

    //Lectura del Tipo 2
    fragmento = strtok(NULL, ",");
    short posee2Tipos = 0;

    if(fragmento[strlen(fragmento) - 1] == '"')
    {
        //Eliminacion comillas finales
        short largo = strlen(fragmento);
        fragmento[largo - 1] = '\0';
        
        //ELiminacion espacio inicial
        memmove(nuevoPokemon->elemento.tipo2, nuevoPokemon->elemento.tipo2 + 1, strlen(nuevoPokemon->elemento.tipo1));
        
        strcpy(nuevoPokemon->elemento.tipo2, fragmento);
        posee2Tipos = 1;
    }
    //Lectura de los Puntos de Combate
    if(posee2Tipos != 0) fragmento = strtok(NULL, ",");
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
    }

HashMap * importarArchivo(HashMap * mapa)
{
    //Ingreso del nombre del archivo a implementar
    char nombreArchivo[40];
    printf("\nIngrese el nombre del archivo (Incluyendo el .cvs)\n\n");
    scanf("%39s", nombreArchivo);

    //Busqueda del archivo
    FILE * archivo = fopen(nombreArchivo, "r");

    if(archivo == NULL)
    {
        printf("\nArchivo NO IMPLEMENTADO!\n");
        return NULL;
    }

    //Lectura del archivo (saltandose la primera linea)
    char lineaLeida[100];
    short lecturaPrimeraLinea = 0;

    while(fgets(lineaLeida, 99, archivo))
    {
        if(lecturaPrimeraLinea != 0)
        {
            ingresarPokemon(mapa, lineaLeida);
        }
        lecturaPrimeraLinea++;
    }
    
    printf("\nArchivo IMPORTO!\n");
    fclose(archivo);
    return mapa;
}

HashMap * exportarArchivo(HashMap * mapa)
{
    char nombreArchivo[40];
    printf("\nIngrese el nombre del nuevo archivo (Incluyendo el .cvs)\n\n");
    scanf("%39s", nombreArchivo);

    FILE * archivo = fopen(nombreArchivo, "w");

    if(archivo == NULL)
    {
        printf("\nArchivo NO IMPLEMENTADO!\n");
        return NULL;
    }

    printf("\nArchivo EXPORTADO!\n");
    fclose(archivo);
    return mapa;
}
