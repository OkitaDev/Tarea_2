#include <stdio.h>
#include <stdlib.h>

#include "Librerias/Interfaz/interfaz.h"
#include "Librerias/TDA_Mapa/hashmap.h"
#include "Librerias/pokemon.h"

int main()
{
    short opcion;

    //Inicializacion de Mapas
    HashMap * mapaID = createMap(10);
    HashMap * mapaPokedex = createMap(10);

    do
    {
        mostrarMenuPokedex();
        opcion = ingresarOpcion(0); //Ingreso de la opcion
        if(opcion == 0) break;

        switch(opcion)
        {
            case 1:
                mostrarSeleccion();
                opcion = ingresarOpcion(1);
                if(opcion == 1) importarArchivo(mapaID, mapaPokedex);
                else if(opcion == 2) exportarArchivo(mapaID);
                else printf(blue"\nNo hizo nada\n"reset);
                break;
            case 2:
                if(size(mapaID) < 100) registrarPokemon(mapaID, mapaPokedex);
                else printf(red"\nHa superado el maximo de Pokemons!\n"reset);
                break;
            case 3:
                if(size(mapaID) != 0) evolucionarPokemon(mapaID, mapaPokedex);
                else printf(red"\nNo ha atrapado a ningun Pokemon\n"reset);
                break;
            case 4:
                if(size(mapaID) != 0) busquedaPorTipo(mapaID);
                else printf(red"\nNo ha atrapado a ningun Pokemon\n"reset);
                break;
            case 5:
                if(size(mapaID) != 0) buscarPokemonNombre(mapaID);
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
                if(size(mapaID) != 0) mostrarPokemonsOrdenadosPC(mapaID);
                else printf(red"\nNo ha atrapado a ningun Pokemon\n"reset);               
                break;
            case 9:
                if(size(mapaID) != 0) liberarPokemon(mapaID, mapaPokedex);
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
    free(mapaID);
    free(mapaPokedex);

    return 0;
}
