#include <stdio.h>
#include <string.h>
#include <ctype.h>

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
    printf("0. No hacer nada\n");
}

short ingresarOpcion(short pregunta)
{
    short opcion;
    if(pregunta == 0)
    {
        do
        {
            printf("\nElija una opcion: ");
            scanf("%hi", &opcion);
            if(opcion < 0 || opcion > 11) printf("\nNo existe tal opcion\n");
        } while (opcion < 0 || opcion > 11);
    }
    else
    {
        do
        {
            printf("\nElija una opcion: ");
            scanf("%hi", &opcion);
            if(opcion < 0 || opcion > 2) printf("\nNo existe tal opcion\n");
        } while (opcion < 0 || opcion > 2);
    }

}

void convertirEstandar(char * cadena)
{
    cadena[0] = toupper(cadena[0]);

    //Va caracter por caracter pasandolo a minuscula
    for(int i = 1; cadena[i] != '\0' ; i++)
        cadena[i] = tolower(cadena[i]);

    int largo = strlen(cadena) - 1;
    if(cadena[largo] == ' ' || cadena[largo] == '\n') cadena[largo] = '\0';
}

void validarSexo(char * sexo)
{
    do
    {
        printf("\nIngrese el sexo del Pokemon: ");
        getchar();
        fscanf(stdin, "%20[^\n]s", sexo);
        convertirEstandar(sexo);
        
        if(strcmp(sexo, "Macho") != 0 && strcmp(sexo, "Hembra") != 0 && strcmp(sexo, "No tiene") != 0)
        {    
            printf("\nPor favor, ingrese Macho, Hembra o No tiene\n");
        }

    } while (strcmp(sexo, "Macho") != 0 && strcmp(sexo, "Hembra") != 0 && strcmp(sexo, "No tiene") != 0);
}
