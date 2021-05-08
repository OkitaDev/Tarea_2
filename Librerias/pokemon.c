#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "TDA_Mapa/hashmap.h"
#include "Interfaz/interfaz.h"

//Definicion de estructuras
typedef struct tipoTipos
{
	char tipos[25][25];
	int cantidadTipos;
}tipoTipos;

typedef struct tiposIdentificaciones
{
	int id;
	int idPokedex;
}tiposIdentificaciones;

typedef struct tipoEvoluciones
{
	char evolPrevia[25];
	char evolSiguiente[25];
}tipoEvoluciones;

typedef struct tipoPuntos
{
	int pSalud;
	int pCombate;
	short flag;
}tipoPuntos;

typedef struct tipoPokemon
{
	char nombrePokemon[25];
	char region[25];
	char sexo[25];
	tipoTipos elementos;
	tipoPuntos puntos;
	tipoEvoluciones evol;
	tiposIdentificaciones ident;
}tipoPokemon;

typedef struct tipoPokedex
{
	int cantidadAtrapado;
	char nombrePokemon[25];
	char region[25];
	tipoTipos elementos;
	tipoEvoluciones evol;
	tiposIdentificaciones ident;
}tipoPokedex;

/* Funcion para ingresar los tipos del
pokemon en un arregloPokemon de cadenas*/
void ingresoDeTipos(tipoPokemon * pokemon)
{
	//Inicializacion de variables
	char tipo[25];
	int i = 0;

	while(i != 25)
	{
		//Ingreso del tipo
		printf("\nIngrese el tipo %i del Pokemon (Ingrese %cno%c para avanzar): ", i + 1, 34, 34);
		getchar();
		fscanf(stdin, "%24[^\n]s", tipo);
		
		//Convertir de "fuego " a "Fuego", por ejemplo
		convertirEstandar(tipo);

		if(i == 0 && strcmp(tipo, "No") == 0) //Obliga a colocar 1 tipo minimo al pokemon
		{
			printf(red "\nEl pokemon debe tener al menos 1 tipo\n" reset);
		}
		else //Almacenar el valor, y cuando tenga 1 tipo, se puede ingresar no
		{
			if(strcmp("No", tipo) == 0) break;
			strcpy(pokemon->elementos.tipos[i], tipo);
			i++;
		}
	}

	//Cantidad de tipos que posee el pokemon
	pokemon->elementos.cantidadTipos = i;
}

/* Funcion para ingresar elementos a una variable tipoPokedex
en conjunto de la insercion al mapaPokedex*/
void creacionEntradaPokedex(HashMap * mapaPokedex, tipoPokedex * entrada, tipoPokemon * pokemon)
{
	//Insercion de elementos necesarios para el tipoPokedex a traves de un tipoPokemon
	entrada->elementos.cantidadTipos = pokemon->elementos.cantidadTipos;
	entrada->ident.idPokedex = pokemon->ident.idPokedex;

	for(int i = 0; i < pokemon->elementos.cantidadTipos; i++)
		strcpy(entrada->elementos.tipos[i], pokemon->elementos.tipos[i]);

	strcpy(entrada->evol.evolSiguiente, pokemon->evol.evolSiguiente);
	strcpy(entrada->evol.evolPrevia, pokemon->evol.evolPrevia);
	strcpy(entrada->region, pokemon->region);
	insertMap(mapaPokedex, entrada->nombrePokemon, entrada);
}

/* Funcion que indica si el ID de la pokedex
esta correcto o no*/
short idPokedexCorrecto(HashMap * mapa, tipoPokemon * pokemon)
{
	//Busqueda de la entrada de Pokedex del pokemon
	tipoPokedex * entradaAuxiliar = searchMap(mapa, pokemon->nombrePokemon);

	if(entradaAuxiliar != NULL) //Si existe una entrada del pokemon
	{
		//Comparar si el numero de la pokedex es el mismo o no
		if(entradaAuxiliar->ident.idPokedex != pokemon->ident.idPokedex)
		{
			printf(red"\nEl pokemon %s con el ID %i, posee su ID Pokedex erroneo\n",pokemon->nombrePokemon, pokemon->ident.id);
			printf("Por lo tanto no se ingresara a la Pokedex o al PC\n"reset);
			return 1;
		}
	}

	return 0;
}

