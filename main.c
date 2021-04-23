#include <stdio.h>

#include "Librerias/Interfaz/interfaz.h"
#include "Librerias/TDA_Mapa/hashmap.h"
#include "Librerias/pokemon.h"

int main()
{
    short opcion;
    HashMap * mapa = createMap(24);

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
                if(opcion == 1) importarArchivo(mapa);
                else if(opcion == 2) exportarArchivo(mapa);
                break;
            case 2:
                atraparPokemon(mapa);
                break;
            case 3:
                evolucionarPokemon(mapa);
                break;
            case 4:
                busquedaPorTipo(mapa);
                break;
            case 5:
                if(size(mapa) != 0) buscarPokemonNombre(mapa);
                else printf("\nNo ha atrapado a ningun Pokemon\n");
                break;
            case 6:
                if(size(mapa) != 0)buscarpokemonpornombrepokedex(mapa);
                else printf("\nNo ha atrapado a ningun Pokemon\n");
                break;
            case 7:
                mostrarPokedex(mapa);
                break;
            case 8:
                mostrarPokemonsOrdenadosPC(mapa);
                break;
            case 9:
                liberarPokemon(mapa);
                break;
            case 10:
                mostrarPokemonsOrdenadosPC(mapa);
                break;
        }
        printf("\n");
    } while (opcion != 0);

    printf("\n -----\n");
    printf("| FIN |\n");
    printf(" -----\n\n");

    return 0;
}
