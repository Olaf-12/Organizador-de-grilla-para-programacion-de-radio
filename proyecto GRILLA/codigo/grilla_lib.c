#include "radio.h"

void organizarGrilla(show *showspr, int nmrshow, cancion *cancionespr, int nmrcan, patrocinador *patrocinantes, int nmrpatrocinantes)
{
    int tiempo_actual;
    int bandera_7;
    int bandera_12;
    int bandera_18;
    int elegido;
    int i;
    int siguiente;

    tiempo_actual = INICIO_DIA;
    bandera_7 = 0;
    bandera_12 = 0;
    bandera_18 = 0;
    limite_actual = SEGUNDOS_DIA - 1;

    while (tiempo_actual <= limite_actual) {
        if (tiempo_actual >= 7 * 3600 && tiempo_actual < 9 * 3600 && bandera_7 == 0) {
            for (i = 0; i < nmrshow; i++) {
                if (showspr[i].seleccion == 0) {
                    siguiente = meterShow(showspr, nmrshow, cancionespr, nmrcan, patrocinantes, nmrpatrocinantes, tiempo_actual, i);
                    if (siguiente > tiempo_actual) {
                        tiempo_actual = siguiente;
                        bandera_7 = 1;
                        break;
                    }
                }
            }
            if (bandera_7 == 0) {
                bandera_7 = 1;
            }
            continue;
        }

        if (tiempo_actual >= 12 * 3600 && tiempo_actual < 14 * 3600 && bandera_12 == 0) {
            for (i = 0; i < nmrshow; i++) {
                if (showspr[i].seleccion == 0) {
                    siguiente = meterShow(showspr, nmrshow, cancionespr, nmrcan, patrocinantes, nmrpatrocinantes, tiempo_actual, i);
                    if (siguiente > tiempo_actual) {
                        tiempo_actual = siguiente;
                        bandera_12 = 1;
                        break;
                    }
                }
            }
            if (bandera_12 == 0) {
                bandera_12 = 1;
            }
            continue;
        }

        if (tiempo_actual >= 18 * 3600 && tiempo_actual < 19 * 3600 && bandera_18 == 0) {
            for (i = 0; i < nmrshow; i++) {
                if (showspr[i].seleccion == 0) {
                    siguiente = meterShow(showspr, nmrshow, cancionespr, nmrcan, patrocinantes, nmrpatrocinantes, tiempo_actual, i);
                    if (siguiente > tiempo_actual) {
                        tiempo_actual = siguiente;
                        bandera_18 = 1;
                        break;
                    }
                }
            }
            if (bandera_18 == 0) {
                bandera_18 = 1;
            }
            continue;
        }

        elegido = aleatorio(1, 100);

        if (elegido <= 45) {
            if (colocar_patrocinador(&tiempo_actual, patrocinantes, nmrpatrocinantes)) {
                continue;
            }
            if (colocar_cancion(&tiempo_actual, cancionespr, nmrcan)) {
                continue;
            }
            for (i = 0; i < nmrshow; i++) {
                if (showspr[i].seleccion == 0) {
                    siguiente = meterShow(showspr, nmrshow, cancionespr, nmrcan, patrocinantes, nmrpatrocinantes, tiempo_actual, i);
                    if (siguiente > tiempo_actual) {
                        tiempo_actual = siguiente;
                        break;
                    }
                }
            }
            if (siguiente <= tiempo_actual) {
                tiempo_actual++;
            }
        } else if (elegido <= 85) {
            if (colocar_cancion(&tiempo_actual, cancionespr, nmrcan)) {
                continue;
            }
            if (colocar_patrocinador(&tiempo_actual, patrocinantes, nmrpatrocinantes)) {
                continue;
            }
            for (i = 0; i < nmrshow; i++) {
                if (showspr[i].seleccion == 0) {
                    siguiente = meterShow(showspr, nmrshow, cancionespr, nmrcan, patrocinantes, nmrpatrocinantes, tiempo_actual, i);
                    if (siguiente > tiempo_actual) {
                        tiempo_actual = siguiente;
                        break;
                    }
                }
            }
            if (siguiente <= tiempo_actual) {
                tiempo_actual++;
            }
        } else {
            for (i = 0; i < nmrshow; i++) {
                if (showspr[i].seleccion == 0) {
                    siguiente = meterShow(showspr, nmrshow, cancionespr, nmrcan, patrocinantes, nmrpatrocinantes, tiempo_actual, i);
                    if (siguiente > tiempo_actual) {
                        tiempo_actual = siguiente;
                        break;
                    }
                }
            }
            if (siguiente > tiempo_actual) {
                continue;
            }
            if (colocar_patrocinador(&tiempo_actual, patrocinantes, nmrpatrocinantes)) {
                continue;
            }
            if (colocar_cancion(&tiempo_actual, cancionespr, nmrcan)) {
                continue;
            }
            tiempo_actual++;
        }
    }
}