/* Funcion para cambiar el ID de un pokemon, si este se
encuentra repetido*/
void idOcupado(HashMap * mapa, tipoPokemon * pokemon)
{
	//Busqueda de un pokemon con el mismo id
	tipoPokemon * aux = searchMap(mapa, &pokemon->ident.id);
	int id;

	if(aux != NULL) //Si existe otro pokemon con el ID
	{
		id = pokemon->ident.id;
		
		/*Ciclo while, para verificar
		que no choque con otro ID con
		el mismo valor*/
		while(aux != NULL)
		{
			pokemon->ident.id++;
			aux = searchMap(mapa, &pokemon->ident.id);
			if(aux == NULL) break;
		}

		printf(blue"\nEl pokemon %s debera cambiar su ID %i\n", pokemon->nombrePokemon,id);
		printf("Su nuevo ID es %i\n"reset, pokemon->ident.id);
	}
}

/*Funcion para indicar si hay mas Pokemons de la misma especie*/
short elPokemonExiste(HashMap * mapa, tipoPokedex * entradaPokemon)
{
	//En el caso de que aun no exista la entrada (especificamente para evolucionarPokemon)
	if(entradaPokemon == NULL) return 1;

	//Creacion de variable auxiliar, por si existe otro pokemon de la misma especie
	tipoPokedex * aux = searchMap(mapa, entradaPokemon->nombrePokemon);

	if(aux != NULL)
	{
		/* Si existe, se comprueba que posean el mismo ID de la
		Pokedex, si es asi se aumenta la ocurrencia del pokemon, si no
		se indica que no se ingresara el pokemon debido a esto*/
		aux->cantidadAtrapado += 1;
	}
	else
	{
		//Si no ha sido capturado un pokemon de la especia, se indica que es el primero
		entradaPokemon->cantidadAtrapado = 1;
		return 1;
	}

	return 0;
}

/* Funcion que separa los elementos del Pokemon
y forma la variable para ser almacenada en el
mapa*/
void lecturaDeInformacion(HashMap * mapaNombre, HashMap * mapaID, HashMap * mapaPokedex, char * lineaLeida)
{
	//Creacion de structs
	tipoPokemon * nuevoPokemon = (tipoPokemon *) malloc (sizeof(tipoPokemon));
	tipoPokedex * nuevaEntrada = (tipoPokedex *) malloc (sizeof(tipoPokedex));
	
	//Cadena para separar la linea
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
		//Eliminacion de las comillas iniciales
		memmove(fragmento, fragmento + 1, strlen(fragmento));
		
		//Copia de informacion
		strcpy(nuevoPokemon->elementos.tipos[0], fragmento);
		convertirEstandar(nuevoPokemon->elementos.tipos[0]);
	
		short largo; //Almacenar largo de la cadena
		short cantidad = 1; //Contador

		//Reiteracion que finaliza cuando no posea mas tipos
		do
		{
			fragmento = strtok(NULL, ",");
			largo = strlen(fragmento) - 1;

			if(fragmento[largo] != '"') //Mientras no reconozca las comillas finales
			{
				strcpy(nuevoPokemon->elementos.tipos[cantidad], fragmento);
				convertirEstandar(nuevoPokemon->elementos.tipos[cantidad]);
			}

			cantidad++;
		} while (fragmento[largo] != '"');

		//Eliminacion de la comillas finales
		fragmento[largo] = '\0';

		strcpy(nuevoPokemon->elementos.tipos[cantidad - 1], fragmento);
		convertirEstandar(nuevoPokemon->elementos.tipos[cantidad - 1]);
		nuevoPokemon->elementos.cantidadTipos = cantidad;
	}
	else
	{
		strcpy(nuevoPokemon->elementos.tipos[0], fragmento);
		convertirEstandar(nuevoPokemon->elementos.tipos[0]);
		nuevoPokemon->elementos.cantidadTipos = 1;
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

	if(idPokedexCorrecto(mapaPokedex, nuevoPokemon) == 1) return;

	//Lectura de la Region
	fragmento = strtok(NULL,",");
	strcpy(nuevoPokemon->region, fragmento);
	convertirEstandar(nuevoPokemon->region);

	//Copia de nombre a variable tipoPokedex
	strcpy(nuevaEntrada->nombrePokemon, nuevoPokemon->nombrePokemon);

	/* Si es la primera especie del pokemon
	se creara una entrada en la pokedex*/
	if(elPokemonExiste(mapaPokedex, nuevaEntrada) == 1) 
	{
		creacionEntradaPokedex(mapaPokedex, nuevaEntrada, nuevoPokemon);
	}

	//Añadir Pokemon al Mapa (Con clave = nombre)
	insertMap(mapaNombre, nuevoPokemon->nombrePokemon, nuevoPokemon);
	insertMap(mapaID, & nuevoPokemon->ident.id, nuevoPokemon);
}

