#include <stdio.h>

#include "Librerias/Interfaz/interfaz.h"
#include "Librerias/TDA_Mapa/hashmap.h"
#include "Librerias/pokemon.h"

int main()
{
    short opcion;
    HashMap * mapa = createMap(100);

    do
    {
        mostrarMenuPokedex();
        fflush(stdin);
        scanf("%hi", &opcion);
        if(opcion == 0) break;

        switch(opcion)
        {
            case 1:
                mostrarSeleccion();
                scanf("%hi", &opcion);
                if(opcion == 1) importarArchivo(mapa);
                else if(opcion == 2) exportarArchivo(mapa);
                else printf("\nNo se hizo nada\n");
                break;
            case 2:
                atraparPokemon(mapa);
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                if(size(mapa) != 0) buscarPokemonNombre(mapa);
                else printf("\nNo ha atrapado algun Pokemon\n");
                break;
            case 6:
                break;
            case 7:
                break;
            case 8:
                break;
            case 9:
                break;
            case 10:
                break;
            default:
                printf("\nLa opcion que ha marcado no existe\n");
                break;
        }
        printf("\n");
    } while (opcion != 0);

    printf("\n -----\n");
    printf("| FIN |\n");
    printf(" -----\n\n");

    return 0;
}
