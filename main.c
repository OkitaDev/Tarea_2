#include <stdio.h>
#include <stdlib.h>

#include "Librerias/Interfaz/interfaz.h"
#include "Librerias/TDA_Mapa/hashmap.h"
#include "Librerias/pokemon.h"

int main()
{
    short opcion;
    HashMap * mapaNombre = createMap(24);
    HashMap * mapaID = createMap(24);

    do
    {
        mostrarMenuPokedex();
        opcion = ingresarOpcion(0);
        if(opcion == 0) break;

        switch(opcion)
        {
            case 1:
                mostrarSeleccion();
                opcion = ingresarOpcion(1);
                if(opcion == 1) importarArchivo(mapaNombre, mapaID);
                else if(opcion == 2) exportarArchivo(mapaID);
                else printf("\nNo hizo nada\n");
                break;
            case 2:
                if(size(mapaNombre) + 1 < 100)atraparPokemon(mapaNombre, mapaID);
                else printf("\nHa superado el maximo de Pokemons!\n");
                break;
            case 3:
                evolucionarPokemon(mapaNombre, mapaID);
                break;
            case 4:
                busquedaPorTipo(mapaNombre);
                break;
            case 5:
                if(size(mapaNombre) != 0) buscarPokemonNombre(mapaNombre);
                else printf("\nNo ha atrapado a ningun Pokemon\n");
                break;
            case 6:
                if(size(mapaNombre) != 0) buscarPokemonPokedex(mapaNombre);
                else printf("\nPokemon no encontrado\n");
                break;
            case 7:
                if(size(mapaNombre) != 0) mostrarPokedex(mapaNombre);
                else printf("\nNo ha atrapado a ningun Pokemon\n");
                break;
            case 8:
                mostrarPokemonsOrdenadosPC(mapaNombre);
                break;
            case 9:
                liberarPokemon(mapaID);
                break;
            case 10:
                mostrarPokemonRegion(mapaNombre);
                break;
        }
        opcion = 1;
        printf("\n");
    } while (opcion != 0);

    printf("\n -----\n");
    printf("| FIN |\n");
    printf(" -----\n\n");

    //Liberacion de memoria
    free(mapaNombre);
    free(mapaID);

    return 0;
}
