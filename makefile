CC=gcc
CFLAGS=-I.

wave: funciones.o wave.o
	$(CC) -o wave funciones.o wave.o -lpthread -lm -I.
	rm wave.o funciones.o


