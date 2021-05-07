#include <stdio.h>
#include <stdlib.h>

#include "Librerias/Interfaz/interfaz.h"
#include "Librerias/TDA_Mapa/hashmap.h"
#include "Librerias/pokemon.h"

int main()
{
    short opcion;
    HashMap * mapaNombre = createMap(10);
    HashMap * mapaID = createMap(10);
    HashMap * mapaPokedex = createMap(10);

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
                if(opcion == 1) importarArchivo(mapaNombre, mapaID, mapaPokedex);
                else if(opcion == 2) exportarArchivo(mapaID);
                else printf(blue"\nNo hizo nada\n"reset);
                break;
            case 2:
                if(size(mapaNombre) < 100) registrarPokemon(mapaNombre, mapaID, mapaPokedex);
                else printf(red"\nHa superado el maximo de Pokemons!\n"reset);
                break;
            case 3:
                if(size(mapaID) != 0) evolucionarPokemon(mapaID, mapaPokedex);
                else printf(red"\nNo ha atrapado a ningun Pokemon\n"reset);
                break;
            case 4:
                if(size(mapaID) != 0) busquedaPorTipo(mapaNombre);
                else printf(red"\nNo ha atrapado a ningun Pokemon\n"reset);
                break;
            case 5:
                if(size(mapaNombre) != 0) buscarPokemonNombre(mapaNombre);
                else printf(red"\nNo ha atrapado a ningun Pokemon\n"reset);
                break;
            case 6:
                if(size(mapaPokedex) != 0) buscarPokemonPokedex(mapaPokedex);
                else printf(red"\nNo ha atrapado a ningun Pokemon\n"reset);
                break;
            case 7:
                if(size(mapaPokedex) != 0) mostrarPokedex(mapaPokedex);
                else printf(red"\nNo ha atrapado a ningun Pokemon\n"reset);
                break;
            case 8:
                if(size(mapaNombre) != 0) mostrarPokemonsOrdenadosPC(mapaNombre);
                else printf(red"\nNo ha atrapado a ningun Pokemon\n"reset);               
                break;
            case 9:
                if(size(mapaID) != 0) liberarPokemon(mapaNombre, mapaID, mapaPokedex);
                else printf(red"\nNo posee pokemons para liberar\n"reset);
                break;
            case 10:
                if(size(mapaPokedex) != 0) mostrarPokemonRegion(mapaPokedex);
                else printf(red"\nNo ha atrapado a ningun Pokemon\n"reset);
                break;
        }
        opcion = 1;
    } while (opcion != 0);

    printf(yellow"\n -----\n");
    printf("| FIN |\n");
    printf(" -----\n\n"reset);

    //Liberacion de memoria
    free(mapaNombre);
    free(mapaID);
    free(mapaPokedex);

    return 0;
}