/* Funcion que lee el archivo y separa sus lineas
para poder ser leidas por ingresarPokemon()*/
void importarArchivo(HashMap * mapaNombre, HashMap * mapaID, HashMap * mapaPokedex)
{
	char nombreArchivo[40];

	//Ingreso del nombre del archivo a implementar
	printf("\nIngrese el nombre del archivo (Incluyendo el .csv): ");
	getchar();
	fscanf(stdin, "%39[^\n]s", nombreArchivo);

	//Busqueda del archivo
	FILE * archivo = fopen(nombreArchivo, "r");

	//Si no se encuentra el archivo, no se ingresa nada
	if(archivo == NULL)
	{
		printf(red"\nArchivo NO IMPORTADO!\n"reset);
		return;
	}

	char lineaLeida[101]; //Variable de separacion de lineas del .csv
	
	//Eliminacion de la primera linea del .csv
	fgets(lineaLeida, 100, archivo);

	//Lectura del archivo (saltandose la primera linea)
	while(fgets(lineaLeida, 100, archivo))
	{
		//If por si hay mas de 100 pokemon
		if(size(mapaNombre) >= 100)
		{
			printf(blue"\nHa superado el maximo de Pokemons\n");
			printf("Se ha podido algunos implementar pokemons!\n"reset);
			break;
		}

		//Llamada a funcion para insertar en el mapa
		lecturaDeInformacion(mapaNombre, mapaID, mapaPokedex,lineaLeida);
	}

	printf(green"\nArchivo IMPORTADO!\n"reset);
	fclose(archivo);
}

/* Funcion para copiar la informacion del mapa
a un archivo .csv*/
void exportarArchivo(HashMap * mapaID)
{
	/* Si el mapa esta vacio
	no se exportara nada*/
	if(size(mapaID) == 0)
	{
		printf(red "\nNo ha atrapado ningun Pokemon\n" reset);
		return;
	}

	char nombreArchivo[40];

	//Lectura del nombre del archivo
	printf("\nIngrese el nombre del nuevo archivo (Incluyendo el .csv): ");
	getchar();
	fscanf(stdin, "%39[^\n]s", nombreArchivo);

	//Acceso al archivo
	FILE * archivo = fopen(nombreArchivo, "w");
	
	//Si no se puede crear o modificar el archivo, se retorna termina
	if(archivo == NULL)
	{
		printf(red "\nArchivo NO EXPORTADO!\n" reset);
		return;
	}

	//Ingreso de la primera linea del archivo.csv (informacion de tabla)
	fprintf(archivo, "id,nombre,tipos,pc,ps,sexo,evolucion previa,evolucion posterior,numero pokedex,region\n");

	//Ingreso de la informacion de los pokemons en el mapa
	tipoPokemon * pokemonAuxiliar = firstMap(mapaID);

	while(pokemonAuxiliar != NULL)
	{
		//Ingreso de ID y nombre
		fprintf(archivo,"%i,%s,", pokemonAuxiliar->ident.id, pokemonAuxiliar->nombrePokemon);

		//Copia de tipos, dependiendo de si posee 1 o mas tipos, se colocan comillas o no
		if(pokemonAuxiliar->elementos.cantidadTipos != 1) fprintf(archivo, "%c", 34);

		for(int i = 0; i < pokemonAuxiliar->elementos.cantidadTipos; i++)
		{
			fprintf(archivo, "%s",pokemonAuxiliar->elementos.tipos[i]);

			//Evitar colocar comas demás
			if(i < pokemonAuxiliar->elementos.cantidadTipos - 1) fprintf(archivo, "%c", 44);
		}
		
		if(pokemonAuxiliar->elementos.cantidadTipos != 1) fprintf(archivo, "%c", 34);

		//Ingreso de los demas elementos (putnos, evolucion, IDs)
		fprintf(archivo, ",%i,%i,%s",pokemonAuxiliar->puntos.pCombate ,pokemonAuxiliar->puntos.pSalud, pokemonAuxiliar->sexo);
		fprintf(archivo, ",%s,%s",pokemonAuxiliar->evol.evolPrevia, pokemonAuxiliar->evol.evolSiguiente);
		fprintf(archivo, ",%i,%s\n", pokemonAuxiliar->ident.idPokedex, pokemonAuxiliar->region);

		//Avance del mapa
		pokemonAuxiliar = nextMap(mapaID);
	}

	printf(green "\nArchivo EXPORTADO!\n" reset);
	fclose(archivo);
}

