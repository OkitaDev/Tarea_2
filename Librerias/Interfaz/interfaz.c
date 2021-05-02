#include <stdio.h>
#include <string.h>
#include <ctype.h>

//Muestra el menu
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

//Muestra un sub menu
void mostrarSeleccion()
{
    printf("\n1. Importar archivo .csv\n");
    printf("2. Exportar archivo .csv\n");
    printf("0. No hacer nada\n");
}

//Eleccion de opciones en un intervalo
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

//Funcion para dejar de un modo las cadenas
void convertirEstandar(char * cadena)
{
    //Sacar un espacio extra
    if(cadena[0] == ' ') memmove(cadena, cadena + 1, strlen(cadena));

    //Primer caracter en mayuscula
    cadena[0] = toupper(cadena[0]);

    //Va caracter por caracter pasandolo a minuscula
    for(int i = 1; cadena[i] != '\0' ; i++)
        cadena[i] = tolower(cadena[i]);

    int largo = strlen(cadena) - 1;
    if(cadena[largo] == ' ' || cadena[largo] == '\n') cadena[largo] = '\0';
}

//Validacion de que se ingrese Macho, Hembra o No tiene
void validarSexo(char * sexo)
{
    do
    {
        //Ingreso de la cadena
        printf("\nIngrese el sexo del Pokemon: ");
        getchar();
        fscanf(stdin, "%20[^\n]s", sexo);
        convertirEstandar(sexo);
        
        //Mensaje si no se cumple
        if(strcmp(sexo, "Macho") != 0 && strcmp(sexo, "Hembra") != 0 && strcmp(sexo, "No tiene") != 0)
        {    
            printf("\nPor favor, ingrese %cMacho%c, %cHembra%c o %cNo tiene%c\n", 34, 34, 34, 34, 34, 34);
        }

    } while (strcmp(sexo, "Macho") != 0 && strcmp(sexo, "Hembra") != 0 && strcmp(sexo, "No tiene") != 0);
}
