#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "TDA_Mapa/hashmap.h"

typedef struct tipoPokemon
{
    int id;
    char nombrePokemon[25];
    char tipos[100];
    int pSalud;
    int pCombate;
    char evolPrevia[25];
    char evolSiguiente[25];
    int numeroPokedex;
    char region[25];
}tipoPokemon;

HashMap * importarArchivo(HashMap * mapa)
{
    char nombreArchivo[40];
    printf("\nIngrese el nombre del archivo (Incluyendo el .cvs)\n\n");
    scanf("%39s", nombreArchivo);

    FILE * archivo = fopen(nombreArchivo, "r");

    if(archivo == NULL) 
    {
        printf("\nArchivo NO IMPLEMENTADO!\n");
        return NULL;
    }

    printf("\nArchivo IMPORTO!\n");
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
    return mapa;
}