/* Funcion para ingresar la informacion de 1
nuevo pokemon*/
void registrarPokemon(HashMap * mapaNombre, HashMap * mapaID, HashMap * mapaPokedex)
{
	//Creacion de variables a usar
	tipoPokemon * nuevoPokemon = (tipoPokemon *) malloc (sizeof(tipoPokemon));
	tipoPokedex * nuevaEntrada =(tipoPokedex *) malloc (sizeof(tipoPokedex));

	//Ingreso del ID
	nuevoPokemon->ident.id = size(mapaNombre) + 1;

	//Ingreso del nombre
	printf("\nIngrese el nombre del Pokemon: ");
	getchar();
	fscanf(stdin, "%39[^\n]s", nuevoPokemon->nombrePokemon);
	convertirEstandar(nuevoPokemon->nombrePokemon);

	//Se corrobora que el ID no este repetido
	idOcupado(mapaID, nuevoPokemon);

	//Ingreso del numero de la Pokedex
	printf("\nIngrese el numero de ubicacion de la Pokedex: ");
	getchar();
	fscanf(stdin, "%i", &nuevoPokemon->ident.idPokedex);

	//Se corrobora que el ID Pokedex este correcto
	if(idPokedexCorrecto(mapaPokedex, nuevoPokemon) == 1) return;

	//Ingreso de los tipos del Pokemon
	ingresoDeTipos(nuevoPokemon);

	//Ingreso de PC y PS
	printf("\nIngrese los PS del Pokemon: ");
	getchar();
	fscanf(stdin, "%i", &nuevoPokemon->puntos.pSalud);

	printf("\nIngrese los PC del Pokemon: ");
	getchar();
	fscanf(stdin, "%i", &nuevoPokemon->puntos.pCombate);

	//Ingreso del sexo del Pokemon y validacion
	validarSexo(nuevoPokemon->sexo);

	//Ingreso de las evoluciones (previa y posterior)
	printf("\nIngrese la evolucion anterior del Pokemon: ");
	getchar();
	fscanf(stdin, "%24[^\n]s", nuevoPokemon->evol.evolPrevia);
	convertirEstandar(nuevoPokemon->evol.evolPrevia);

	printf("\nIngrese la evolucion siguiente del Pokemon: ");
	getchar();
	fscanf(stdin, "%24[^\n]s", nuevoPokemon->evol.evolSiguiente);
	convertirEstandar(nuevoPokemon->evol.evolSiguiente);

	//Ingreso de la region del Pokemon
	printf("\nIngrese la region originaria del Pokemon: ");
	getchar();
	fscanf(stdin, "%24[^\n]s", nuevoPokemon->region);
	convertirEstandar(nuevoPokemon->region);

	//Copia de nombre del tipoPokemon al tipoPokedex
	strcpy(nuevaEntrada->nombrePokemon, nuevoPokemon->nombrePokemon);

	//Revisar si es un nuevo pokemon o uno existente
	if(elPokemonExiste(mapaPokedex, nuevaEntrada) != 0)
	{
		creacionEntradaPokedex(mapaPokedex, nuevaEntrada, nuevoPokemon);
	}

	//Ingreso al mapa
	insertMap(mapaNombre, nuevoPokemon->nombrePokemon, nuevoPokemon);
	insertMap(mapaID, & nuevoPokemon->ident.id, nuevoPokemon);

	printf(green"\nPokemon ingresado correctamente!\n"reset);
}

