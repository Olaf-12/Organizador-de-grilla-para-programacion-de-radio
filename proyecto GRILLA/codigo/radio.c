#include <stdio.h>
#include "radio.h" // librarias / modulos

/* Nombre: Jose Torres
/* CI Jose Torres: 31.530.657
/* Seccion 7 */


int main(void)
{
    FILE *canciones;
    FILE *shows;
    FILE *publicidad;
    FILE *salida;
    cancion cancionespr[MAX_CANCIONES];
    show showspr[MAX_SHOWS];
    patrocinador patrocinantes[MAX_PATROCINANTES];
    auxveces cuna[MAX_PATROCINANTES];
    FILE *consulta;
    int nmrcan;
    int nmrshow;
    int nmrpatrocinantes;
    int i;
    int f;
    int opcion1;
    int opcion2;
    int dia;
    reloj tiempo;
    
    char nombre_archivo[MAX_LINEA];
    char nombre_dia[MAX_LINEA];

    srand((unsigned int)time(NULL));

    canciones = fopen("canciones.txt", "r");
    shows = fopen("shows.txt", "r");
    publicidad = fopen("publicidad.txt", "r");

    if (canciones == NULL || shows == NULL || publicidad == NULL) {
        perror("Error al abrir el archivo");
        return 1;
    }

    if (fgets(nombre_archivo, sizeof(nombre_archivo), canciones) == NULL) {
        printf("Archivo canciones.txt vacio\n");
        fclose(canciones);
        fclose(shows);
        fclose(publicidad);
        return 1;
    }
    limpiar_espacios(nombre_archivo);
    nmrcan = atoi(nombre_archivo);
    if (nmrcan < 1 || nmrcan > MAX_CANCIONES) {
        printf("Numero de canciones fuera de rango\n");
        fclose(canciones);
        fclose(shows);
        fclose(publicidad);
        return 1;
    }

    if (fgets(nombre_archivo, sizeof(nombre_archivo), shows) == NULL) {
        printf("Archivo shows.txt vacio\n");
        fclose(canciones);
        fclose(shows);
        fclose(publicidad);
        return 1;
    }
    limpiar_espacios(nombre_archivo);
    nmrshow = atoi(nombre_archivo);
    if (nmrshow < 3 || nmrshow > MAX_SHOWS) {
        printf("Numero de shows fuera de rango\n");
        fclose(canciones);
        fclose(shows);
        fclose(publicidad);
        return 1;
    }

    nmrpatrocinantes = contar_lineas_no_vacias(publicidad);
    if (nmrpatrocinantes < 1) {
        printf("No hay patrocinantes\n");
        fclose(canciones);
        fclose(shows);
        fclose(publicidad);
        return 1;
    }
    if (nmrpatrocinantes > MAX_PATROCINANTES) {
        printf("Numero de patrocinantes sobrepasa el limite\n");
        fclose(canciones);
        fclose(shows);
        fclose(publicidad);
        return 1;
    }

    leerCanciones(canciones, cancionespr);
    leerPublicidad(publicidad, patrocinantes, cuna);
    leerShows(shows, showspr);

    compararCanPorRate(cancionespr, nmrcan);
    compararShowPorRate(showspr, nmrshow);

    fclose(canciones);
    fclose(shows);
    fclose(publicidad);

    printf("Cargando grillas para la semana...\n");

    menuPrincipal();

    return 0;
}


