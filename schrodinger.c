#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define C 1
#define DT 0.1
#define DD 2

float** crearMatriz(int N)
{
	float** matriz = (float**) calloc(N,sizeof(float*));
	if (matriz == NULL)
	{
		printf("No se pudo asignar memoria a la matriz\n");
		return NULL;
	}
	int i;
	for(i=0;i<N;i++)
	{
		matriz[i]=(float*) calloc(N,sizeof(float));
		if (matriz[i] == NULL)
		{
			printf("No se pudo asignar memoria a la matriz\n");
			return NULL;
		}
	}
	return matriz;
}

void printMatriz(float** matriz,int N)
{
	int i,j;
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
		{
			printf("%f ",matriz[i][j]);
		}
		printf("\n");
	}
}

void iniciarMatriz0(float** matriz,int N)
{
	float i,j;
	int h,k;
	for(i=0.4*N;i<0.6*N;i++)
	{
		for(j=0.4*N;j<0.6*N;j++)
		{
			h=round(i);
			k=round(j);
			matriz[h][k]=20;
		}
	}
}


int main()
{
	int N = 10;
	float** H0 = crearMatriz(N);
	float** H1 = crearMatriz(N);
	float** H = crearMatriz(N);
	//printMatriz(H0,N);
	
	iniciarMatriz0(H0,N);
	printMatriz(H0,N);
}