void evolucionarPokemon(HashMap * mapaID, HashMap * mapaPokedex)
{
	int idBuscado; //Variable para buscar pokemon

	//Busqueda del Pokemon a evolucionar
	printf("\nIngrese el ID del Pokemon a evolucionar: ");
	getchar();
	fscanf(stdin, "%i", &idBuscado);

	tipoPokemon * pokemonAuxiliar = searchMap(mapaID, &idBuscado); //Variable con el pokemon buscado

	//Si el ID tiene relacionado un pokemon o no
	if(pokemonAuxiliar != NULL)
	{
		//Si posee evolucion o no
		if(strcmp(pokemonAuxiliar->evol.evolSiguiente, "No tiene") != 0)
		{
			//Muestra de informacion
			printf("\nNombre: %s\n", pokemonAuxiliar->nombrePokemon);
			printf("PS: %i PC: %i\n", pokemonAuxiliar->puntos.pSalud, pokemonAuxiliar->puntos.pCombate);

			//Variable para almacenar el nombre del pokemon sin evolucionar
			char auxNombre[25];
			strcpy(auxNombre,pokemonAuxiliar->nombrePokemon);

			//Variable para buscar el pokemon en la pokedex y reducir su cantidad (si existe)
			tipoPokedex * entradaAuxiliar = searchMap(mapaPokedex, pokemonAuxiliar->nombrePokemon);
			if(entradaAuxiliar != NULL) entradaAuxiliar->cantidadAtrapado--;

			//Aumento de numero de ID Pokemon
			pokemonAuxiliar->ident.idPokedex++;

			//Cambio en el orden de evoluciones
			strcpy(pokemonAuxiliar->nombrePokemon, pokemonAuxiliar->evol.evolSiguiente);
			strcpy(pokemonAuxiliar->evol.evolPrevia, auxNombre);

			//Aumento en los puntos, debido a la evolucion
			pokemonAuxiliar->puntos.pSalud = round(pokemonAuxiliar->puntos.pSalud * 1.25);
			pokemonAuxiliar->puntos.pCombate = round(pokemonAuxiliar->puntos.pCombate * 1.50);

			//Variable para encontrar si hay mas de la especie (evolucionada) o no
			tipoPokedex * entradaAuxiliar2 = searchMap(mapaPokedex, pokemonAuxiliar->nombrePokemon);
			
			//Si hay mas de su especie, se usa esa evolucion para rellenar elementos
			if(elPokemonExiste(mapaPokedex, entradaAuxiliar2) == 0)
			{
				strcpy(pokemonAuxiliar->evol.evolSiguiente, entradaAuxiliar2->evol.evolSiguiente); //Copia de evolucion

				for(int i = 0; i < entradaAuxiliar2->cantidadAtrapado; i++) //Copia de tipos
					strcpy(pokemonAuxiliar->elementos.tipos[i], entradaAuxiliar2->elementos.tipos[i]);

				pokemonAuxiliar->elementos.cantidadTipos = entradaAuxiliar2->elementos.cantidadTipos; //Copia de cantidad
			}
			else //Si no, se debe ingresar la evolucion y los tipos
			{
				//Ingreso de la evolucion
				printf("\nIngrese la siguiente evolucion de %s: ", pokemonAuxiliar->nombrePokemon);
				getchar();
				fscanf(stdin, "%24[^\n]s", pokemonAuxiliar->evol.evolSiguiente);
				convertirEstandar(pokemonAuxiliar->evol.evolSiguiente);

				//Ingreso de los tipos del pokemon evolucionado
				ingresoDeTipos(pokemonAuxiliar);
				
				//Creacion de nueva entrada Pokedex
				entradaAuxiliar2 = (tipoPokedex *) malloc (sizeof(tipoPokedex));
				strcpy(entradaAuxiliar2->nombrePokemon, pokemonAuxiliar->nombrePokemon);
				creacionEntradaPokedex(mapaPokedex, entradaAuxiliar2, pokemonAuxiliar);
				entradaAuxiliar2->cantidadAtrapado = 1;
			}

			//Muestra de informacion
			printf(green"\nPokemon Evolucionado!\n"reset);
			printf("\nNombre: %s\n", pokemonAuxiliar->nombrePokemon);
			printf("PS: %i PC: %i\n", pokemonAuxiliar->puntos.pSalud, pokemonAuxiliar->puntos.pCombate);
		}
		else
		{
			printf(blue"\nEl Pokemon %s no posee evolucion\n"reset, pokemonAuxiliar->nombrePokemon);
		}
	}
	else
	{
		printf(red"\nNo existe un Pokemon asociado al ID %i\n"reset, idBuscado);
	}
}

