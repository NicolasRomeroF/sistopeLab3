#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include <pthread.h>

#define C 1
#define DT 0.1
#define DD 2

typedef struct {
	int i;
	int j;
} Position;

typedef struct {
	Position* escribir;
	int cantPosiciones;
	pthread_t id;
	int i;
} Hebra;

#endif