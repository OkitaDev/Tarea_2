#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "TDA_Mapa/hashmap.h"
#include "Interfaz/interfaz.h"

typedef struct tipoNumericos
{
	int id;
	int idPokedex;
	int ocurrencia;
	int cantidadTipos;
}tipoNumericos;

typedef struct tipoEvoluciones
{
	char evolPrevia[25];
	char evolSiguiente[25];
}tipoEvoluciones;

typedef struct tipoPuntos
{
	int pSalud;
	int pCombate;
}tipoPuntos;

typedef struct tipoPokemon
{
	char nombrePokemon[25];
	char region[25];
	char sexo[25];
	char tipos[25][50];
	tipoPuntos puntos;
	tipoEvoluciones evol;
	tipoNumericos ident;
}tipoPokemon;

/* Funcion para cambiar el ID de un pokemon, si este se
encuentra repetido*/
void idOcupado(HashMap * mapa, tipoPokemon * pokemon)
{
	tipoPokemon * aux = searchMap(mapa, &pokemon->ident.id);
	int id = pokemon->ident.id;

	if(aux != NULL)
	{
		/*Ciclo while, para verificar 
		que no choque con otro ID*/
		while(aux != NULL)
		{
			pokemon->ident.id++;
			aux = nextMap(mapa);
		}
		
		printf("\nEl pokemon %s debera cambiar su ID %i\n", pokemon->nombrePokemon,id);
		printf("Su nuevo ID es %i\n", pokemon->ident.id);
	}
}

/*Funcion para indicar si hay mas Pokemons de la misma especie
y verificar si poseen el mismo id de la Pokedex*/
short elPokemonYaExiste(HashMap * mapa, tipoPokemon * pokemon)
{
	//Creacion de variable auxiliar, por si existe otro pokemon de la misma especie
	tipoPokemon * aux = searchMap(mapa, pokemon->nombrePokemon);

	if(aux != NULL)
	{
		/* Si existe, se comprueba que posean el mismo ID de la
		Pokedex, si es asi se aumenta la ocurrencia del pokemon, si no
		se indica que no se ingresara el pokemon debido a esto*/
		if(pokemon->ident.idPokedex == aux->ident.idPokedex)
		{
			aux->ident.ocurrencia += 1;
			pokemon->ident.ocurrencia = aux->ident.ocurrencia;

			//Cambiar en las demas instancias de ocurrencia de pokemon
			while(aux != NULL)
			{
				if(strcmp(pokemon->nombrePokemon, aux->nombrePokemon) == 0)
				{
					aux->ident.ocurrencia = pokemon->ident.ocurrencia;
				}
				aux = nextMap(mapa);
			}
		}
		else
		{
			printf("\nEl numero de la Pokedex es erroneo\n");
			printf("El pokemon %s con ID %i no sera ingresado\n", pokemon->nombrePokemon, pokemon->ident.id);
			return 1;
		}
	}
	else
	{
		//Si no ha sido capturado un pokemon de la especia, se indica que es el primero
		pokemon->ident.ocurrencia = 1;
	}

	return 0;
}

