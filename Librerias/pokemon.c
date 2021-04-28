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

void ingresoDeTipos(tipoPokemon * pokemon)
{
	char tipo[25];
	int i;

	//Ingreso de los tipos
	while(i != 25)
	{
		printf("\nIngrese el tipo %i del Pokemon (Ingrese no para avanzar): ", i + 1);
		getchar();
		fscanf(stdin, "%24[^\n]s", tipo);
		convertirEstandar(tipo);
		if(i == 0 && strcmp(tipo, "No") == 0)
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
	tipoPokedex * auxPokedex = searchMap(mapa, pokemon->nombrePokemon);

	if(auxPokedex != NULL)
	{
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

/*Funcion para indicar si hay mas Pokemons de la misma especie*/
short elPokemonExiste(HashMap * mapa, tipoPokedex * entradaPokemon)
{
	if(entradaPokemon == NULL)
	{
		return 3;
	}
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
void ingresarPokemon(HashMap * mapaNombre, HashMap * mapaID, HashMap * mapaPokedex, char * lineaLeida)
{
	tipoPokemon * nuevoPokemon = (tipoPokemon *) malloc (sizeof(tipoPokemon));
	tipoPokedex * nuevoIngreso = (tipoPokedex *) malloc (sizeof(tipoPokedex));
	char * fragmento;

	//Lectura del ID
	fragmento = strtok(lineaLeida, ",");
	nuevoPokemon->ident.id = strtol(fragmento, NULL, 10);
	idOcupado(mapaID, nuevoPokemon);

	//Lectura del nombre
	fragmento = strtok(NULL, ",");
	strcpy(nuevoPokemon->nombrePokemon, fragmento);
	convertirEstandar(nuevoPokemon->nombrePokemon);

	//Lectura de los tipos
	fragmento = strtok(NULL, ",");

	//Se verifica si posee 1 o mas tipos
	if(fragmento[0] == '"')
	{
		//Eliminacion de las primeras comillas
		memmove(fragmento, fragmento + 1, strlen(fragmento));
		strcpy(nuevoPokemon->datos.tipos[0], fragmento);
		convertirEstandar(nuevoPokemon->datos.tipos[0]);
		short largo;
		short cantidad = 1;

		//Reiteracion hasta que no hayan mas tipos
		do
		{
			fragmento = strtok(NULL, ",");
			largo = strlen(fragmento) - 1;
			if(fragmento[largo] != '"')
			{
				strcpy(nuevoPokemon->datos.tipos[cantidad], fragmento);
				convertirEstandar(nuevoPokemon->datos.tipos[cantidad]);
			}
			cantidad++;
		} while (fragmento[largo] != '"');

		//Eliminacion de la comillas finales
		fragmento[largo] = '\0';
		convertirEstandar(nuevoPokemon->datos.tipos[cantidad - 1]);
		strcpy(nuevoPokemon->datos.tipos[cantidad - 1], fragmento);
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

	strcpy(nuevoIngreso->nombrePokemon, nuevoPokemon->nombrePokemon);

	if(elPokemonExiste(mapaPokedex, nuevoIngreso) != 0)
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
		if(size(mapaNombre) >= 100)
		{
			printf("\nHa superado el maximo de Pokemons\n");
			printf("Se ha podido algunos implementar pokemons!\n");
			break;
		}
		//Llamada a funcion para insertar en el mapa
		ingresarPokemon(mapaNombre, mapaID, mapaPokedex,lineaLeida);
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
		fprintf(archivo,"%i,%s,", auxPokemon->ident.id, auxPokemon->nombrePokemon);

		//Copia de tipos, dependiendo de si posee 1 o mas tipos, se colocan comillas
		if(auxPokemon->datos.cantidadTipos != 1) fprintf(archivo, "%c", 34);
		for(int i = 0; i < auxPokemon->datos.cantidadTipos; i++)
		{
				fprintf(archivo, "%s",auxPokemon->datos.tipos[i]);
				if(i < auxPokemon->datos.cantidadTipos - 1) fprintf(archivo, "%c", 44);
		}
		if(auxPokemon->datos.cantidadTipos != 1) fprintf(archivo, "%c", 34);

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
void atraparPokemon(HashMap * mapaNombre, HashMap * mapaID, HashMap * mapaPokedex)
{
	//Creacion de variables a usar
	tipoPokemon * pokemonAtrapado = malloc(sizeof(tipoPokemon));
	tipoPokedex * nuevoIngreso = malloc(sizeof(tipoPokedex));

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

	//Ingreso del sexo del Pokemon
	validarSexo(pokemonAtrapado->sexo);

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

	strcpy(nuevoIngreso->nombrePokemon, pokemonAtrapado->nombrePokemon);

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
	int idBuscado;

	//Busqueda del Pokemon a evolucionar
	printf("\nIngrese el ID del pokemon: ");
	getchar();
	fscanf(stdin, "%i", &idBuscado);

	tipoPokemon * auxPokemon = searchMap(mapaID, &idBuscado);

	//Si existe el ID o no
	if(auxPokemon != NULL)
	{
		//Si posee evolucion o no
		if(strcmp(auxPokemon->evol.evolSiguiente, "No tiene") != 0)
		{
			printf("\nNombre: %s\n", auxPokemon->nombrePokemon);
			printf("PS: %i PC: %i\n", auxPokemon->puntos.pSalud, auxPokemon->puntos.pCombate);

			//Variable para almacenar el nombre del pokemon sin evolucionar
			char auxNombre[25];
			strcpy(auxNombre,auxPokemon->nombrePokemon);

			//Variable para buscar el pokemon en la pokedex y reducir su cantidad
			tipoPokedex * auxPokedex = searchMap(mapaPokedex, auxPokemon->nombrePokemon);
			if(auxPokedex != NULL) auxPokedex->cantidadAtrapado--;

			//Cambios debido a la evolucion
			strcpy(auxPokemon->nombrePokemon, auxPokemon->evol.evolSiguiente);
			strcpy(auxPokemon->evol.evolPrevia, auxNombre);
			auxPokemon->puntos.pSalud = round(auxPokemon->puntos.pSalud * 1.25);
			auxPokemon->puntos.pCombate = round(auxPokemon->puntos.pCombate * 1.50);

			//Variable para encontrar si hay mas de la especie o no
			tipoPokedex * auxPokedex2 = searchMap(mapaPokedex, auxPokemon->nombrePokemon);
			short existe = elPokemonExiste(mapaPokedex, auxPokedex2);

			if(existe == 0) //Si hay mas de su especie, se usa esa evolucion
			{
				strcpy(auxPokemon->evol.evolSiguiente, auxPokedex2->evol.evolSiguiente);
				for(int i = 0; i < auxPokedex2->cantidadAtrapado; i++)
					strcpy(auxPokemon->datos.tipos[i], auxPokedex2->datos.tipos[i]);

				auxPokemon->datos.cantidadTipos = auxPokedex2->datos.cantidadTipos;
			}
			else //Si no, se debe ingresar la evolucion
			{
				printf("\nIngrese la siguiente evolucion de %s: ", auxPokemon->nombrePokemon);
				getchar();
				fscanf(stdin, "%24[^\n]s", auxPokemon->evol.evolSiguiente);
				convertirEstandar(auxPokemon->evol.evolSiguiente);
				ingresoDeTipos(auxPokemon);
				//Creacion de nueva entrada Pokedex
				auxPokedex2 = malloc (sizeof(tipoPokedex));
				strcpy(auxPokedex2->nombrePokemon, auxPokemon->nombrePokemon);
				creacionEntradaPokedex(mapaPokedex, auxPokedex2, auxPokemon);
				auxPokedex2->cantidadAtrapado = 1;
			}

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

	//Si no se encuentra
	if(existePokemon == 0) printf("\nNo ha atrapado %s\n", nombreBuscado);
}

void buscarPokemonPokedex(HashMap * mapa)
{
	char nombreBuscado[25];
	tipoPokedex * entrada;

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
	int ultimo=0;
	tipoPokedex * menor;
	tipoPokedex * mayor;
    ///Inicialización de punteros
	menor=(tipoPokedex*)calloc(1,sizeof(tipoPokedex));
	mayor=(tipoPokedex*)calloc(1,sizeof(tipoPokedex));
	mayor->idents.idPokedex = 0;
	menor->idents.idPokedex = 0;
    //Se busca el pokemon con mayor nid de pokedex y se almacena en mayor
	do
	{
		if(pokemon->idents.idPokedex > mayor->idents.idPokedex) mayor = pokemon;
		pokemon = nextMap(mapa);

	}while(pokemon != NULL);
    //Se inicia el ciclo donde se imprimiran y compararan los nodos y su contenido, este, siempre y cuando los parametros
	//'menor' y 'mayor' sean distintos
	do
	{
		pokemon=firstMap(mapa);
		menor = mayor;
		///Recorre el arreglo para buscar que id de pokedex es la menor dentro del parametro de ser menor que el mayor numero y
		///mayor que el último numero encontrado,
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
		printf("Region: %s", menor->region);
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
	tipoPokedex * pokemon=firstMap(mapa);
	char regionbuscada[25];
    printf("\nIngrese la region a buscar: \n");
	getchar();
	fscanf(stdin,"%24s",regionbuscada);
	convertirEstandar(regionbuscada);
	printf("\n%s:\n",regionbuscada);
	do{
		if(strcmp(pokemon->region,regionbuscada)==0 && pokemon->cantidadAtrapado!=0)
		{
            printf("\n%d)%s\n",pokemon->idents.idPokedex,pokemon->nombrePokemon);
			printf("Tipo(s): ");
			for(int i=0;i<pokemon->datos.cantidadTipos;i++)
			{
                if(pokemon->datos.cantidadTipos-1==i){
					printf("%s\n",pokemon->datos.tipos[i]);
				}else
				{
                    printf("%s, ",pokemon->datos.tipos[i]);
				}
			}
			printf("Evolucion Previa: %s\n",pokemon->evol.evolPrevia);
			printf("Evolucion Siguiente: %s\n",pokemon->evol.evolSiguiente);
			printf("Cantidad en posesion: %d\n",pokemon->cantidadAtrapado);
		}
		pokemon=nextMap(mapa);

	}while(pokemon!=NULL);

}