void busquedaPorTipo(HashMap * mapaNombre)
{
	char tipoBuscado[25];
	tipoPokemon * pokemonAuxiliar;
	int contTipos;
	short existe = 0;

	printf("\nIngrese el tipo que desea buscar: ");
	getchar();
	fscanf(stdin, "%24[^\n]s", tipoBuscado);
	convertirEstandar(tipoBuscado);
	
	pokemonAuxiliar = firstMap(mapaNombre);

	while(pokemonAuxiliar != NULL)
	{
		contTipos = pokemonAuxiliar->elementos.cantidadTipos - 1;
	
		while(contTipos != -1)
		{
			if(strcmp(pokemonAuxiliar->elementos.tipos[contTipos], tipoBuscado) == 0)
			{
				if(existe == 0) printf(green"\nTodos los Pokemons de tipo %s:\n"reset, tipoBuscado);
				printf("\n%s ID: %d \nTipo(s): ",pokemonAuxiliar->nombrePokemon, pokemonAuxiliar->ident.id);
				
				for(int i = 0; i < pokemonAuxiliar->elementos.cantidadTipos; i++)
					printf("%s ", pokemonAuxiliar->elementos.tipos[i]);
				
				printf("\nPS: %i PC: %i\n", pokemonAuxiliar->puntos.pSalud, pokemonAuxiliar->puntos.pCombate);

				existe = 1; //Indica que encontro al menos 1 con el tipo similar al que pide
			}
			contTipos--;
		}

		pokemonAuxiliar = nextMap(mapaNombre);
		if(pokemonAuxiliar == NULL) break;
	}

	if(existe == 0)
	{
		printf(red"\nUsted no posee ningun Pokemon del tipo %s\n"reset, tipoBuscado);
	}
}

/* Buscar a todos los pokemon con el mismo nombre
y mostrar sus elementos de combate*/
void buscarPokemonNombre(HashMap * mapaNombre)
{
	char nombreBuscado[25]; //Variable para almacenar el nombre a buscar
	tipoPokemon * pokemonAuxiliar; //Variable para recorrer el mapa
	short existePokemon = 0; //Variable booleana

	//Ingreso del nombre del Pokemon
	printf("\nIngrese el nombre del Pokemon a buscar: ");
	getchar();
	fscanf(stdin, "%24[^\n]s", nombreBuscado);
	convertirEstandar(nombreBuscado);

	pokemonAuxiliar = searchMap(mapaNombre, nombreBuscado); //Buscar al primer pokemon con ese nombre

	/*Se recorre el Mapa ya que no se sabe la cantidad de Pokemons
	atrapados con el mismo nombre, en vez de usar la funcion searchMap,
	ya que esta entrega solo uno de los posibles pokemon*/

	while(pokemonAuxiliar != NULL)
	{
		if(strcmp(pokemonAuxiliar->nombrePokemon, nombreBuscado) == 0)
		{
			if(existePokemon == 0) printf(green "\nTodos los %s en su PC:\n" reset, nombreBuscado);
			printf("\n%s ID: %i\n", pokemonAuxiliar->nombrePokemon, pokemonAuxiliar->ident.id);
			printf("Sexo: %s \n", pokemonAuxiliar->sexo);
			printf("Tipo(s): ");

			for(int i = 0; i < pokemonAuxiliar->elementos.cantidadTipos; i++)
				printf("%s ", pokemonAuxiliar->elementos.tipos[i]);

			printf("\nPS: %i PC: %i\n", pokemonAuxiliar->puntos.pSalud, pokemonAuxiliar->puntos.pCombate);
			existePokemon = 1;
		}

		pokemonAuxiliar = nextMap(mapaNombre);
		if(pokemonAuxiliar == NULL) break;
	}

	//Si no se encuentra, se indica que no existe
	if(existePokemon == 0) printf(red"\nNo ha atrapado %s\n"reset, nombreBuscado);
}