/* Funcion que separa los datos del Pokemon
y forma la variable para ser almacenada en el 
mapa*/
void ingresarPokemon(HashMap * mapaNombre, HashMap * mapaID, char * lineaLeida)
{
	tipoPokemon * nuevoPokemon = (tipoPokemon *) malloc (sizeof(tipoPokemon));
	char * fragmento;

	//Lectura del ID
	fragmento = strtok(lineaLeida, ",");
	nuevoPokemon->ident.id = strtol(fragmento, NULL, 10);

	//Lectura del nombre
	fragmento = strtok(NULL, ",");
	strcpy(nuevoPokemon->nombrePokemon, fragmento);
	convertirEstandar(nuevoPokemon->nombrePokemon);
	idOcupado(mapaID, nuevoPokemon);

	//Lectura de los tipos
	fragmento = strtok(NULL, ",");

	//Se verifica si posee 1 o mas tipos
	if(fragmento[0] == '"')
	{
		//Eliminacion de las primeras comillas
		memmove(fragmento, fragmento + 1, strlen(fragmento));
		strcpy(nuevoPokemon->tipos[0], fragmento);
		convertirEstandar(nuevoPokemon->tipos[0]);
		short largo;
		short cantidad = 1;

		//Reiteracion hasta que no hayan mas tipos
		do
		{
			fragmento = strtok(NULL, ",");
			largo = strlen(fragmento) - 1;
			if(fragmento[largo] != '"')
			{
				strcpy(nuevoPokemon->tipos[cantidad], fragmento);
				convertirEstandar(nuevoPokemon->tipos[cantidad]);
			}
			cantidad++;
		} while (fragmento[largo] != '"');

		//Eliminacion de la comillas finales
		fragmento[largo] = '\0';
		convertirEstandar(nuevoPokemon->tipos[cantidad - 1]);
		strcpy(nuevoPokemon->tipos[cantidad - 1], fragmento);
		nuevoPokemon->ident.cantidadTipos = cantidad;
	}
	else
	{
		strcpy(nuevoPokemon->tipos[0], fragmento);
		convertirEstandar(nuevoPokemon->tipos[0]);
		nuevoPokemon->ident.cantidadTipos = 1;
	}

	//Lectura de los puntos de Combate
	fragmento = strtok(NULL, ",");
	nuevoPokemon->puntos.pCombate = strtol(fragmento, NULL, 10);

	//Lectura de los Puntos de Salud
	fragmento = strtok(NULL, ",");
	nuevoPokemon->puntos.pSalud = strtol(fragmento, NULL, 10);

	//Lectura del sexo del Pokemon
	fragmento = strtok(NULL,",");
	strcpy(nuevoPokemon->sexo, fragmento);

	//Lectura de las evoluciones
	fragmento = strtok(NULL,",");
	strcpy(nuevoPokemon->evol.evolPrevia, fragmento);
	fragmento = strtok(NULL,",");
	strcpy(nuevoPokemon->evol.evolSiguiente, fragmento);

	//Lectura del numero de la Pokedex
	fragmento = strtok(NULL, ",");
	nuevoPokemon->ident.idPokedex = strtol(fragmento, NULL, 10);

	//Aumentar la cantidad de veces que posee un Pokemon y si el nro de Pokedex del Pokemon es el mismo
	if(elPokemonYaExiste(mapaNombre, nuevoPokemon) == 1) return;

	//Lectura de la Region
	fragmento = strtok(NULL,",");
	strcpy(nuevoPokemon->region, fragmento);
	convertirEstandar(nuevoPokemon->region);

	//Añadir Pokemon al Mapa (Con clave = nombre)
	insertMap(mapaNombre, nuevoPokemon->nombrePokemon, nuevoPokemon);
	insertMap(mapaID, & nuevoPokemon->ident.id, nuevoPokemon);
}

/* Funcion que lee el archivo y separa sus lineas
para poder ser leidas por ingresarPokemon()*/
void importarArchivo(HashMap * mapaNombre, HashMap * mapaID)
{
	//Si hay mas de 100 pokemon, no hace nada
	if(size(mapaNombre) + 1 >= 100)
	{
		printf("\nHa superado el maximo de Pokemons!\n");
		return;
	}

	//Ingreso del nombre del archivo a implementar
	char nombreArchivo[40];
	printf("\nIngrese el nombre del archivo (Incluyendo el .csv): ");
	getchar();
	fscanf(stdin, "%39[^\n]s", nombreArchivo);

	//Busqueda del archivo
	FILE * archivo = fopen(nombreArchivo, "r");

	//Si no se encuentra el archivo
	if(archivo == NULL)
	{
		printf("\nArchivo NO IMPORTADO!\n");
		return;
	}

	char lineaLeida[101];
	//Eliminacion de la primera linea del .csv
	fgets(lineaLeida, 100, archivo);

	//Lectura del archivo (saltandose la primera linea)
	while(fgets(lineaLeida, 100, archivo))
	{
		//If por si hay mas de 100 pokemon
		if(size(mapaNombre) + 1 >= 100)
		{
			printf("\nHa superado el maximo de Pokemons\n");
			printf("Se ha podido algunos implementar pokemons!\n");
			break;
		}

		//If para aumentar el tamaño de los mapas
		if(size(mapaNombre) == round(capacity(mapaNombre) * 0.7))
		{
			enlarge(mapaNombre);
			enlarge(mapaID);
		}

		//Llamada a funcion para insertar en el mapa
		ingresarPokemon(mapaNombre, mapaID, lineaLeida);
	}

	printf("\nArchivo IMPORTADO!\n");
	fclose(archivo);
}

