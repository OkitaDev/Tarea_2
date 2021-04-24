#include <stdio.h>

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
                break;
            case 2:
                if(size(mapaNombre) + 1 < 100)atraparPokemon(mapaNombre, mapaID);
                else printf("\nHa superado el maximo de Pokemons!\n");
                break;
            case 3:
                evolucionarPokemon(mapaNombre);
                break;
            case 4:
                busquedaPorTipo(mapaNombre);
                break;
            case 5:
                if(size(mapaNombre) != 0) buscarPokemonNombre(mapaNombre);
                else printf("\nNo ha atrapado a ningun Pokemon\n");
                break;
            case 6:
                if(size(mapaNombre) != 0)buscarpokemonpornombrepokedex(mapaNombre);
                else printf("\nPokemon no encontrado\n");
                break;
            case 7:
                mostrarPokedex(mapaNombre);
                break;
            case 8:
                mostrarPokemonsOrdenadosPC(mapaNombre);
                break;
            case 9:
                liberarPokemon(mapaNombre);
                break;
            case 10:
                mostrarPokemonsOrdenadosPC(mapaNombre);
                break;
        }
        printf("\n");
    } while (opcion != 0);

    printf("\n -----\n");
    printf("| FIN |\n");
    printf(" -----\n\n");

    return 0;
}