/* Buscar la informacion de un pokemon registrado 
en la pokedex a traves de su nombre*/
void buscarPokemonPokedex(HashMap * mapaPokedex)
{
	char nombreBuscado[25]; //Variable para buscar el pokemon
	tipoPokedex * entrada; //Variable para buscar la entrada

	//Ingreso nombre de pokemón
	printf("\nIngrese el pokemon a buscar: ");
	getchar();
	fscanf(stdin, "%24[^\n]s", nombreBuscado);
	convertirEstandar(nombreBuscado);

	//Se busca el pokemon y se ingresa en el auxiliar
	entrada = searchMap(mapaPokedex,nombreBuscado);

	//De no ser el auxiliar NULL, se imprimen sus elementos por pantalla
	if(entrada != NULL)
	{
		printf(green"\nEntrada de Pokedex Encontrada!\n"reset);
		printf("\n%i) %s\n", entrada->ident.idPokedex, entrada->nombrePokemon);
		printf("Region: %s Tipos: ", entrada->region);

		for(int i = 0; i < entrada->elementos.cantidadTipos; i++)
			printf("%s ", entrada->elementos.tipos[i]);

		printf("\nEvolucion Previa: %s, Evolucion Siguiente: %s", entrada->evol.evolPrevia,entrada->evol.evolSiguiente);
		printf("\nVeces Atrapado: %i\n", entrada->cantidadAtrapado);
	}
	else
	{
		printf(red"\nNo posee informacion del pokemon %s\n"reset, nombreBuscado);
	}
}

void mostrarPokedex(HashMap * mapaPokedex)
{
	tipoPokedex * pokemonAuxiliar;
	pokemonAuxiliar = firstMap(mapaPokedex);
	int ultimo = 0;

    ///Inicialización de punteros
	tipoPokedex * menor=(tipoPokedex *) malloc (sizeof(tipoPokedex));
	tipoPokedex * mayor=(tipoPokedex *) malloc (sizeof(tipoPokedex));
	
	mayor->ident.idPokedex = 0;
	menor->ident.idPokedex = 0;
    
	//Se busca el pokemon con mayor id de pokedex y se almacena en mayor
	do
	{
		if(pokemonAuxiliar->ident.idPokedex > mayor->ident.idPokedex) mayor = pokemonAuxiliar;
		pokemonAuxiliar = nextMap(mapaPokedex);

	}while(pokemonAuxiliar != NULL);

    /*Se inicia el ciclo donde se imprimiran y compararan los nodos 
	y su contenido, este, siempre y cuando los parametros
	'menor' y 'mayor' sean distintos*/
	do
	{
		pokemonAuxiliar=firstMap(mapaPokedex);
		menor = mayor;
		/*Recorre el arregloPokemon para buscar que id de pokedex es 
		la menor dentro del parametro de ser menor que el mayor numero y
		mayor que el último numero encontrado,*/
		do
		{
			if(pokemonAuxiliar->ident.idPokedex <= menor->ident.idPokedex && pokemonAuxiliar->ident.idPokedex > ultimo) menor = pokemonAuxiliar;

			pokemonAuxiliar = nextMap(mapaPokedex);

		}while(pokemonAuxiliar!=NULL);

		ultimo = menor->ident.idPokedex;

        //Se imprime el pokemon que cumple las condiciones anteriormente solicitadas
		printf("\n%d)%s\n",menor->ident.idPokedex, menor->nombrePokemon);
		printf("Tipo(s): ");
		for (int i=0; i<menor->elementos.cantidadTipos; i++)
		{
			if(menor->elementos.cantidadTipos-1==i)
			{
				printf("%s\n",menor->elementos.tipos[i]);
			}else
			{
				printf("%s,",menor->elementos.tipos[i]);

			}

		}
		printf("Evolucion Previa: %s \n",menor->evol.evolPrevia);
		printf("Evolucion siguiente: %s\n",menor->evol.evolSiguiente);
		printf("Region: %s ", menor->region);
		printf("Cantidad en posesion: %d\n",menor->cantidadAtrapado);

	}while(menor != mayor);
}