/* Funcion para copiar la informacion del mapa
a un archivo .csv*/
void exportarArchivo(HashMap * mapa)
{
	/* Si el mapa esta vacio
	no se exportara nada*/
	if(size(mapa) == 0)
	{
		printf("\nNo ha atrapado ningun Pokemon\n");
		return;
	}

	//Lectura del nombre del archivo
	char nombreArchivo[40];
	printf("\nIngrese el nombre del nuevo archivo (Incluyendo el .csv): ");
	getchar();
	fscanf(stdin, "%39[^\n]s", nombreArchivo);

	//Acceso al archivo
	FILE * archivo = fopen(nombreArchivo, "w");
	if(archivo == NULL)
	{
		printf("\nArchivo NO EXPORTADO!\n");
		return;
	}

	//Ingreso de la primera linea del archivo.csv
	fprintf(archivo, "id,nombre,tipos,pc,ps,sexo,evolucion previa,evolucion posterior,numero pokedex,region\n");

	//Ingreso de la informacion de los pokemons en el mapa
	tipoPokemon * auxPokemon = firstMap(mapa);
	while(auxPokemon != NULL)
	{
		//Ingreso de ID y nombre
		fprintf(archivo, "%i,%s,", auxPokemon->ident.id, auxPokemon->nombrePokemon, 34);

		//Copia de tipos, dependiendo de si posee 1 o mas tipos, se colocan comillas
		if(auxPokemon->ident.cantidadTipos != 1) fprintf(archivo, "%c", 34);
		for(int i = 0; i < auxPokemon->ident.cantidadTipos; i++)
		{
				fprintf(archivo, "%s",auxPokemon->tipos[i]);
				if(i < auxPokemon->ident.cantidadTipos - 1) fprintf(archivo, "%c", 44);
		}
		if(auxPokemon->ident.cantidadTipos != 1) fprintf(archivo, "%c", 34);

		//Ingreso de los demas datos
		fprintf(archivo, ",%i,%i,%s",auxPokemon->puntos.pCombate ,auxPokemon->puntos.pSalud, auxPokemon->sexo);
		fprintf(archivo, ",%s,%s",auxPokemon->evol.evolPrevia, auxPokemon->evol.evolSiguiente);
		fprintf(archivo, ",%i,%s", auxPokemon->ident.idPokedex, auxPokemon->region);
		auxPokemon = nextMap(mapa);
	}

	printf("\nArchivo EXPORTADO!\n");
	fclose(archivo);
}

