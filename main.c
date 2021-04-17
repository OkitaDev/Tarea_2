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
    } while (opcion != 0);
    
    printf("\n -----\n");
    printf("| FIN |\n");
    printf(" -----\n\n");

    return 0;
}