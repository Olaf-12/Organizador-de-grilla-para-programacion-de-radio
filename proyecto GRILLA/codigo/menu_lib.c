#include "radio.h"

FILE *abrirDia(int dia){
    switch(dia){
        case 0: return fopen("lunes.out","r");
        case 1: return fopen("martes.out","r");
        case 2: return fopen("miercoles.out","r");
        case 3: return fopen("jueves.out","r");
        case 4: return fopen("viernes.out","r");
        case 5: return fopen("sabado.out","r");
        case 6: return fopen("domingo.out","r");
        default: return NULL;
    }
}

void consultarMomentoEspecifico(void){
    int dia;
    int hora,min,seg;

    printf("Seleccione un dia (lunes=0 ... domingo=6): ");
    scanf("%d",&dia);

    if(dia < 0 || dia > 6){
        printf("Dia invalido.\n\n");
        return;
    }

    printf("Ingrese la hora exacta (hora min seg): ");
    scanf("%d %d %d",&hora,&min,&seg);

    if(hora < 0 || hora > 23 || min < 0 || min > 59 || seg < 0 || seg > 59){
        printf("Hora invalida.\n\n");
        return;
    }

    FILE *archivo = abrirDia(dia);

    if(archivo == NULL){
        printf("No se pudo abrir la programacion.\n\n");
        return;
    }

    int instante = hora*3600 + min*60 + seg;

    char actual[200];
    char siguiente[200];

    int h1,m1,s1;
    int h2,m2,s2;

    int encontrado = 0;

    if(fgets(actual,sizeof(actual),archivo) != NULL){

        while(fgets(siguiente,sizeof(siguiente),archivo) != NULL){

            sscanf(actual,"%d %d %d",&h1,&m1,&s1);
            sscanf(siguiente,"%d %d %d",&h2,&m2,&s2);

            int inicioActual = h1*3600 + m1*60 + s1;
            int inicioSiguiente = h2*3600 + m2*60 + s2;

            if(instante >= inicioActual && instante < inicioSiguiente){
                printf("\nElemento reproduciendose:\n");
                printf("%s",actual);
                encontrado = 1;
                break;
            }

            strcpy(actual,siguiente);
        }

        if(!encontrado){
            sscanf(actual,"%d %d %d",&h1,&m1,&s1);

            int ultimoInicio = h1*3600 + m1*60 + s1;

            if(instante >= ultimoInicio){
                printf("\nElemento reproduciendose:\n");
                printf("%s",actual);
                encontrado = 1;
            }
        }
    }

    if(!encontrado){
        printf("No hay programacion en ese momento.\n");
    }

    fclose(archivo);

    printf("\nConsulta completada.\n\n");
}

const char *nombreDia(int dia){
    switch(dia){
        case 0: return "lunes";
        case 1: return "martes";
        case 2: return "miercoles";
        case 3: return "jueves";
        case 4: return "viernes";
        case 5: return "sabado";
        case 6: return "domingo";
        default: return "";
    }
}

void consultarDiaCompleto(void){
    int dia;
    char linea[200];

    printf("Seleccione un dia (lunes=0 ... domingo=6): ");
    scanf("%d",&dia);

    if(dia < 0 || dia > 6){
        printf("Dia invalido.\n\n");
        return;
    }

    FILE *archivo = abrirDia(dia);

    if(archivo == NULL){
        printf("No se pudo abrir el archivo.\n\n");
        return;
    }

    printf("\nProgramacion de %s:\n\n",nombreDia(dia));

    while(fgets(linea,sizeof(linea),archivo) != NULL){
        printf("%s",linea);
    }

    printf("\n");

    fclose(archivo);
}

void consultarCancion(void){
    char nombreBuscado[101];
    char nombreOriginal[101];
    char linea[200];
    int c;

    while((c = getchar()) != '\n' && c != EOF);

    printf("Escriba el nombre de la cancion: ");
    fgets(nombreBuscado,sizeof(nombreBuscado),stdin);

    nombreBuscado[strcspn(nombreBuscado,"\n")] = '\0';
    strcpy(nombreOriginal,nombreBuscado);

    for(int i = 0; nombreBuscado[i]; i++){
        nombreBuscado[i] = tolower(nombreBuscado[i]);
    }

    printf("\n");

    for(int dia = 0; dia < 7; dia++){

        FILE *archivo = abrirDia(dia);

        if(archivo == NULL){
            continue;
        }

        int encontrado = 0;

        printf("<------------------------------------------>\n");
        printf("Dia %s:\n",nombreDia(dia));

        while(fgets(linea,sizeof(linea),archivo) != NULL){

            char aux[200];
            strcpy(aux,linea);

            for(int i = 0; aux[i]; i++){
                aux[i] = tolower(aux[i]);
            }

            if(strstr(aux,nombreBuscado) != NULL){
                printf("%s",linea);
                encontrado = 1;
            }
        }

        if(!encontrado){
            printf("No se encontro la cancion.\n");
        }

        printf("\n");

        fclose(archivo);
    }
}

void menuPrincipal(void){
    int opcion;

    do{
        printf("====== MENU ======\n");
        printf("1) Consultar programacion de un dia\n");
        printf("2) Consultar programacion en un momento especifico\n");
        printf("3) Consultar una cancion\n");
        printf("4) Salir\n");

        scanf("%d",&opcion);

        switch(opcion){
            case 1:
                consultarDiaCompleto();
                break;

            case 2:
                consultarMomentoEspecifico();
                break;

            case 3:
                consultarCancion();
                break;

            case 4:
                printf("Consulta finalizada.\n");
                break;

            default:
                printf("Opcion invalida.\n\n");
        }

    }while(opcion != 4);
}