/* Funcion para ingresar la informacion de 1
nuevo pokemon*/
void atraparPokemon(HashMap * mapaNombre, HashMap * mapaID)
{
	//Aumento de tamaño de los mapas
	if(size(mapaNombre) == round(capacity(mapaNombre) * 0.7))
	{
		enlarge(mapaNombre);
		enlarge(mapaID);
	}

	//Creacion de variables a usar
	tipoPokemon * pokemonAtrapado = malloc(sizeof(tipoPokemon));
	char tipo[50];
	int i;

	//Ingreso del ID
	pokemonAtrapado->ident.id = size(mapaNombre) + 1;

	//Ingreso del nombre
	printf("\nIngrese el nombre del Pokemon: ");
	getchar();
	fscanf(stdin, "%39[^\n]s", pokemonAtrapado->nombrePokemon);
	convertirEstandar(pokemonAtrapado->nombrePokemon);
	idOcupado(mapaID, pokemonAtrapado);

	//Ingreso del numero de la Pokedex
	printf("\nIngrese el numero de ubicacion de la Pokedex: ");
	getchar();
	fscanf(stdin, "%i", &pokemonAtrapado->ident.idPokedex);

	//Revision de Ocurrencia
	if(elPokemonYaExiste(mapaNombre, pokemonAtrapado) == 1) return;

	//Ingreso de los tipos
	for(i = 0; i < 25; i++)
	{
		printf("\nIngrese el tipo %i del Pokemon (Ingrese no para parar): ", i + 1);
		getchar();
		fscanf(stdin, "%49[^\n]s", tipo);
		convertirEstandar(tipo);
		if(strcmp("No", tipo) == 0) break;
		strcat(tipo, " ");
		strcpy(pokemonAtrapado->tipos[i], tipo);
	}
	//Cantidad de tipos que posee el pokemon
	pokemonAtrapado->ident.cantidadTipos = i;

	//Ingreso de PC y PS
	printf("\nIngrese los PS del Pokemon: ");
	getchar();
	fscanf(stdin, "%i", &pokemonAtrapado->puntos.pSalud);
	printf("\nIngrese los PC del Pokemon: ");
	getchar();
	fscanf(stdin, "%i", &pokemonAtrapado->puntos.pCombate);

	//Ingreso del sexo del Pokemon
	printf("\nIngrese el sexo del Pokemon: ");
	getchar();
	fscanf(stdin, "%24[^\n]s", pokemonAtrapado->sexo);
	convertirEstandar(pokemonAtrapado->sexo);

	//Ingreso de las evoluciones
	printf("\nIngrese la evolucion anterior del Pokemon: ");
	getchar();
	fscanf(stdin, "%24[^\n]s", pokemonAtrapado->evol.evolPrevia);
	convertirEstandar(pokemonAtrapado->evol.evolPrevia);
	printf("\nIngrese la evolucion siguiente del Pokemon: ");
	getchar();
	fscanf(stdin, "%24[^\n]s", pokemonAtrapado->evol.evolSiguiente);
	convertirEstandar(pokemonAtrapado->evol.evolSiguiente);

	//Ingreso de la region del Pokemon
	printf("\nIngrese la region originaria del Pokemon: ");
	getchar();
	fscanf(stdin, "%24[^\n]s", pokemonAtrapado->region);
	strcat(pokemonAtrapado->region, "\n");
	convertirEstandar(pokemonAtrapado->region);

	//Ingreso al mapa
	insertMap(mapaNombre, pokemonAtrapado->nombrePokemon, pokemonAtrapado);
	insertMap(mapaID, & pokemonAtrapado->ident.id, pokemonAtrapado);
}

void evolucionarPokemon(HashMap * mapa)
{

}

void busquedaPorTipo(HashMap * mapa)
{

}