void mostrarPokemonsOrdenadosPC(HashMap * mapaNombre)
{
	tipoPokemon * arregloPokemon[size(mapaNombre)];
    tipoPokemon * auxiliarPokemon = firstMap(mapaNombre);
    int i = 0;

    while(auxiliarPokemon != NULL)
    {
        arregloPokemon[i] = auxiliarPokemon;
        i++;
        auxiliarPokemon = nextMap(mapaNombre);
    }

    tipoPokemon * temp; //Variable temporal.

    for (i = 1; i < size(mapaNombre);i++)
    {
    	for (int j = 0; j < size(mapaNombre)-i ;j++) 
    	{
    	    if (arregloPokemon[j]->puntos.pCombate < arregloPokemon[j+1]->puntos.pCombate)
    	    {
            	temp = arregloPokemon[j];
            	arregloPokemon[j] = arregloPokemon[j+1];
            	arregloPokemon[j+1] = temp;
            }
        }
    }

	printf("\nID NOMBRE  PC\n");
    for(i = 0; i < size(mapaNombre); i++)
        printf("%i %s %i\n", arregloPokemon[i]->ident.id, arregloPokemon[i]->nombrePokemon, arregloPokemon[i]->puntos.pCombate);	
}

//Al liberar un pokemon se tiene que eliminar del almacenamiento, pero no de la pokedex
void liberarPokemon(HashMap * mapaNombre, HashMap * mapaID, HashMap * mapaPokedex)
{
	//se crea variable con el tipoPokemon y int representa la ID del elimainado
	tipoPokemon * poke;
	int eliminado;
	//lo que muestra la funcion por pantalla al momento de seleccionarla
	printf("\nLiberar Pokemon\n");
	printf("Que pokemon desea liberar?\n");
	//se utiliza el getchar para limpiar la basura del tecladp
	getchar();
	//se lee la varible pedida al usuario
	fscanf(stdin,"%i" , &eliminado);
	//se inicializa el mapa para buscar dicha ID
	poke = searchMap(mapaID, &eliminado);
        //se pregunta si poke "existe", diciendo si es distinto de nulo
 	if (poke != NULL)
 	{
		 //se crea un aux para buscar dentro de los mapas
		tipoPokedex * auxPoke = searchMap(mapaPokedex,poke->nombrePokemon);
		auxPoke->cantidadAtrapado--;
		//se utiliza la funcion erase par borra de los mapas nombre y id pero no de pokedex
		eraseMap(mapaNombre, poke->nombrePokemon); 
		eraseMap(mapaID, &poke->ident.id);
		//se manda el mensaje de confirmaciond e que si fue eliminado
		printf(green"\nEl pokemon fue liberado existosamente!\n"reset);
	}
	else
	{
		//si el id no se encontro retornaria diciendo que no se encontro asociado
		printf(red"\nEl ID %i no se encuentra asociado a algun pokemon\n"reset, eliminado);
    }
}

void mostrarPokemonRegion(HashMap * mapaPokedex)
{
	tipoPokedex * entradaAuxiliar = firstMap(mapaPokedex);
	char regionBuscada[25];
	short existePokemon = 0;

    printf("\nIngrese la Region a buscar: ");
	getchar();
	fscanf(stdin,"%24s",regionBuscada);
	convertirEstandar(regionBuscada);
	
	do
	{
		if(strcmp(entradaAuxiliar->region,regionBuscada) == 0 && entradaAuxiliar->cantidadAtrapado != 0)
		{
			if(existePokemon == 0) printf(green "\nPokemons pertenecientes a %s:\n" reset,regionBuscada);
            printf("\n%d)%s\n",entradaAuxiliar->ident.idPokedex,entradaAuxiliar->nombrePokemon);
			printf("Tipo(s): ");

			for(int i = 0;i < entradaAuxiliar->elementos.cantidadTipos;i++)
			{
                if(entradaAuxiliar->elementos.cantidadTipos - 1 == i)
				{
					printf("%s\n",entradaAuxiliar->elementos.tipos[i]);
				}
				else
				{
                    printf("%s, ",entradaAuxiliar->elementos.tipos[i]);
				}
			}

			printf("Evolucion Previa: %s\n",entradaAuxiliar->evol.evolPrevia);
			printf("Evolucion Siguiente: %s\n",entradaAuxiliar->evol.evolSiguiente);
			printf("Cantidad en posesion: %d\n",entradaAuxiliar->cantidadAtrapado);
			existePokemon = 1;
		}
		entradaAuxiliar = nextMap(mapaPokedex);

	}while(entradaAuxiliar != NULL);

	if(existePokemon == 0) printf(red"\nNo hay pokemons de la region %s\n"reset, regionBuscada);
}
