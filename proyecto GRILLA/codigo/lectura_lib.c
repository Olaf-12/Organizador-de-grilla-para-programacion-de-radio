#include "radio.h"

void leerCanciones(FILE *canciones, cancion *cancionespr)
{
    char linea[MAX_LINEA];
    int total;
    int i;
    int auxm;
    int auxs;
    int auxr;

    total = 0;
    if (fgets(linea, sizeof(linea), canciones) != NULL) {
        limpiar_espacios(linea);
        total = atoi(linea);
    }

    for (i = 0; i < total && i < MAX_CANCIONES; i++) {
        if (fgets(linea, sizeof(linea), canciones) == NULL) {
            break;
        }
        limpiar_espacios(linea);
        if (!extraer_ultimo_entero(linea, &auxr)) {
            i--;
            continue;
        }
        if (!extraer_ultimo_entero(linea, &auxs)) {
            i--;
            continue;
        }
        if (!extraer_ultimo_entero(linea, &auxm)) {
            i--;
            continue;
        }
        limpiar_espacios(linea);

        cancionespr[i].seleccionado = 0;
        cancionespr[i].min = auxm;
        cancionespr[i].seg = auxs;
        cancionespr[i].rate = auxr;
        strncpy(cancionespr[i].nombre, linea, sizeof(cancionespr[i].nombre) - 1);
        cancionespr[i].nombre[sizeof(cancionespr[i].nombre) - 1] = '\0';
        limpiar_espacios(cancionespr[i].nombre);
    }
}

int aleatorio(int minimo, int maximo)
{
    int numero_aleatorio;

    if (maximo < minimo) {
        return minimo;
    }
    numero_aleatorio = (rand() % (maximo - minimo + 1)) + minimo;
    return numero_aleatorio;
}

void leerPublicidad(FILE *publicidad, patrocinador *patrocinantes, auxveces *cunas)
{
    char linea[MAX_LINEA];
    int i;
    int auxs;
    int auxv;

    i = 0;
    rewind(publicidad);
    while (fgets(linea, sizeof(linea), publicidad) != NULL && i < MAX_PATROCINANTES) {
        limpiar_espacios(linea);
        if (linea_vacia(linea)) {
            continue;
        }
        if (!extraer_ultimo_entero(linea, &auxv)) {
            continue;
        }
        if (!extraer_ultimo_entero(linea, &auxs)) {
            continue;
        }
        limpiar_espacios(linea);

        patrocinantes[i].seleccionado = 0;
        patrocinantes[i].seg = auxs;
        patrocinantes[i].veces = auxv;
        cunas[i].veces = auxv;
        strncpy(patrocinantes[i].nombre, linea, sizeof(patrocinantes[i].nombre) - 1);
        patrocinantes[i].nombre[sizeof(patrocinantes[i].nombre) - 1] = '\0';
        limpiar_espacios(patrocinantes[i].nombre);
        i++;
    }
}

void leerShows(FILE *shows, show *showspr)
{
    char linea[MAX_LINEA];
    int total;
    int i;
    int auxm;
    int auxs;
    int auxseg;
    int auxp;

    total = 0;
    rewind(shows);
    if (fgets(linea, sizeof(linea), shows) != NULL) {
        limpiar_espacios(linea);
        total = atoi(linea);
    }

    for (i = 0; i < total && i < MAX_SHOWS; i++) {
        if (fgets(linea, sizeof(linea), shows) == NULL) {
            break;
        }
        limpiar_espacios(linea);
        if (!extraer_ultimo_entero(linea, &auxp)) {
            i--;
            continue;
        }
        if (!extraer_ultimo_entero(linea, &auxseg)) {
            i--;
            continue;
        }
        if (!extraer_ultimo_entero(linea, &auxs)) {
            i--;
            continue;
        }
        if (!extraer_ultimo_entero(linea, &auxm)) {
            i--;
            continue;
        }
        limpiar_espacios(linea);

        showspr[i].min = auxm;
        showspr[i].seg = auxs;
        showspr[i].segmentos = auxseg;
        showspr[i].rate = auxp;
        showspr[i].seleccion = 0;
        strncpy(showspr[i].nombre, linea, sizeof(showspr[i].nombre) - 1);
        showspr[i].nombre[sizeof(showspr[i].nombre) - 1] = '\0';
        limpiar_espacios(showspr[i].nombre);
    }
}