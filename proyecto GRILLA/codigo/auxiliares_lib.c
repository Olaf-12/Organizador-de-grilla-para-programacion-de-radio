#include "radio.h"

static void limpiar_espacios(char *cadena)
{
    size_t len;
    char *inicio;
    char *fin;

    if (cadena == NULL) {
        return;
    }

    len = strlen(cadena);
    while (len > 0 && (cadena[len - 1] == '\n' || cadena[len - 1] == '\r' || isspace((unsigned char)cadena[len - 1]))) {
        cadena[len - 1] = '\0';
        len--;
    }

    inicio = cadena;
    while (*inicio != '\0' && isspace((unsigned char)*inicio)) {
        inicio++;
    }

    if (inicio != cadena) {
        memmove(cadena, inicio, strlen(inicio) + 1);
    }

    len = strlen(cadena);
    if (len == 0) {
        return;
    }

    fin = cadena + len - 1;
    while (fin >= cadena && isspace((unsigned char)*fin)) {
        *fin = '\0';
        if (fin == cadena) {
            break;
        }
        fin--;
    }
}

static int linea_vacia(const char *cadena)
{
    while (*cadena != '\0') {
        if (!isspace((unsigned char)*cadena)) {
            return 0;
        }
        cadena++;
    }
    return 1;
}

static int extraer_ultimo_entero(char *linea, int *valor)
{
    char *fin;
    char *inicio;

    if (linea == NULL || valor == NULL) {
        return 0;
    }

    limpiar_espacios(linea);
    if (linea[0] == '\0') {
        return 0;
    }

    fin = linea + strlen(linea);
    while (fin > linea && isspace((unsigned char)fin[-1])) {
        fin--;
    }
    if (fin <= linea) {
        return 0;
    }

    inicio = fin;
    while (inicio > linea && !isspace((unsigned char)inicio[-1])) {
        inicio--;
    }

    *valor = atoi(inicio);
    while (inicio > linea && isspace((unsigned char)inicio[-1])) {
        inicio--;
    }
    *inicio = '\0';
    limpiar_espacios(linea);
    return 1;
}

static void poner_evento(int indice, const char *nombre, int min, int seg, char tipo)
{
    if (indice < 0 || indice >= SEGUNDOS_DIA) {
        return;
    }

    grilla[indice].seleccionado = 1;
    grilla[indice].min = min;
    grilla[indice].seg = seg;
    grilla[indice].tipo = tipo;
    grilla[indice].nombre[0] = '\0';
    if (nombre != NULL) {
        strncpy(grilla[indice].nombre, nombre, MAX_NOMBRE - 1);
        grilla[indice].nombre[MAX_NOMBRE - 1] = '\0';
    }
}

static void formato_hora(int segundos, char *buffer, size_t tam)
{
    int h;
    int m;
    int s;

    if (buffer == NULL || tam == 0) {
        return;
    }

    if (segundos < 0) {
        segundos = 0;
    }
    if (segundos >= SEGUNDOS_DIA) {
        segundos = SEGUNDOS_DIA - 1;
    }

    h = segundos / 3600;
    m = (segundos % 3600) / 60;
    s = segundos % 60;
    sprintf(buffer, "%02d:%02d:%02d", h, m, s);
}

static int nombre_archivo_dia(int dia, char *salida, size_t tam)
{
    const char *nombres[7] = {
        "grilla_lunes.out",
        "grilla_martes.out",
        "grilla_miercoles.out",
        "grilla_jueves.out",
        "grilla_viernes.out",
        "grilla_sabado.out",
        "grilla_domingo.out"
    };

    if (dia < 0 || dia > 6 || salida == NULL) {
        return 0;
    }

    strncpy(salida, nombres[dia], tam - 1);
    salida[tam - 1] = '\0';
    return 1;
}

static void nombre_dia_texto(int dia, char *salida, size_t tam)
{
    const char *nombres[7] = {
        "lunes",
        "martes",
        "miercoles",
        "jueves",
        "viernes",
        "sabado",
        "domingo"
    };

    if (dia < 0 || dia > 6 || salida == NULL || tam == 0) {
        return;
    }

    strncpy(salida, nombres[dia], tam - 1);
    salida[tam - 1] = '\0';
}

static void convertir_minusculas(char *cadena)
{
    size_t i;

    if (cadena == NULL) {
        return;
    }

    for (i = 0; cadena[i] != '\0'; i++) {
        cadena[i] = (char)tolower((unsigned char)cadena[i]);
    }
}

static int contar_lineas_no_vacias(FILE *archivo)
{
    char linea[MAX_LINEA];
    int contador;

    contador = 0;
    rewind(archivo);
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        limpiar_espacios(linea);
        if (!linea_vacia(linea)) {
            contador++;
        }
    }
    rewind(archivo);
    return contador;
}