void buscarPokemonNombre(HashMap * mapa)
{
	char nombreBuscado[25];
	tipoPokemon * pokemonAuxiliar = firstMap(mapa);
	short existePokemon = 0;

	//Ingreso del nombre del Pokemon
	printf("\nIngrese el nombre del Pokemon a buscar: ");
	getchar();
	fscanf(stdin, "%24[^\n]s", nombreBuscado);
	convertirEstandar(nombreBuscado);

	/*Se recorre el Mapa ya que no se sabe la cantidad de Pokemons
	atrapados con el mismo nombre, en vez de usar la funcion searchMap,
	ya que esta entrega solo uno de los posibles pokemon*/
	while(pokemonAuxiliar != NULL)
	{
		if(strcmp(pokemonAuxiliar->nombrePokemon, nombreBuscado) == 0)
		{
			printf("\n%s Sexo: %s\n", pokemonAuxiliar->nombrePokemon, pokemonAuxiliar->sexo);
			printf("Tipo(s): ");

			for(int i = 0; i < pokemonAuxiliar->ident.cantidadTipos; i++)
				printf("%s", pokemonAuxiliar->tipos[i]);

			printf("\nID: %i Pokedex: %i\n", pokemonAuxiliar->ident.id, pokemonAuxiliar->ident.idPokedex);
			printf("PS: %i PC: %i\n", pokemonAuxiliar->puntos.pSalud, pokemonAuxiliar->puntos.pCombate);
			existePokemon = 1;
		}
		pokemonAuxiliar = nextMap(mapa);
	}

	//Si no se encuentra
	if(existePokemon == 0) printf("\nNo ha atrapado %s\n", nombreBuscado);
}

void buscarPokemonPokedex(HashMap * mapa)
{
	char nombreBuscado[25];
	tipoPokemon * pokemon;

	//Ingreso nombre de pokemón
	printf("\nIngrese el pokemon a buscar: ");
	getchar();
	fscanf(stdin, "%24[^\n]s", nombreBuscado);
	convertirEstandar(nombreBuscado);

	//Se busca el pokemon y se ingresa en el auxiliar
	pokemon = searchMap(mapa,nombreBuscado);

	//De no ser el auxiliar NULL, se imprimen sus datos por pantalla
	if(pokemon != NULL)
	{
		printf("\nNombre: %s Numero Pokedex: %i\n", pokemon->nombrePokemon, pokemon->ident.idPokedex);
		printf("Region: %sTipos: ", pokemon->region);
		for(int i = 0; i < pokemon->ident.cantidadTipos; i++)
			printf("%s", pokemon->tipos[i]);

		printf("\nEvolucion Previa: %s Evolucion Siguiente: %s", pokemon->evol.evolPrevia,pokemon->evol.evolSiguiente);
		printf("\nVeces Atrapado: %i\n", pokemon->ident.ocurrencia);
	}
	else
	{
		printf("No se encuentra el pokemon ingresado.\n");
	}
}

void mostrarPokedex(HashMap * mapa)
{
	tipoPokemon * pokemon;
	pokemon = firstMap(mapa);
	int ultimo;
	tipoPokemon * menor;
	tipoPokemon * mayor;

	menor=(tipoPokemon*)calloc(1,sizeof(tipoPokemon));
	mayor=(tipoPokemon*)calloc(1,sizeof(tipoPokemon));
	mayor->ident.idPokedex = 0;
	menor->ident.idPokedex = 0;

	do
	{
		if(pokemon->ident.idPokedex > mayor->ident.idPokedex) mayor = pokemon;
		pokemon = nextMap(mapa);

	}while(pokemon != NULL);

	do
	{
		pokemon=firstMap(mapa);
		menor = mayor;
		do
		{
			if(pokemon->ident.idPokedex <= menor->ident.idPokedex && pokemon->ident.idPokedex > ultimo)
				menor = pokemon;

			pokemon = nextMap(mapa);

		}while(pokemon!=NULL);

		ultimo = menor->ident.idPokedex;

		printf("\n%d)%s\n",menor->ident.idPokedex, menor->nombrePokemon);
		printf("Tipo(s): %s\n", menor->tipos);
		printf("Evolucion Previa: %s ",menor->evol.evolPrevia);
		printf("Evolucion siguiente: %s\n",menor->evol.evolSiguiente);
		printf("Region: %s", menor->region);
		printf("Cantidad en posesion: %d\n",menor->ident.ocurrencia);

	}while(menor != mayor);
}

void mostrarPokemonsOrdenadosPC(HashMap * mapa)
{

}

void liberarPokemon(HashMap * mapa)
{

}

void mostrarPokemonRegion(HashMap * mapa)
{

}
