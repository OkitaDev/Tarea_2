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

typedef struct tipoPokemon
{
    char nombrePokemon[25];
    char tipos[100];
    char region[25];
    tipoPuntos puntos;
    tipoEvoluciones evol;
    tipoIdentificacion ident;
}tipoPokemon;

void ingresarPokemon(HashMap * mapa, char * lineaLeida)
{
    tipoPokemon * nuevoPokemon = malloc (sizeof(tipoPokemon *));
    char * fragmento;

    fragmento = strtok(lineaLeida, ",");
    printf("%s\n", fragmento);
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
