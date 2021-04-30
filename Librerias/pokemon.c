#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "TDA_Mapa/hashmap.h"
#include "Interfaz/interfaz.h"

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
}tipoPuntos;

typedef struct tipoPokemon
{
	char nombrePokemon[25];
	char region[25];
	char sexo[25];
	tipoTipos datos;
	tipoPuntos puntos;
	tipoEvoluciones evol;
	tiposIdentificaciones ident;
}tipoPokemon;

typedef struct tipoPokedex
{
	int cantidadAtrapado;
	char nombrePokemon[25];
	char region[25];
	tipoTipos datos;
	tipoEvoluciones evol;
	tiposIdentificaciones idents;
}tipoPokedex;

/* Funcion para ingresar los tipos del
pokemon en un arreglo de cadenas*/
void ingresoDeTipos(tipoPokemon * pokemon)
{
	char tipo[25];
	int i = 0;

	//Ingreso de los tipos
	while(i != 25)
	{
		//Ingreso del tipo
		printf("\nIngrese el tipo %i del Pokemon (Ingrese no para avanzar): ", i + 1);
		getchar();
		fscanf(stdin, "%24[^\n]s", tipo);
		
		//Convertir de "fuego " a "Fuego"
		convertirEstandar(tipo);

		if(i == 0 && strcmp(tipo, "No") == 0) //Obligar a colocar 1 tipo
		{
			printf("\nEl pokemon debe tener al menos 1 tipo\n");
		}
		else
		{
			if(strcmp("No", tipo) == 0) break;
			strcat(tipo, " ");
			strcpy(pokemon->datos.tipos[i], tipo);
			i++;
		}
	}

	//Cantidad de tipos que posee el pokemon
	pokemon->datos.cantidadTipos = i;
}

