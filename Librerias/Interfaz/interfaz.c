#include <stdio.h>

void mostrarMenuPokedex()
{
    printf("\n ---------\n");
    printf("| POKEDEX |\n");
    printf(" ---------\n\n");

    printf("1. Importar/Exportar desde archivo .csv\n");
    printf("2. Registrar Pokemon Atrapado\n");
    printf("3. Evolucionar Pokemon\n");
    printf("4. Buscar Pokemon por Tipo\n");
    printf("5. Buscar mis Pokemon por Nombre\n");
    printf("6. Buscar Pokemon en Pokedex por Nombre\n");
    printf("7. Mostrar todos los Pokemons en Pokedex\n");
    printf("8. Mostrar mis Pokemon ordenados por PC\n");
    printf("9. Liberar un Pokemon\n");
    printf("10. Mostrar Pokemon por Region\n");
    printf("0. Salir de la Pokedex\n");
}

void mostrarSeleccion()
{
    printf("\n1. Importar archivo .csv\n");
    printf("2. Exportar archivo .csv\n");
}

short ingresarOpcion(short pregunta)
{
    short opcion;
    if(pregunta == 0)
    {
        do
        {
            printf("\nElija una opcion: ");
            fflush(stdin);
            scanf("%hi", &opcion);
            if(opcion < 0 || opcion > 11) printf("\nNo existe tal opcion\n");
        } while (opcion < 0 || opcion > 11);
    }
    else
    {
        do
        {
            printf("\nElija una opcion: ");
            fflush(stdin);
            scanf("%hi", &opcion);
            if(opcion < 1 || opcion > 2) printf("\nNo existe tal opcion\n");
        } while (opcion < 1 || opcion > 2);
    }
    
}