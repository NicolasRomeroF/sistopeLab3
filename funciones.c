#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
#include "funciones.h"
#include "structs.h"

int N;
float** H0;
float** H1;
float** H;
float** Hsalida;
Hebra* hebras;

float** crearMatriz(int N)
{
	float** matriz = (float**) calloc(N, sizeof(float*));
	if (matriz == NULL)
	{
		printf("No se pudo asignar memoria a la matriz\n");
		return NULL;
	}
	int i;
	for (i = 0; i < N; i++)
	{
		matriz[i] = (float*) calloc(N, sizeof(float));
		if (matriz[i] == NULL)
		{
			printf("No se pudo asignar memoria a la matriz\n");
			return NULL;
		}
	}
	return matriz;
}

void printMatriz(float** matriz, int N)
{
	printf("\n----- Matriz -----\n");
	int i, j;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			printf("%f ", matriz[i][j]);
		}
		printf("\n");
	}
	printf("\n----- Fin Matriz -----\n");
}

void iniciarMatriz0(float** H0, int N)
{
	float i, j;
	int h, k;
	for (i = 0.4 * N; i < 0.6 * N; i++)
	{
		for (j = 0.4 * N; j < 0.6 * N; j++)
		{
			h = round(i);
			k = round(j);
			H0[h][k] = 20;
		}
	}
}

void iniciarMatriz1(float** H1, float** H0, int N)
{
	int i, j;
	for (i = 1; i < N - 1; i++)
	{
		for (j = 1; j < N - 1; j++)
		{
			H1[i][j] = H0[i][j] + (pow(C, 2) / 2) * pow(DT / DD, 2) * (H0[i + 1][j] + H0[i - 1][j] + H0[i][j - 1] + H0[i][j + 1] - 4 * H0[i][j]);
		}
	}
}

void copyMatrix(float** dest, float** src, int N)
{
	int i, j;
	for (i = 1; i < N - 1; i++)
	{
		for (j = 1; j < N - 1; j++)
		{
			dest[i][j] = src[i][j];
		}
	}
}

void schrodinger(float** H, float** H1, float** H0, int N, int final)
{
	int t, i, j;
	for (t = 0; t < final; t++)
	{
		for (i = 1; i < N - 1; i++)
		{
			for (j = 1; j < N - 1; j++)
			{
				H[i][j] = 2 * H1[i][j] - H0[i][j] + pow(C, 2) * pow(DT / DD, 2) * (H1[i + 1][j] + H1[i - 1][j] + H1[i][j - 1] + H1[i][j + 1] - 4 * H1[i][j]);
				//printf("i:%d j:%d",i,j);
			}
		}
		copyMatrix(H0, H1, N);
		copyMatrix(H1, H, N);
	}
}

void* waveHebra(void* id)
{
	Hebra* hilo = (Hebra*)id;
	int pos, i, j;
	Position* e = hilo->escribir;
	//printf("Entre a wave hebra: %d\n", hilo->i);
	for (pos = 0; pos < hilo->cantPosiciones; pos++)
	{

		i = e[pos].i;
		j = e[pos].j;

		//printf("pos:%d i: %d j: %d hebra: %d ir: %d jr: %d\n", pos, i, j, hilo->i, hilo->escribir[pos].i, hilo->escribir[pos].j);
		H[i][j] = 2 * H1[i][j] - H0[i][j] + pow(C, 2) * pow(DT / DD, 2) * (H1[i + 1][j] + H1[i - 1][j] + H1[i][j - 1] + H1[i][j + 1] - 4 * H1[i][j]);

	}
	return NULL;
}

void wave(int T, int Hcant, int tSalida)
{
	printf("in\n");
	int iteracion, i, j;
	for (iteracion = 2; iteracion < T; iteracion++)
	{
		for (i = 0; i < Hcant; i++)
		{
			//printf("iteracion: %d hebra: %d\n", iteracion, i);
			pthread_create(&(hebras[i].id), NULL, waveHebra, (void*) (&hebras[i]));
		}
		//printf("join\n");
		for (j = 0; j < Hcant; j++)
		{
			//printf("j: %d\n", j);
			pthread_join(hebras[j].id, NULL);
		}
		//printf("copyMatrix\n");
		copyMatrix(H0, H1, N);
		copyMatrix(H1, H, N);
		if(iteracion==tSalida && tSalida>3)
		{
			copyMatrix(Hsalida,H,N);
		}
	}
}

void asignarPosiciones(int Hcant, int posPorHebra, int N)
{
	int i, j, h, e;
	h = 0;
	e = 0;

	for (i = 1; i < N - 1; i++)
	{
		for (j = 1; j < N - 1; j++)
		{
			//printf("h: %d e: %d\n", h, e);
			hebras[h].escribir[e].i = i;
			hebras[h].escribir[e].j = j;
			e++;
			if (e == posPorHebra && h != Hcant - 1)
			{
				hebras[h].cantPosiciones = e;
				e = 0;
				h++;
			}
		}
	}
	hebras[h].cantPosiciones = e;
}

void printPosicionesHebra(int Hcant)
{
	int i, j, cont;
	cont = 0;
	Hebra h;
	for (i = 0; i < Hcant; i++)
	{
		h = hebras[i];
		//printf("Hebra: %d cant: %d\n", i, h.cantPosiciones);
		for (j = 0; j < h.cantPosiciones; j++)
		{
		//	printf("i:%d j:%d\n", h.escribir[j].i, h.escribir[j].j);
			cont++;
		}
	}
	//printf("cont: %d\n", cont);
}

