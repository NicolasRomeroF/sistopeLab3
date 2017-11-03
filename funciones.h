#ifndef _FUNCIONES_H_
#define _FUNCIONES_H_

#include <stdio.h>
#include "structs.h"


float** crearMatriz(int N);
void printMatriz(float** matriz, int N);
void iniciarMatriz0(float** H0, int N);
void iniciarMatriz1(float** H1, float** H0, int N);
void copyMatrix(float** dest, float** src, int N);
void schrodinger(float** H, float** H1, float** H0, int N, int final);
void* waveHebra(void* id);
void wave(int T, int Hcant, int tSalida);
void asignarPosiciones(int Hcant, int posPorHebra, int N);
void printPosicionesHebra(int Hcant);
Hebra* iniciarHebras(int Hcant, int posPorHebra);
void fwriteMatriz(FILE* f, int N);
void freadMatriz(float** Hprueba,FILE* f, int N);
int verificarEntradas(int N, int T, int H, int t);
void freeMatriz(float** M,int N);
int inicio(int argc, char **argv);

#endif