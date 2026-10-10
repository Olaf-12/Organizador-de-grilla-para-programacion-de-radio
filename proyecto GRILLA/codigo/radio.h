#ifndef radio_h
#define radio_h
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define SEGUNDOS_DIA 86400
#define INICIO_DIA 300
#define MAX_CANCIONES 1000
#define MAX_SHOWS 15
#define MAX_PATROCINANTES 20000
#define MAX_SEGMENTOS_SHOW 100
#define MAX_LINEA 256
#define MAX_NOMBRE 101

typedef struct {
    int seleccionado;
    char nombre[51];
    int min;
    int seg;
    int rate;
} cancion;

typedef struct {
    int seleccionado;
    char nombre[31];
    int seg;
    int veces;
} patrocinador;

typedef struct {
    int veces;
} auxveces;

typedef struct {
    char nombre[101];
    int min;
    int seg;
    int segmentos;
    int rate;
    int seleccion;
} show;

typedef struct {
    int hora;
    int min;
    int seg;
} reloj;

typedef struct {
    char nombre[101];
    int seleccionado;
    int min;
    int seg;
    char tipo;
} elemento;

static elemento grilla[SEGUNDOS_DIA];
static int limite_actual = SEGUNDOS_DIA - 1;

int aleatorio(int minimo, int maximo);
void menuPrincipal(void);
void consultarDiaCompleto(void);
void consultarMomentoEspecifico(void);
void consultarCancion(void);
FILE *abrirDia(int dia);
const char *nombreDia(int dia);
void leerCanciones(FILE *canciones, cancion *cancionespr);
void leerPublicidad(FILE *publicidad, patrocinador *patrocinantes, auxveces *cunas);
void leerShows(FILE *shows, show *showspr);
int cancionRepetida(int idcancion, char *nombrecan);
void compararCanPorRate(cancion *canciones, int nmrcan);
void organizarGrilla(show *showspr, int nmrshow, cancion *cancionespr, int nmrcan, patrocinador *patrocinantes, int nmrpatrocinantes);
void organizarHorario(show *showspr, int nmrshow, cancion *cancionespr, int nmrcan, patrocinador *patrocinantes, int nmrpatrocinantes);
int meterShow(show *showspr, int nmrshow, cancion *cancionespr, int nmrcan, patrocinador *patrocinantes, int nmrpatrocinantes, int i, int j);
void compararShowPorRate(show *showspr, int nmrshow);


#endif