int meterShow(show *showspr, int nmrshow, cancion *cancionespr, int nmrcan, patrocinador *patrocinantes, int nmrpatrocinantes, int i, int j)
{
    int max_segmentos;
    int duracion_show;
    int k;
    int tiempo_sim;
    int tiempo_real;
    int indice_cancion[MAX_SEGMENTOS_SHOW];
    int indice_patrocinador[MAX_SEGMENTOS_SHOW];
    int tipo_intermedio[MAX_SEGMENTOS_SHOW];
    int duracion_intermedio[MAX_SEGMENTOS_SHOW];
    char nombre_intermedio[MAX_SEGMENTOS_SHOW][MAX_NOMBRE];
    int min_intermedio[MAX_SEGMENTOS_SHOW];
    int seg_intermedio[MAX_SEGMENTOS_SHOW];
    int cont_intermedios;
    int elegido;
    int idx;
    int dur;
    int start;

    (void)nmrshow;

    if (i < 0 || i >= SEGUNDOS_DIA) {
        return i;
    }
    if (j < 0 || j >= MAX_SHOWS) {
        return i;
    }
    if (showspr[j].seleccion == 1) {
        return i;
    }
    if (showspr[j].segmentos <= 0 || showspr[j].segmentos > MAX_SEGMENTOS_SHOW) {
        return i;
    }

    duracion_show = showspr[j].min * 60 + showspr[j].seg;
    if (duracion_show <= 0) {
        return i;
    }

    max_segmentos = showspr[j].segmentos;
    cont_intermedios = 0;
    tiempo_sim = i;

    for (k = 0; k < max_segmentos; k++) {
        if (tiempo_sim + duracion_show - 1 > limite_actual) {
            return i;
        }

        tiempo_sim += duracion_show + 1;

        if (k < max_segmentos - 1) {
            elegido = aleatorio(0, 1);
            idx = -1;
            dur = 0;

            if (elegido == 0) {
                if (buscar_cancion_valida(tiempo_sim, cancionespr, nmrcan, &idx)) {
                    dur = cancionespr[idx].min * 60 + cancionespr[idx].seg;
                    tipo_intermedio[cont_intermedios] = 'C';
                    indice_cancion[cont_intermedios] = idx;
                    indice_patrocinador[cont_intermedios] = -1;
                    duracion_intermedio[cont_intermedios] = dur;
                    strncpy(nombre_intermedio[cont_intermedios], cancionespr[idx].nombre, MAX_NOMBRE - 1);
                    nombre_intermedio[cont_intermedios][MAX_NOMBRE - 1] = '\0';
                    min_intermedio[cont_intermedios] = cancionespr[idx].min;
                    seg_intermedio[cont_intermedios] = cancionespr[idx].seg;
                } else if (buscar_patrocinador_valido(tiempo_sim, patrocinantes, nmrpatrocinantes, &idx)) {
                    dur = patrocinantes[idx].seg;
                    tipo_intermedio[cont_intermedios] = 'P';
                    indice_cancion[cont_intermedios] = -1;
                    indice_patrocinador[cont_intermedios] = idx;
                    duracion_intermedio[cont_intermedios] = dur;
                    strncpy(nombre_intermedio[cont_intermedios], patrocinantes[idx].nombre, MAX_NOMBRE - 1);
                    nombre_intermedio[cont_intermedios][MAX_NOMBRE - 1] = '\0';
                    min_intermedio[cont_intermedios] = 0;
                    seg_intermedio[cont_intermedios] = patrocinantes[idx].seg;
                } else {
                    return i;
                }
            } else {
                if (buscar_patrocinador_valido(tiempo_sim, patrocinantes, nmrpatrocinantes, &idx)) {
                    dur = patrocinantes[idx].seg;
                    tipo_intermedio[cont_intermedios] = 'P';
                    indice_cancion[cont_intermedios] = -1;
                    indice_patrocinador[cont_intermedios] = idx;
                    duracion_intermedio[cont_intermedios] = dur;
                    strncpy(nombre_intermedio[cont_intermedios], patrocinantes[idx].nombre, MAX_NOMBRE - 1);
                    nombre_intermedio[cont_intermedios][MAX_NOMBRE - 1] = '\0';
                    min_intermedio[cont_intermedios] = 0;
                    seg_intermedio[cont_intermedios] = patrocinantes[idx].seg;
                } else if (buscar_cancion_valida(tiempo_sim, cancionespr, nmrcan, &idx)) {
                    dur = cancionespr[idx].min * 60 + cancionespr[idx].seg;
                    tipo_intermedio[cont_intermedios] = 'C';
                    indice_cancion[cont_intermedios] = idx;
                    indice_patrocinador[cont_intermedios] = -1;
                    duracion_intermedio[cont_intermedios] = dur;
                    strncpy(nombre_intermedio[cont_intermedios], cancionespr[idx].nombre, MAX_NOMBRE - 1);
                    nombre_intermedio[cont_intermedios][MAX_NOMBRE - 1] = '\0';
                    min_intermedio[cont_intermedios] = cancionespr[idx].min;
                    seg_intermedio[cont_intermedios] = cancionespr[idx].seg;
                } else {
                    return i;
                }
            }

            if (tiempo_sim + duracion_intermedio[cont_intermedios] - 1 > limite_actual) {
                return i;
            }
            tiempo_sim += duracion_intermedio[cont_intermedios] + 1;
            cont_intermedios++;
        }
    }

    tiempo_real = i;
    for (k = 0; k < max_segmentos; k++) {
        poner_evento(tiempo_real, showspr[j].nombre, showspr[j].min, showspr[j].seg, 'S');
        tiempo_real += duracion_show + 1;
        if (k < max_segmentos - 1) {
            if (tipo_intermedio[k] == 'C') {
                poner_evento(tiempo_real, nombre_intermedio[k], min_intermedio[k], seg_intermedio[k], 'C');
                if (indice_cancion[k] >= 0 && indice_cancion[k] < nmrcan) {
                    (void)indice_cancion[k];
                }
            } else {
                poner_evento(tiempo_real, nombre_intermedio[k], 0, seg_intermedio[k], 'P');
                if (indice_patrocinador[k] >= 0 && indice_patrocinador[k] < nmrpatrocinantes) {
                    patrocinantes[indice_patrocinador[k]].veces--;
                }
            }
            tiempo_real += duracion_intermedio[k] + 1;
        }
    }

    showspr[j].seleccion = 1;
    return tiempo_real;
}

