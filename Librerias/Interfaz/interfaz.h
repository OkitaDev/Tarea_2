#ifndef INTERFAZ_H
#define INTERFAZ_H

#define green "\e[1;92m"
#define red "\e[1;91m"
#define blue "\e[1;94m"
#define yellow "\e[1;93m"
#define reset "\e[0m" 

//Funcion para mostrar el menu de opciones
void mostrarMenuPokedex();

//Funcion para mostrar menu de la opcion 1
void mostrarSeleccion();

//Funcion para ingresar una opcion del menu
short ingresarOpcion(short);

/*Funcion para que el primer caracter sea mayuscula
los demas caracteres minuscula, y si se encuentra
un salto de linea o un espacio demas en el ultimo caracter
se elimina*/
void convertirEstandar(char *);

/* Funcion para ingresar unicamenta
Macho, Hembra o No tiene*/
void validarSexo(char *);

#endif /* interfaz.h */
