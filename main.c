#include <stdio.h>
#include "Librerias/Interfaz/interfaz.h"

int main()
{
    short opcion;
    do
    {
        mostrarMenuPokedex();
        scanf("%hi", &opcion);
        if(opcion == 0) break;
        
        switch(opcion)
        {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
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