/* Funcion para ingresar datos a una variable tipoPokedex
en conjunto de la insercion al mapaPokedex*/
void creacionEntradaPokedex(HashMap * mapaPokedex, tipoPokedex * entrada, tipoPokemon * pokemon)
{
	//Insercion de datos necesarios para el tipoPokedex a traves de un tipoPokemon
	entrada->datos.cantidadTipos = pokemon->datos.cantidadTipos;
	entrada->idents.idPokedex = pokemon->ident.idPokedex;

	for(int i = 0; i < pokemon->datos.cantidadTipos; i++)
		strcpy(entrada->datos.tipos[i], pokemon->datos.tipos[i]);

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
	tipoPokedex * auxPokedex = searchMap(mapa, pokemon->nombrePokemon);

	if(auxPokedex != NULL)
	{
		//Comparar si el numero de la pokedex es el mismo o no
		if(auxPokedex->idents.idPokedex != pokemon->ident.idPokedex)
		{
			printf("\nEl pokemon %s con el ID %i, posee su ID Pokedex erroneo\n",pokemon->nombrePokemon, pokemon->ident.id);
			printf("Por lo tanto no se ingresara a la Pokedex o al PC\n");
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

	if(aux != NULL)
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

		printf("\nEl pokemon %s debera cambiar su ID %i\n", pokemon->nombrePokemon,id);
		printf("Su nuevo ID es %i\n", pokemon->ident.id);
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

/* Funcion que separa los datos del Pokemon
y forma la variable para ser almacenada en el
mapa*/
void lecturaDeInformacion(HashMap * mapaNombre, HashMap * mapaID, HashMap * mapaPokedex, char * lineaLeida)
{
	//Creacion de structs
	tipoPokemon * nuevoPokemon = (tipoPokemon *) malloc (sizeof(tipoPokemon));
	tipoPokedex * nuevoIngreso = (tipoPokedex *) malloc (sizeof(tipoPokedex));
	
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
		strcpy(nuevoPokemon->datos.tipos[0], fragmento);
		convertirEstandar(nuevoPokemon->datos.tipos[0]);
	
		short largo; //Almacenar largo de la cadena
		short cantidad = 1; //Contador

		//Reiteracion que finaliza cuando no posea mas tipos
		do
		{
			fragmento = strtok(NULL, ",");
			largo = strlen(fragmento) - 1;

			if(fragmento[largo] != '"') //Mientras no reconozca las comillas finales
			{
				strcpy(nuevoPokemon->datos.tipos[cantidad], fragmento);
				convertirEstandar(nuevoPokemon->datos.tipos[cantidad]);
			}

			cantidad++;
		} while (fragmento[largo] != '"');

		//Eliminacion de la comillas finales
		fragmento[largo] = '\0';

		strcpy(nuevoPokemon->datos.tipos[cantidad - 1], fragmento);
		convertirEstandar(nuevoPokemon->datos.tipos[cantidad - 1]);
		nuevoPokemon->datos.cantidadTipos = cantidad;
	}
	else
	{
		strcpy(nuevoPokemon->datos.tipos[0], fragmento);
		convertirEstandar(nuevoPokemon->datos.tipos[0]);
		nuevoPokemon->datos.cantidadTipos = 1;
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
	strcpy(nuevoIngreso->nombrePokemon, nuevoPokemon->nombrePokemon);

	/* Si es la primera especie del pokemon
	se creara una entrada en la pokedex*/
	if(elPokemonExiste(mapaPokedex, nuevoIngreso) == 1) 
	{
		creacionEntradaPokedex(mapaPokedex, nuevoIngreso, nuevoPokemon);
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
		printf("\nArchivo NO IMPORTADO!\n");
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
			printf("\nHa superado el maximo de Pokemons\n");
			printf("Se ha podido algunos implementar pokemons!\n");
			break;
		}

		//Llamada a funcion para insertar en el mapa
		lecturaDeInformacion(mapaNombre, mapaID, mapaPokedex,lineaLeida);
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
		printf("\nArchivo NO EXPORTADO!\n");
		return;
	}

	//Ingreso de la primera linea del archivo.csv (informacion de tabla)
	fprintf(archivo, "id,nombre,tipos,pc,ps,sexo,evolucion previa,evolucion posterior,numero pokedex,region\n");

	//Ingreso de la informacion de los pokemons en el mapa
	tipoPokemon * auxPokemon = firstMap(mapa);

	while(auxPokemon != NULL)
	{
		//Ingreso de ID y nombre
		fprintf(archivo,"%i,%s,", auxPokemon->ident.id, auxPokemon->nombrePokemon);

		//Copia de tipos, dependiendo de si posee 1 o mas tipos, se colocan comillas o no
		if(auxPokemon->datos.cantidadTipos != 1) fprintf(archivo, "%c", 34);

		for(int i = 0; i < auxPokemon->datos.cantidadTipos; i++)
		{
			fprintf(archivo, "%s",auxPokemon->datos.tipos[i]);

			//Evitar colocar comas demás
			if(i < auxPokemon->datos.cantidadTipos - 1) fprintf(archivo, "%c", 44);
		}
		
		if(auxPokemon->datos.cantidadTipos != 1) fprintf(archivo, "%c", 34);

		//Ingreso de los demas datos (putnos, evolucion, IDs)
		fprintf(archivo, ",%i,%i,%s",auxPokemon->puntos.pCombate ,auxPokemon->puntos.pSalud, auxPokemon->sexo);
		fprintf(archivo, ",%s,%s",auxPokemon->evol.evolPrevia, auxPokemon->evol.evolSiguiente);
		fprintf(archivo, ",%i,%s\n", auxPokemon->ident.idPokedex, auxPokemon->region);

		//Avance del mapa
		auxPokemon = nextMap(mapa);
	}

	printf("\nArchivo EXPORTADO!\n");
	fclose(archivo);
}

/* Funcion para ingresar la informacion de 1
nuevo pokemon*/
void atraparPokemon(HashMap * mapaNombre, HashMap * mapaID, HashMap * mapaPokedex)
{
	//Creacion de variables a usar
	tipoPokemon * pokemonAtrapado = (tipoPokemon *) malloc (sizeof(tipoPokemon));
	tipoPokedex * nuevoIngreso =(tipoPokedex *) malloc (sizeof(tipoPokedex));

	//Ingreso del ID
	pokemonAtrapado->ident.id = size(mapaNombre) + 1;

	//Ingreso del nombre
	printf("\nIngrese el nombre del Pokemon: ");
	getchar();
	fscanf(stdin, "%39[^\n]s", pokemonAtrapado->nombrePokemon);
	convertirEstandar(pokemonAtrapado->nombrePokemon);

	//Se corrobora que el ID no este repetido
	idOcupado(mapaID, pokemonAtrapado);

	//Ingreso del numero de la Pokedex
	printf("\nIngrese el numero de ubicacion de la Pokedex: ");
	getchar();
	fscanf(stdin, "%i", &pokemonAtrapado->ident.idPokedex);

	//Se corrobora que el ID Pokedex este correcto
	if(idPokedexCorrecto(mapaPokedex, pokemonAtrapado) == 1) return;

	//Ingreso de los tipos del Pokemon
	ingresoDeTipos(pokemonAtrapado);

	//Ingreso de PC y PS
	printf("\nIngrese los PS del Pokemon: ");
	getchar();
	fscanf(stdin, "%i", &pokemonAtrapado->puntos.pSalud);

	printf("\nIngrese los PC del Pokemon: ");
	getchar();
	fscanf(stdin, "%i", &pokemonAtrapado->puntos.pCombate);

	//Ingreso del sexo del Pokemon y validacion
	validarSexo(pokemonAtrapado->sexo);

	//Ingreso de las evoluciones (previa y posterior)
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
	convertirEstandar(pokemonAtrapado->region);

	//Copia de nombre del tipoPokemon al tipoPokedex
	strcpy(nuevoIngreso->nombrePokemon, pokemonAtrapado->nombrePokemon);

	//Revisar si es un nuevo pokemon o uno existente
	if(elPokemonExiste(mapaPokedex, nuevoIngreso) != 0)
	{
		creacionEntradaPokedex(mapaPokedex, nuevoIngreso, pokemonAtrapado);
	}

	//Ingreso al mapa
	insertMap(mapaNombre, pokemonAtrapado->nombrePokemon, pokemonAtrapado);
	insertMap(mapaID, & pokemonAtrapado->ident.id, pokemonAtrapado);
}

void evolucionarPokemon(HashMap * mapaID, HashMap * mapaPokedex)
{
	int idBuscado; //Variable para buscar pokemon

	//Busqueda del Pokemon a evolucionar
	printf("\nIngrese el ID del pokemon: ");
	getchar();
	fscanf(stdin, "%i", &idBuscado);

	tipoPokemon * auxPokemon = searchMap(mapaID, &idBuscado); //Variable con el pokemon buscado

	//Si el ID tiene relacionado un pokemon o no
	if(auxPokemon != NULL)
	{
		//Si posee evolucion o no
		if(strcmp(auxPokemon->evol.evolSiguiente, "No tiene") != 0)
		{
			//Muestra de informacion
			printf("\nNombre: %s\n", auxPokemon->nombrePokemon);
			printf("PS: %i PC: %i\n", auxPokemon->puntos.pSalud, auxPokemon->puntos.pCombate);

			//Variable para almacenar el nombre del pokemon sin evolucionar
			char auxNombre[25];
			strcpy(auxNombre,auxPokemon->nombrePokemon);

			//Variable para buscar el pokemon en la pokedex y reducir su cantidad (si existe)
			tipoPokedex * auxPokedex = searchMap(mapaPokedex, auxPokemon->nombrePokemon);
			if(auxPokedex != NULL) auxPokedex->cantidadAtrapado--;

			//Cambio en el orden de evoluciones
			strcpy(auxPokemon->nombrePokemon, auxPokemon->evol.evolSiguiente);
			strcpy(auxPokemon->evol.evolPrevia, auxNombre);

			//Aumento en los puntos, debido a la evolucion
			auxPokemon->puntos.pSalud = round(auxPokemon->puntos.pSalud * 1.25);
			auxPokemon->puntos.pCombate = round(auxPokemon->puntos.pCombate * 1.50);

			//Variable para encontrar si hay mas de la especie (evolucionada) o no
			tipoPokedex * auxPokedex2 = searchMap(mapaPokedex, auxPokemon->nombrePokemon);
			
			//Si hay mas de su especie, se usa esa evolucion para rellenar datos
			if(elPokemonExiste(mapaPokedex, auxPokedex2) == 0)
			{
				strcpy(auxPokemon->evol.evolSiguiente, auxPokedex2->evol.evolSiguiente); //Copia de evolucion

				for(int i = 0; i < auxPokedex2->cantidadAtrapado; i++) //Copia de tipos
					strcpy(auxPokemon->datos.tipos[i], auxPokedex2->datos.tipos[i]);

				auxPokemon->datos.cantidadTipos = auxPokedex2->datos.cantidadTipos; //Copia de cantidad
			}
			else //Si no, se debe ingresar la evolucion y los tipos
			{
				//Ingreso de la evolucion
				printf("\nIngrese la siguiente evolucion de %s: ", auxPokemon->nombrePokemon);
				getchar();
				fscanf(stdin, "%24[^\n]s", auxPokemon->evol.evolSiguiente);
				convertirEstandar(auxPokemon->evol.evolSiguiente);

				//Ingreso de los tipos del pokemon evolucionado
				ingresoDeTipos(auxPokemon);
				
				//Creacion de nueva entrada Pokedex
				auxPokedex2 = (tipoPokedex *) malloc (sizeof(tipoPokedex));
				strcpy(auxPokedex2->nombrePokemon, auxPokemon->nombrePokemon);
				creacionEntradaPokedex(mapaPokedex, auxPokedex2, auxPokemon);
				auxPokedex2->cantidadAtrapado = 1;
			}

			//Muestra de informacion
			printf("\nNombre: %s\n", auxPokemon->nombrePokemon);
			printf("PS: %i PC: %i\n", auxPokemon->puntos.pSalud, auxPokemon->puntos.pCombate);
		}
		else
		{
			printf("\nEl Pokemon %s no posee evolucion\n", auxPokemon->nombrePokemon);
		}
	}
	else
	{
		printf("\nNo hay pokemon asociado al ID %i\n", idBuscado);
	}
}

void busquedaPorTipo(HashMap * mapa)
{

}

/* Buscar a todos los pokemon con el mismo nombre
y mostrar sus datos de combate*/
void buscarPokemonNombre(HashMap * mapa)
{
	char nombreBuscado[25]; //Variable para almacenar el nombre a buscar
	tipoPokemon * pokemonAuxiliar; //Variable para recorrer el mapa
	short existePokemon = 0; //Variable booleana

	//Ingreso del nombre del Pokemon
	printf("\nIngrese el nombre del Pokemon a buscar: ");
	getchar();
	fscanf(stdin, "%24[^\n]s", nombreBuscado);
	convertirEstandar(nombreBuscado);

	pokemonAuxiliar = searchMap(mapa, nombreBuscado); //Buscar al primer pokemon con ese nombre

	/*Se recorre el Mapa ya que no se sabe la cantidad de Pokemons
	atrapados con el mismo nombre, en vez de usar la funcion searchMap,
	ya que esta entrega solo uno de los posibles pokemon*/
	while(pokemonAuxiliar != NULL)
	{
		if(strcmp(pokemonAuxiliar->nombrePokemon, nombreBuscado) == 0)
		{
			printf("\nNombre: %s Sexo: %s\n", pokemonAuxiliar->nombrePokemon, pokemonAuxiliar->sexo);
			printf("Tipo(s): ");

			for(int i = 0; i < pokemonAuxiliar->datos.cantidadTipos; i++)
				printf("%s", pokemonAuxiliar->datos.tipos[i]);

			printf("\nID: %i Pokedex: %i\n", pokemonAuxiliar->ident.id, pokemonAuxiliar->ident.idPokedex);
			printf("PS: %i PC: %i\n", pokemonAuxiliar->puntos.pSalud, pokemonAuxiliar->puntos.pCombate);
			existePokemon = 1;
		}
		pokemonAuxiliar = nextMap(mapa);
	}

	//Si no se encuentra, se indica que no existe
	if(existePokemon == 0) printf("\nNo ha atrapado %s\n", nombreBuscado);
}

/* Buscar la informacion de un pokemon registrado 
en la pokedex a traves de su nombre*/
void buscarPokemonPokedex(HashMap * mapa)
{
	char nombreBuscado[25]; //Variable para buscar el pokemon
	tipoPokedex * entrada; //Variable para buscar la entrada

	//Ingreso nombre de pokemón
	printf("\nIngrese el pokemon a buscar: ");
	getchar();
	fscanf(stdin, "%24[^\n]s", nombreBuscado);
	convertirEstandar(nombreBuscado);

	//Se busca el pokemon y se ingresa en el auxiliar
	entrada = searchMap(mapa,nombreBuscado);

	//De no ser el auxiliar NULL, se imprimen sus datos por pantalla
	if(entrada != NULL)
	{
		printf("\nNombre: %s Numero Pokedex: %i\n", entrada->nombrePokemon, entrada->idents.idPokedex);
		printf("Region: %sTipos: ", entrada->region);

		for(int i = 0; i < entrada->datos.cantidadTipos; i++)
			printf("%s", entrada->datos.tipos[i]);

		printf("\nEvolucion Previa: %s Evolucion Siguiente: %s", entrada->evol.evolPrevia,entrada->evol.evolSiguiente);
		printf("\nVeces Atrapado: %i\n", entrada->cantidadAtrapado);
	}
	else
	{
		printf("No se encuentra el pokemon ingresado.\n");
	}
}

void mostrarPokedex(HashMap * mapa)
{
	tipoPokedex * pokemon;
	pokemon = firstMap(mapa);
	int ultimo = 0;

    ///Inicialización de punteros
	tipoPokedex * menor=(tipoPokedex *) malloc (sizeof(tipoPokedex));
	tipoPokedex * mayor=(tipoPokedex *) malloc (sizeof(tipoPokedex));
	
	mayor->idents.idPokedex = 0;
	menor->idents.idPokedex = 0;
    
	//Se busca el pokemon con mayor id de pokedex y se almacena en mayor
	do
	{
		if(pokemon->idents.idPokedex > mayor->idents.idPokedex) mayor = pokemon;
		pokemon = nextMap(mapa);

	}while(pokemon != NULL);

    /*Se inicia el ciclo donde se imprimiran y compararan los nodos 
	y su contenido, este, siempre y cuando los parametros
	'menor' y 'mayor' sean distintos*/
	do
	{
		pokemon=firstMap(mapa);
		menor = mayor;
		/*Recorre el arreglo para buscar que id de pokedex es 
		la menor dentro del parametro de ser menor que el mayor numero y
		mayor que el último numero encontrado,*/
		do
		{
			if(pokemon->idents.idPokedex <= menor->idents.idPokedex && pokemon->idents.idPokedex > ultimo) menor = pokemon;

			pokemon = nextMap(mapa);

		}while(pokemon!=NULL);

		ultimo = menor->idents.idPokedex;

        //Se imprime el pokemon que cumple las condiciones anteriormente solicitadas
		printf("\n%d)%s\n",menor->idents.idPokedex, menor->nombrePokemon);
		printf("Tipo(s): ");
		for (int i=0; i<menor->datos.cantidadTipos; i++)
		{
			if(menor->datos.cantidadTipos-1==i)
			{
				printf("%s\n",menor->datos.tipos[i]);
			}else
			{
				printf("%s,",menor->datos.tipos[i]);

			}

		}
		printf("Evolucion Previa: %s \n",menor->evol.evolPrevia);
		printf("Evolucion siguiente: %s\n",menor->evol.evolSiguiente);
		printf("Region: %s ", menor->region);
		printf("Cantidad en posesion: %d\n",menor->datos.cantidadTipos);

	}while(menor != mayor);
}

void mostrarPokemonsOrdenadosPC(HashMap * mapa)
{
//del esteban
}

///al liberar un pokemon se tiene que eliminar del almacenamiento pero no de la pokedex
void liberarPokemon(HashMap * mapaNombre, HashMap * mapaID, HashMap * mapaPokedex)
{
	//del esteban
	/*	
	tipoPokemon * poke;

	int eliminado;

	Printf("¿Que pokemon desea liberar?\n");
	
	getchar();

	fscanf(stdin,"%i" , &eliminado);

	poke = searchMap(mapaID, &eliminado);
        

 if (poke != NULL){
    	tipoPokedex * auxPoke = searchMap(mapaPokedex,eliminado);
      	auxPoke->cantidadAtrapado--;
		eraseMap(mapaNombre, poke->nombrePokemon);
        eraseMap(mapaID, &poke->ident.id);
	 }
  
        else{
           Printf(“El pokemon no se encontro”);
        }

	}

	 */
}

void mostrarPokemonRegion(HashMap * mapa)
{
	tipoPokedex * pokemon = firstMap(mapa);
	char regionBuscada[25];

    printf("\nIngrese la Region a buscar: ");
	getchar();
	fscanf(stdin,"%24s",regionBuscada);
	convertirEstandar(regionBuscada);
	
	printf("\nPokemons pertenecientes a %s:\n",regionBuscada);
	
	do
	{
		if(strcmp(pokemon->region,regionBuscada) == 0 && pokemon->cantidadAtrapado != 0)
		{
            printf("\n%d)%s\n",pokemon->idents.idPokedex,pokemon->nombrePokemon);
			printf("Tipo(s): ");

			for(int i = 0;i < pokemon->datos.cantidadTipos;i++)
			{
                if(pokemon->datos.cantidadTipos - 1 == i)
				{
					printf("%s\n",pokemon->datos.tipos[i]);
				}
				else
				{
                    printf("%s, ",pokemon->datos.tipos[i]);
				}
			}

			printf("Evolucion Previa: %s\n",pokemon->evol.evolPrevia);
			printf("Evolucion Siguiente: %s\n",pokemon->evol.evolSiguiente);
			printf("Cantidad en posesion: %d\n",pokemon->cantidadAtrapado);
		}
		pokemon = nextMap(mapa);

	}while(pokemon != NULL);

}