Hebra* iniciarHebras(int Hcant, int posPorHebra)
{
	Hebra* h = (Hebra*)malloc(sizeof(Hebra) * Hcant);
	if(h == NULL)
	{
		printf("ERROR: No se pudo asignar memoria a hebra\n");
		return NULL;
	}
	int i;
	int extra = 0;
	for (i = 0; i < Hcant; i++)
	{
		if (i == Hcant - 1)
		{
			extra = posPorHebra - 1;
		}
		h[i].escribir = (Position*)malloc(sizeof(Position) * (posPorHebra + extra));
		h[i].i = i;
	}
	return h;
}

void fwriteMatriz(FILE* f, int N)
{
	for (int i = 0; i < N; i++)
	{
		fwrite(Hsalida[i], sizeof(float), N, f);
	}
}

void freadMatriz(float** Hprueba,FILE* f, int N)
{
	for (int i = 0; i < N; i++)
	{
		fread(Hprueba[i], sizeof(float), N, f);
	}
}

int verificarEntradas(int N, int T, int H, int t)
{
	if(N<3)
	{
		printf("ERROR: N debe tener un valor mayor o igual a 3 (act: %d)\n",N);
		return 0;
	}
	if(T<3)
	{
		printf("ERROR: T debe tener un valor mayor o igual a 3\n");
		return 0;
	}
	if(H<1)
	{
		printf("ERROR: H debe tener un valor mayor o igual a 1\n");
		return 0;
	}
	if(t<3 || t>T)
	{
		printf("ADVERTENCIA: t debe tener un valor mayor a 3 y ademas debe ser menor o igual a T.\n No se creara archivo de salida.\n");
	}
	return 1;
}

void freeMatriz(float** M,int N)
{
	int i;
	for(i=0;i<N;i++)
	{
		free(M[i]);
	}
	free(M);
}


int inicio(int argc, char **argv)
{
	/*
	N: tamanio grilla
	T: numero de pasos
	H: numero de hebras
	f: archivo de salida
	t: iteracion de salida
	*/


	//Aqui declaramos las variables a recibir por medio de optarg
	int Nsize;
	int Tcant;
	int Hcant;
	char* fSalida;
	int tSalida;
	int index;
	int c;

	opterr = 0;

	//Con este ciclo se obtienen los argumentos ingresados por consola y se asignan
	//a las variables anteriormente declaradas
	while ((c = getopt (argc, argv, "N:T:H:f:t:")) != -1) {
		switch (c) {
		case 'N':
			sscanf(optarg, "%d", &Nsize);
			break;
		case 'T':
			sscanf(optarg, "%d", &Tcant);
			break;
		case 'H':
			sscanf(optarg, "%d", &Hcant);
			break;
		case 'f':
			fSalida = optarg;
			break;
		case 't':
			sscanf(optarg, "%d", &tSalida);
			break;

		//Caso en que se ingrese una opcion desconocida
		case '?':
			if (optopt == 'N' || optopt == 'T' || optopt == 'H' || optopt == 'd' || optopt == 't') {
				fprintf (stderr, "Opcion -%c requiere un argumento.\n", optopt);
			}
			else if (isprint (optopt)) {
				fprintf (stderr, "Opcion desconocida `-%c'.\n", optopt);
			}
			else {
				fprintf (stderr, "Caracter desconocido `\\x%x'.\n", optopt);

			}
			return 1;
		default:
			return 1;
		}

	}

	//Se muestran por pantalla los datos ingresados.
	printf ("N = %d, T = %d, H = %d, f = %s, t = %d\n", Nsize, Tcant, Hcant, fSalida, tSalida);

	//Se muestra mensaje en caso de ingresar un argumento que no existe
	for (index = optind; index < argc; index++) {
		printf ("Argumento no existente %s\n", argv[index]);
		return -1;
	}

	if(verificarEntradas(Nsize,Tcant,Hcant,tSalida)==0)
	{
		return 0;
	}

	N = Nsize;
	H0 = crearMatriz(N);
	H1 = crearMatriz(N);
	H = crearMatriz(N);
	Hsalida= crearMatriz(N);

	if(H0 == NULL || H1 == NULL || H == NULL || Hsalida == NULL)
	{
		return -1;
	}
	//printMatriz(H0,N);

	iniciarMatriz0(H0, N);
	iniciarMatriz1(H1, H0, N);
	//printMatriz(H0, N);
	//printMatriz(H1, N);

	int posPorHebra = ((N - 2) * (N - 2)) / Hcant;

	hebras = iniciarHebras(Hcant, posPorHebra);
	if(hebras==NULL)
	{
		return -1;
	}
	//printf("-----asignar-----\n");
	asignarPosiciones(Hcant, posPorHebra, N);
	//printf("-----printpos-----\n");
	//printPosicionesHebra(Hcant);
	//printf("-----wave-----\n");
	clock_t start_t, end_t;
	double total_t;

	start_t =clock();
	wave(Tcant, Hcant, tSalida);
	end_t=clock();
	total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
	printf("Tiempo total: %f\n", total_t);
	//printf("CLOCK: %ld",CLOCKS_PER_SEC);

	FILE* salida = fopen(fSalida, "wb");
	fwriteMatriz(salida,N);
	fclose(salida);
	//printf("asdasd\n");
	//schrodinger(H, H1, H0, N, Tcant);
	//printMatriz(Hsalida, N);
	//printMatriz(Hprueba, N);
	freeMatriz(H0,N);
	freeMatriz(H1,N);
	freeMatriz(H,N);
	freeMatriz(Hsalida,N);
}