int cancionRepetida(int idcancion, char *nombrecan)
{
    int inicio;
    int fin;
    int i;

    if (nombrecan == NULL) {
        return 0;
    }

    inicio = idcancion - 14400;
    fin = idcancion + 14400;
    if (inicio < 0) {
        inicio = 0;
    }
    if (fin >= SEGUNDOS_DIA) {
        fin = SEGUNDOS_DIA - 1;
    }

    for (i = inicio; i <= fin; i++) {
        if (grilla[i].seleccionado == 1) {
            if (strcmp(grilla[i].nombre, nombrecan) == 0) {
                return 1;
            }
        }
    }

    return 0;
}



void compararCanPorRate(cancion *canciones, int nmrcan)
{
    int i;
    int j;
    cancion temp;

    for (i = 0; i < nmrcan - 1; i++) {
        for (j = i + 1; j < nmrcan; j++) {
            if (canciones[j].rate > canciones[i].rate) {
                temp = canciones[i];
                canciones[i] = canciones[j];
                canciones[j] = temp;
            }
        }
    }
}

void compararShowPorRate(show *showspr, int nmrshow)
{
    int i;
    int j;
    show temp;

    for (i = 0; i < nmrshow - 1; i++) {
        for (j = i + 1; j < nmrshow; j++) {
            if (showspr[j].rate > showspr[i].rate) {
                temp = showspr[i];
                showspr[i] = showspr[j];
                showspr[j] = temp;
            }
        }
    }
}

void organizarHorario(show *showspr, int nmrshow, cancion *cancionespr, int nmrcan, patrocinador *patrocinantes, int nmrpatrocinantes)
{
    (void)showspr;
    (void)nmrshow;
    (void)cancionespr;
    (void)nmrcan;
    (void)patrocinantes;
    (void)nmrpatrocinantes;
}