static void escribir_salida_diaria(FILE *salida)
{
    int i;

    if (salida == NULL) {
        return;
    }

    for (i = INICIO_DIA; i < SEGUNDOS_DIA; i++) {
        if (grilla[i].seleccionado == 1) {
            fprintf(salida, "%02d %02d %02d %c %s\n",
                    grilla[i].min >= 0 ? (i / 3600) : 0,
                    grilla[i].min >= 0 ? ((i % 3600) / 60) : 0,
                    grilla[i].min >= 0 ? (i % 60) : 0,
                    grilla[i].tipo,
                    grilla[i].nombre);
        }
    }
}

static void reiniciar_grilla_dia(void)
{
    int i;

    for (i = 0; i < SEGUNDOS_DIA; i++) {
        grilla[i].seleccionado = 0;
        grilla[i].min = 0;
        grilla[i].seg = 0;
        grilla[i].tipo = '\0';
        grilla[i].nombre[0] = '\0';
    }
}

static int hay_show_principal(show *showspr, int nmrshow)
{
    int i;

    for (i = 0; i < nmrshow; i++) {
        if (showspr[i].seleccion == 0) {
            return 1;
        }
    }
    return 0;
}

static int elegir_show_disponible(show *showspr, int nmrshow)
{
    int inicio;
    int k;
    int i;

    if (nmrshow <= 0) {
        return -1;
    }

    inicio = aleatorio(0, nmrshow - 1);
    for (k = 0; k < nmrshow; k++) {
        i = (inicio + k) % nmrshow;
        if (showspr[i].seleccion == 0) {
            return i;
        }
    }

    return -1;
}

static int buscar_cancion_valida(int inicio, cancion *cancionespr, int nmrcan, int *indice)
{
    int arranque;
    int k;
    int i;
    int duracion;

    if (nmrcan <= 0) {
        return 0;
    }

    arranque = aleatorio(0, nmrcan - 1);
    for (k = 0; k < nmrcan; k++) {
        i = (arranque + k) % nmrcan;
        duracion = cancionespr[i].min * 60 + cancionespr[i].seg;
        if (duracion <= 0) {
            continue;
        }
        if (cancionRepetida(inicio, cancionespr[i].nombre) == 0) {
            if (inicio + duracion - 1 <= limite_actual) {
                if (indice != NULL) {
                    *indice = i;
                }
                return 1;
            }
        }
    }
    return 0;
}

static int buscar_patrocinador_valido(int inicio, patrocinador *patrocinantes, int nmrpatrocinantes, int *indice)
{
    int arranque;
    int k;
    int i;
    int duracion;

    if (nmrpatrocinantes <= 0) {
        return 0;
    }

    arranque = aleatorio(0, nmrpatrocinantes - 1);
    for (k = 0; k < nmrpatrocinantes; k++) {
        i = (arranque + k) % nmrpatrocinantes;
        if (patrocinantes[i].veces <= 0) {
            continue;
        }
        duracion = patrocinantes[i].seg;
        if (duracion <= 0) {
            continue;
        }
        if (inicio + duracion - 1 <= limite_actual) {
            if (indice != NULL) {
                *indice = i;
            }
            return 1;
        }
    }
    return 0;
}

static int colocar_cancion(int *tiempo_actual, cancion *cancionespr, int nmrcan)
{
    int indice;
    int duracion;

    if (tiempo_actual == NULL) {
        return 0;
    }

    if (buscar_cancion_valida(*tiempo_actual, cancionespr, nmrcan, &indice) == 0) {
        return 0;
    }

    duracion = cancionespr[indice].min * 60 + cancionespr[indice].seg;
    poner_evento(*tiempo_actual, cancionespr[indice].nombre, cancionespr[indice].min, cancionespr[indice].seg, 'C');
    *tiempo_actual += duracion + 1;
    return 1;
}

static int colocar_patrocinador(int *tiempo_actual, patrocinador *patrocinantes, int nmrpatrocinantes)
{
    int indice;
    int duracion;

    if (tiempo_actual == NULL) {
        return 0;
    }

    if (buscar_patrocinador_valido(*tiempo_actual, patrocinantes, nmrpatrocinantes, &indice) == 0) {
        return 0;
    }

    duracion = patrocinantes[indice].seg;
    poner_evento(*tiempo_actual, patrocinantes[indice].nombre, 0, patrocinantes[indice].seg, 'P');
    patrocinantes[indice].veces--;
    *tiempo_actual += duracion + 1;
    return 1;
}

static int es_primer_segundo_de_ventana(int tiempo_actual)
{
    if (tiempo_actual == 7 * 3600) {
        return 1;
    }
    if (tiempo_actual == 12 * 3600) {
        return 1;
    }
    if (tiempo_actual == 18 * 3600) {
        return 1;
    }
    return 0;
}

static int ventana_principal(int tiempo_actual)
{
    if (tiempo_actual >= 7 * 3600 && tiempo_actual < 9 * 3600) {
        return 1;
    }
    if (tiempo_actual >= 12 * 3600 && tiempo_actual < 14 * 3600) {
        return 1;
    }
    if (tiempo_actual >= 18 * 3600 && tiempo_actual < 19 * 3600) {
        return 1;
    }
    return 0;
}