#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define C 1
#define DT 0.1
#define DD 2

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

void copyMatrix(float** dest, float** src,int N)
{
	int i, j;
	for (i = 1; i < N - 1; i++)
	{
		for (j = 1; j < N - 1; j++)
		{
			dest[i][j]=src[i][j];
		}
	}
}

void schrodinger(float** H, float** H1, float** H0, int N, int final)
{
	int t, i, j;
	for (t = 0; t < final; t++)
	{
		for (i = 1; i < N-1; i++)
		{
			for (j = 1; j < N-1; j++)
			{
				H[i][j] = 2 * H1[i][j] - H0[i][j] + pow(C, 2) * pow(DT / DD, 2) * (H1[i + 1][j] + H1[i - 1][j] + H1[i][j - 1] + H1[i][j + 1] - 4 * H1[i][j]);
				//printf("i:%d j:%d",i,j);
			}
		}
		copyMatrix(H0,H1,N);
		copyMatrix(H1,H,N);
	}
}


int main()
{
	int N = 100;
	float** H0 = crearMatriz(N);
	float** H1 = crearMatriz(N);
	float** H = crearMatriz(N);
	//printMatriz(H0,N);

	iniciarMatriz0(H0, N);
	iniciarMatriz1(H1, H0, N);
	printMatriz(H0, N);
	printMatriz(H1, N);

	schrodinger(H,H1,H0,N,300);
	printMatriz(H,N);
}
