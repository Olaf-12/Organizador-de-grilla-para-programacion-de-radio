#include <stdio.h> //Nombres: Jose Torres, Julian Reveron
#include <string.h> //CI Jose Torres: 31.530.657, CI Julian Reveron:32.157.352
#include <stdlib.h> // seccion 7
#include <time.h> 
#include <ctype.h>


#define SEGUNDOS_DIA 90000

//Estructuras para cancion,patrocinador y show
typedef struct{
    int seleccionado;
    char nombre[51];
    int min;
    int seg;
    int rate;
}cancion;

typedef struct{
    int seleccionado;
    char nombre[31];
    int seg;
    int veces;
}patrocinador;

typedef struct{
    int veces;
}auxveces;

typedef struct{
    char nombre[101];
    int min;
    int seg;
    int segmentos;
    int rate;
    int seleccion;
}show;

typedef struct{
    int hora;
    int min;
    int seg;
}reloj;

typedef struct 
{
    char nombre[101];
    int seleccionado;
    int min;
    int seg;
    char tipo;
}elemento;

elemento grilla[SEGUNDOS_DIA];


int aleatorio(int minimo, int maximo);
void leerCanciones(FILE *canciones,cancion *cancionespr);
void leerPublicidad(FILE *publicidad, patrocinador *patrocinantes,auxveces *cunas);
void leerShows(FILE *shows,show *showspr);
int cancionRepetida(int idcancion, char *nombrecan);
void compararCanPorRate(cancion *canciones, int nmrcan);
void organizarGrilla(show *showspr,int nmrshow,cancion *cancionespr,int nmrcan, patrocinador *patrocinantes,int nmrpatrocinantes);
void organizarHorario(show *showspr,int nmrshow,cancion *cancionespr,int nmrcan, patrocinador *patrocinantes,int nmrpatrocinantes);
int meterShow(show *showspr,int nmrshow,cancion *cancionespr,int nmrcan, patrocinador *patrocinantes,int nmrpatrocinantes,int i,int j);
void compararShowPorRate(show *showspr,int nmrshow);

int main() {
    srand(time(NULL));
    ///Entrada de las canciones,shows y publicidad
    FILE *canciones = fopen("canciones.txt","r");
    FILE *shows = fopen("shows.txt","r");
    FILE *publicidad = fopen("publicidad.txt","r");

    
    FILE *semana[7];
    if (canciones == NULL || shows == NULL || publicidad == NULL) {
        perror("Error al abrir el archivo");
        return 1; 
    }
    ///Grillas por dia
    
    semana[0] = fopen("lunes.out","w");
    semana[1] = fopen("martes.out","w");
    semana[2] = fopen("miercoles.out","w");
    semana[3] = fopen("jueves.out","w");
    semana[4] = fopen("viernes.out","w");
    semana[5] = fopen("sabado.out","w");
    semana[6] = fopen("domingo.out","w");
    

    /// Tiempo de la grilla
    reloj tiempo;
    
    int nmrcan;
    int nmrpatrocinantes = 0; 
    int nmrshow;
    
    //obtener numero de canciones,shows y patrocinantes
    char aux;
    fscanf(canciones,"%d",&nmrcan);
    if(nmrcan > 1000){
        printf("Numero de canciones sobrepasa el limite\n");
        exit(0);
    }
    fscanf(shows,"%d",&nmrshow);
    if(nmrshow > 15){
        printf("Numero de shows sobrepasa el limite\n");
        exit(0);
    }
    while(fscanf(publicidad,"%c",&aux) != EOF){
        if(aux == '\n'){
            nmrpatrocinantes++;
        }
    }
    nmrpatrocinantes++;
    rewind(publicidad);
    
    
    cancion cancionespr[nmrcan]; // cancionespr = canciones por rating
    show showspr[nmrshow];  // showspr = shows por rating
    patrocinador patrocinantes[nmrpatrocinantes];
    auxveces cuna[nmrpatrocinantes];
    leerCanciones(canciones,cancionespr); // proc para leer canciones
    leerPublicidad(publicidad,patrocinantes,cuna); // proc para leer patrocinantes
    leerShows(shows,showspr); // proc para leer shows
    compararCanPorRate(cancionespr,nmrcan); // ordenar de mayor a menor las canciones por rate
    compararShowPorRate(showspr,nmrshow);
    
    printf("Cargando grillas para la semana...\n");

    tiempo.hora = 0;
    tiempo.min = 5;
    tiempo.seg = 0;

    for(int f = 0; f < 7;f++){
        
        for(int i = 0; i < SEGUNDOS_DIA;i++){
            grilla[i].seleccionado = 0;
            strcpy(grilla[i].nombre,"\n ");
        }
        for(int i = 0; i < nmrshow;i++){
            showspr[i].seleccion = 0;
        }
        
        for(int i = 0; i < nmrpatrocinantes;i++){
            patrocinantes[i].veces = cuna[i].veces;
        }
        
        organizarHorario(showspr,nmrshow,cancionespr,nmrcan,patrocinantes,nmrpatrocinantes);
        organizarGrilla(showspr, nmrshow, cancionespr,nmrcan, patrocinantes,nmrpatrocinantes);
        int i=0;
        tiempo.seg = 0;
        tiempo.min = 0;
        tiempo.hora = 0;
        
        while(tiempo.hora < 24){
            
            if(grilla[i].seleccionado == 1){
                tiempo.seg = i;
                if(tiempo.seg >= 60){
                    while(tiempo.seg >= 60){
                        tiempo.seg -= 60;
                        tiempo.min++;
                    }
                    tiempo.seg++;
                    if(tiempo.seg >= 60){
                        tiempo.min++;
                    }
                }
                
                if(tiempo.min >= 60){
                    while(tiempo.min >= 60){
                        tiempo.min -= 60;
                        tiempo.hora++;
                        if(tiempo.hora == 24){
                            break;
                        }
                    }
                }
                
                if(tiempo.hora == 24){
                    fprintf(semana[f],"23 59 59 %c %s \n",grilla[i].tipo,grilla[i].nombre); 
                    break;
                }
                
                if(tiempo.hora < 10 && tiempo.min < 10 && tiempo.seg < 10){
                    fprintf(semana[f],"0%d 0%d 0%d %c %s \n",tiempo.hora,tiempo.min,tiempo.seg,grilla[i].tipo,grilla[i].nombre);
                }
                else if(tiempo.hora >= 10 && tiempo.min >= 10 && tiempo.seg >= 10){
                    fprintf(semana[f],"%d %d %d %c %s \n",tiempo.hora,tiempo.min,tiempo.seg,grilla[i].tipo,grilla[i].nombre);
                }
                else if(tiempo.hora < 10 && tiempo.min < 10 && tiempo.seg >= 10){
                    fprintf(semana[f],"0%d 0%d %d %c %s \n",tiempo.hora,tiempo.min,tiempo.seg,grilla[i].tipo,grilla[i].nombre);
                }
                else if(tiempo.hora < 10 && tiempo.min >= 10 && tiempo.seg < 10){
                    fprintf(semana[f],"0%d %d 0%d %c %s \n",tiempo.hora,tiempo.min,tiempo.seg,grilla[i].tipo,grilla[i].nombre);
                }
                else if(tiempo.hora >= 10 && tiempo.min < 10 && tiempo.seg < 10){
                    fprintf(semana[f],"%d 0%d 0%d %c %s \n",tiempo.hora,tiempo.min,tiempo.seg,grilla[i].tipo,grilla[i].nombre);
                }
                else if(tiempo.hora < 10 && tiempo.min >= 10 && tiempo.seg >= 10){
                    fprintf(semana[f],"0%d %d %d %c %s \n",tiempo.hora,tiempo.min,tiempo.seg,grilla[i].tipo,grilla[i].nombre);
                }
                else if(tiempo.hora >= 10 && tiempo.min >= 10 && tiempo.seg < 10){
                    fprintf(semana[f],"%d %d 0%d %c %s \n",tiempo.hora,tiempo.min,tiempo.seg,grilla[i].tipo,grilla[i].nombre);
                }
                else if(tiempo.hora >= 10 && tiempo.min < 10 && tiempo.seg >= 10){
                    fprintf(semana[f],"%d 0%d %d %c %s \n",tiempo.hora,tiempo.min,tiempo.seg,grilla[i].tipo,grilla[i].nombre);
                }

                tiempo.seg = 0;
                tiempo.min = 0;
                tiempo.hora = 0;
            }
            i++;
        }
        
        fclose(semana[f]);
    }
    int opcion1,opcion2,dia;
    
    printf("======<Bienvenido a el programa organizador radial para la semana,seleccione una opcion: >======\n\n");
    while(opcion1 != 4){
        
        printf("1) Presione 1 para consultar la programacion para un dia.-\n\n");
        printf("2) Presione 2 para consultar la programacion para un dia y hora.-\n\n");
        printf("3) Presione 3 para consultar una cancion.-\n\n");
        printf("4) Presione 4 para salir.-\n");
        scanf("%d",&opcion1);

         if(opcion1 == 1){
            printf("Seleccione un dia de la semana: lunes = 0,...domingo = 6\n");
            scanf("%d",&dia);
            char auxname[101];
            if(dia == 0){
                semana[dia] = fopen("lunes.out","r");
            }
            else if(dia == 1){
                semana[dia] = fopen("martes.out","r");
            }
            else if(dia == 2){
                semana[dia] = fopen("miercoles.out","r");
            }
            else if(dia == 3){
                semana[dia] = fopen("jueves.out","r");
            }
            else if(dia == 4){
                semana[dia] = fopen("viernes.out","r");
            }
            else if(dia == 5){
                semana[dia] = fopen("sabado.out","r");
            }
            else if(dia == 6){
                semana[dia] = fopen("domingo.out","r");
            }
            while(fgets(auxname,sizeof(auxname),semana[dia]) != NULL){
                printf("%s",auxname);
            }
            printf("\n");
            printf("Programacion cargada\n\n");   
        }

        else if(opcion1 == 2){
            int hora, min, seg;

            printf("Seleccione un dia de la semana: lunes = 0,...domingo = 6\n");
            scanf("%d",&dia);

            if(dia < 0 || dia > 6){
                printf("Dia invalido\n\n");
                continue;
            }

            printf("Ingrese la hora exacta (hora min seg): ");
            scanf("%d %d %d",&hora,&min,&seg);

            if(hora < 0 || hora > 23 || min < 0 || min > 59 || seg < 0 || seg > 59){
                printf("Hora invalida\n\n");
                continue;
            }

            if(dia == 0){
                semana[dia] = fopen("lunes.out","r");
            }
            else if(dia == 1){
                semana[dia] = fopen("martes.out","r");
            }
            else if(dia == 2){
                semana[dia] = fopen("miercoles.out","r");
            }
            else if(dia == 3){
                semana[dia] = fopen("jueves.out","r");
            }
            else if(dia == 4){
                semana[dia] = fopen("viernes.out","r");
            }
            else if(dia == 5){
                semana[dia] = fopen("sabado.out","r");
            }
            else{
                semana[dia] = fopen("domingo.out","r");
            }

            int momento = hora*3600 + min*60 + seg;

            char lineaActual[150];
            char lineaSiguiente[150];

            int h1,m1,s1;
            int h2,m2,s2;

            int encontrado = 0;

            if(fgets(lineaActual,sizeof(lineaActual),semana[dia]) != NULL){

                while(fgets(lineaSiguiente,sizeof(lineaSiguiente),semana[dia]) != NULL){

                    sscanf(lineaActual,"%d %d %d",&h1,&m1,&s1);
                    sscanf(lineaSiguiente,"%d %d %d",&h2,&m2,&s2);

                    int inicioActual = h1*3600 + m1*60 + s1;
                    int inicioSiguiente = h2*3600 + m2*60 + s2;

                    if(momento >= inicioActual && momento < inicioSiguiente){
                        printf("\nElemento en reproduccion:\n");
                        printf("%s\n",lineaActual);
                        encontrado = 1;
                        break;
                    }

                    strcpy(lineaActual,lineaSiguiente);
                }

                if(!encontrado){
                    sscanf(lineaActual,"%d %d %d",&h1,&m1,&s1);

                    if(momento >= (h1*3600 + m1*60 + s1)){
                        printf("\nElemento en reproduccion:\n");
                        printf("%s\n",lineaActual);
                        encontrado = 1;
                    }
                }
            }

            if(!encontrado){
                printf("No se encontro programacion para ese momento.\n");
            }

            fclose(semana[dia]);
            printf("\nConsulta completada.\n\n");
        }
        else if(opcion1 == 3){
            char auxname[101];
            char auxname2[101];
            char cadena[101];
            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            printf("Escriba el nombre de la cancion: ");
            fgets(auxname,sizeof(auxname),stdin);
            auxname[strlen(auxname) - 1] = '\0';
            strcpy(auxname2,auxname);
            for(int h = 0; h < strlen(auxname);h++){
                auxname[h] = tolower(auxname[h]);
            }
            printf("\n\n");
            for(int dia = 0; dia < 7;dia++){
                if(dia == 0){
                    printf("<------------------------------------------------------------->\n\n");
                    printf("Dia lunes: \n");
                    semana[dia] = fopen("lunes.out","r");
                }
                else if(dia == 1){
                    printf("<------------------------------------------------------------->\n\n");
                    printf("Dia martes: \n");
                    semana[dia] = fopen("martes.out","r");
                }
                else if(dia == 2){
                    printf("<------------------------------------------------------------->\n\n");
                    printf("Dia miercoles: \n");
                    semana[dia] = fopen("miercoles.out","r");
                }
                else if(dia == 3){
                    printf("<------------------------------------------------------------->\n\n");
                    printf("Dia jueves: \n");
                    semana[dia] = fopen("jueves.out","r");
                }
                else if(dia == 4){
                    printf("<------------------------------------------------------------->\n\n");
                    printf("Dia viernes: \n");
                    semana[dia] = fopen("viernes.out","r");
                }
                else if(dia == 5){
                    printf("<------------------------------------------------------------->\n\n");
                    printf("Dia sabado: \n");
                    semana[dia] = fopen("sabado.out","r");
                }
                else if(dia == 6){
                    printf("<------------------------------------------------------------->\n\n");
                    printf("Dia domingo: \n");
                    semana[dia] = fopen("domingo.out","r");
                }
            
                int ban = 0, i = 0,seg1 = 0,min1 = 0, hora1 = 0;
                while(fgets(cadena,sizeof(cadena),semana[dia]) != NULL){
                    for(int h = 0; h < strlen(cadena);h++){
                        cadena[h] = tolower(cadena[h]);
                    }
                    seg1 = i;
                    if(seg1 >= 60){
                        while(seg1 >= 60){
                            seg1 -= 60;
                            min1++;
                        }
                    }
                    if(min1 >= 60){
                        while(min1 >= 60){
                            min1 -= 60;
                            hora1++;
                            if(hora1 == 24){
                                break;
                            }
                        }
                    }
                    if(strstr(cadena,auxname) != NULL){
                        printf("La cancion %s se reproducira en la hora: %d ,minuto: %d ,segundo: %d\n",auxname2,hora1,min1,seg1);
                        ban = 1;
                    }
                    i++;
                }
                if(ban == 0){
                    printf("No se encontro la cancion para este dia \n");
                }
                printf("\n\n");
                
                fclose(semana[dia]);
            }
            printf("<------------------------------------------------------------->\n\n");
        }
        else if(opcion1 == 4){
            printf("Consulta finalizada, hasta pronto.");
            break;
        }
        else{
            printf("Opcion invalida\n");
        }
    }
    return 0;
    
}
//Organiza horarios para el dia
void organizarGrilla(show *showspr,int nmrshow,cancion *cancionespr,int nmrcan, patrocinador *patrocinantes,int nmrpatrocinantes){
    int i = 0,b1 = 0,b2 = 0,b3 = 0,hor1 = 9*3600,hor2 = 14*3600,hor3 = 19*3600;
    while(i < SEGUNDOS_DIA){
        if(i >= 7*3600 && i <= 9*3600 && b1 == 0){
            for(int h = 12*3600 - 1; h > 7*3600;h--){
                if(grilla[h].seleccionado == 1){
                    hor1 = h + grilla[h].seg*60 + grilla[h].seg;
                    i = hor1;
                    b1 = 1;
                    break;  
                }
            }
        }
        if(i >= 12*3600 && i <= 14*3600 && b2 == 0){
            for(int h = 18*3600 - 1; h > 12*3600;h--){
                if(grilla[h].seleccionado == 1){
                    hor2 = h + grilla[h].seg*60 + grilla[h].seg;
                    i = hor2;
                    b2 = 1;
                    break;  
                }
            }
        }
        if(i >= 18*3600 && i <= 19*3600 && b3 == 0){
            for(int h = SEGUNDOS_DIA; h > 18*3600;h--){
                if(grilla[h].seleccionado == 1){
                    hor3 = h + grilla[h].seg*60 + grilla[h].seg;
                    i = hor3;
                    b3 = 1;
                    break;  
                }
            }
        }
        if(grilla[i].seleccionado == 0){
            int selec = aleatorio(1,3);
            if(selec == 1){
                int ran = aleatorio(0,nmrcan - 1);
                    if(cancionRepetida(i,cancionespr[ran].nombre) == 1){
                        for(int x = 0;x < nmrcan;x++){
                            if(cancionRepetida(i,cancionespr[x].nombre) != 1){
                                int con = i + cancionespr[x].min*60 + cancionespr[x].seg;
                                if(( con < 7*3600 )|| (con >= hor1 && con < 12*3600 )||( con >= hor2  && con < 18*3600) || (con >= hor3 )){
                                    strcpy(grilla[i].nombre,cancionespr[x].nombre);
                                    grilla[i].min = cancionespr[x].min;
                                    grilla[i].seg = cancionespr[x].seg;
                                    grilla[i].tipo = 'C';
                                    grilla[i].seleccionado = 1;
                                    i += (cancionespr[x].min * 60) + cancionespr[x].seg;
                                    break;
                                }
                                else{
                                    selec = 3;
                                }
                            }
                        if(x == nmrcan - 1 && cancionRepetida(i,cancionespr[x].nombre) == 1){
                            selec = 3;
                            break;
                        }
                    }
                }
                else{
                    int con = i + (cancionespr[ran].min * 60) + cancionespr[ran].seg;
                    if(( con < 7*3600 )|| (con >= hor1 && con < 12*3600 )||( con >= hor2  && con < 18*3600) || (con >= hor3 )){
                        strcpy(grilla[i].nombre,cancionespr[ran].nombre);
                        grilla[i].min = cancionespr[ran].min;
                        grilla[i].seg = cancionespr[ran].seg;
                        grilla[i].tipo = 'C';
                        grilla[i].seleccionado = 1;
                        i += (cancionespr[ran].min * 60) + cancionespr[ran].seg;
                    }
                    else{
                        selec = 3;
                    }
                }
        }
        if(selec == 2){
            int ran  = aleatorio(0,nmrshow - 1);
            int ban = 0,max = 0;
            for(int o = 0; o < nmrcan;o++){
                if(cancionespr[o].min*60 + cancionespr[o].seg > max){
                    max = cancionespr[o].min*60 + cancionespr[o].seg;
                }
            }
            int con = i + showspr[ran].segmentos*((max + (showspr[ran].min)*60) + showspr[ran].seg);
            if((con >= 7*3600 && con <= hor1) || (con >= 12*3600 && con <= hor2) || (con >= 18*3600 && con <= hor3)){
                selec = 3;
                ban = 1;
            }
            if(ban == 0){
                if(showspr[ran].seleccion != 1){
                    i = meterShow(showspr,nmrshow,cancionespr,nmrcan,patrocinantes,nmrpatrocinantes,i,ran);
                    showspr[ran].seleccion = 1;
                }
            }
        }
        
        if(selec == 3){
            int ran = aleatorio(0,nmrpatrocinantes - 1);
            int con = i + patrocinantes[ran].seg;
            if( ( con < 7*3600 )|| (con >= hor1  && con < 12*3600 )||( con >= hor2  && con < 18*3600) || (con >= hor3)){
                if(patrocinantes[ran].veces > 0){
                    int aux = i + patrocinantes[ran].seg*patrocinantes[ran].veces;
                    if(( aux < 7*3600 )|| (aux >= hor1  && aux < 12*3600 )||( aux >= hor2  && aux < 18*3600) || (aux >= hor3 )){
                        while(patrocinantes[ran].veces > 0){
                            grilla[i].seg = patrocinantes[ran].seg;
                            strcpy(grilla[i].nombre,patrocinantes[ran].nombre);
                            grilla[i].tipo = 'P';
                            patrocinantes[ran].veces--;
                            grilla[i].seleccionado = 1;
                            i += patrocinantes[ran].seg;
                        }
                    }
                }else{
                    grilla[i].seg = patrocinantes[ran].seg;
                    strcpy(grilla[i].nombre,patrocinantes[ran].nombre);
                    grilla[i].tipo = 'P';
                    patrocinantes[ran].veces--;
                    grilla[i].seleccionado = 1;
                    i += patrocinantes[ran].seg;
                }
            }
                else if(con >= 7*3600 && con < hor1){
                    i = 7*3600;
                }
                else if(con >= 12*3600 && con < hor2){
                    i = 12*3600;
                }
                else if(con >= 18*3600 && con < hor3){
                    i = 18*3600;
                }
            } 
        }
    }
  
}
//funcion para verificar si se puede introducir un show en una hora especifica
int meterShow(show *showspr,int nmrshow,cancion *cancionespr,int nmrcan, patrocinador *patrocinantes,int nmrpatrocinantes,int i,int j){
    for(int h = 0; h < showspr[j].segmentos;h++){
        strcpy(grilla[i].nombre,showspr[j].nombre);
        grilla[i].min = showspr[j].min;
        grilla[i].seg = showspr[j].seg;
        grilla[i].tipo = 'S';
        grilla[i].seleccionado = 1;
        i += (showspr[j].min*60) + showspr[j].seg;
        int al = aleatorio(0,1);
        if(al == 0){
        int ran = aleatorio(0,nmrcan - 1);
        if(cancionRepetida(i,cancionespr[ran].nombre) == 1){
            for(int x = 0;x < nmrcan;x++){
                if(cancionRepetida(i,cancionespr[x].nombre) != 1){
                    strcpy(grilla[i].nombre,cancionespr[x].nombre);
                    grilla[i].min = cancionespr[x].min;
                    grilla[i].seg = cancionespr[x].seg;
                    grilla[i].tipo = 'C';
                    grilla[i].seleccionado = 1;
                    i += (cancionespr[ran].min * 60) + cancionespr[ran].seg;
                    break;
                }
                if(x == nmrcan - 1 && cancionRepetida(i,cancionespr[x].nombre) == 1){
                    al = 1;
                    break;
                }
            }
        }
            else{
                strcpy(grilla[i].nombre,cancionespr[ran].nombre);
                grilla[i].min = cancionespr[ran].min;
                grilla[i].seg = cancionespr[ran].seg;
                grilla[i].tipo = 'C';
                grilla[i].seleccionado = 1;
                i += (cancionespr[ran].min * 60) + cancionespr[ran].seg;
            }
        }
        if(al == 1){
            int ran = aleatorio(0,nmrpatrocinantes - 1);
            grilla[i].seg = patrocinantes[ran].seg;
            strcpy(grilla[i].nombre,patrocinantes[ran].nombre);
            grilla[i].tipo = 'P';
            patrocinantes[ran].veces--;
            grilla[i].seleccionado = 1;
            i += patrocinantes[ran].seg;
        }
    }
    return i;
} 
//Funcion para organizar horarios estelares
void organizarHorario(show *showspr,int nmrshow,cancion *cancionespr,int nmrcan, patrocinador *patrocinantes,int nmrpatrocinantes){
    int i = 7*3600,fin = 9*3600,j = 0;
    for(int g = 0; g < 3;g++){
        while(i <= fin){
            int cont = i + showspr[j].segmentos*(( ( 1 + showspr[j].min)*60) + showspr[j].seg);        
            if(cont > fin && g != 2){
                break;
            }
            if(grilla[i].seleccionado == 0){
                showspr[j].seleccion = 1;
                for(int h = 1; h <= showspr[j].segmentos;h++){
                    strcpy(grilla[i].nombre,showspr[j].nombre);
                    grilla[i].min = showspr[j].min;
                    grilla[i].seg = showspr[j].seg;
                    grilla[i].tipo = 'S';
                    grilla[i].seleccionado = 1;
                    i += (showspr[j].min*60) + showspr[j].seg;
                    int al = aleatorio(0,1);
                    if(al == 0){
                        int ran = aleatorio(0,nmrcan - 1);
                        if(cancionRepetida(i,cancionespr[ran].nombre) != 1){
                            int con = i + abs(showspr[j].segmentos - h)*((( 1 + showspr[j].min)*60) + showspr[j].seg) +(showspr[j].min*60 + showspr[j].seg)+(cancionespr[ran].min * 60) + cancionespr[ran].seg;
                            if((con >= 7*3600 && con <= 9*3600) || (con >= 12*3600 && con <= 14*3600) || (con >= 18*3600 && con <= 19*3600)){
                                strcpy(grilla[i].nombre,cancionespr[ran].nombre);
                                grilla[i].min = cancionespr[ran].min;
                                grilla[i].seg = cancionespr[ran].seg;
                                grilla[i].tipo = 'C';
                                grilla[i].seleccionado = 1;
                                i += (cancionespr[ran].min * 60) + cancionespr[ran].seg;
                                }
                                else{
                                    al = 1;
                                } 
                            }
                            else{
                                    al = 1;
                            }
                        }
                        if(al == 1){
                            int ran = aleatorio(0,nmrpatrocinantes - 1);
                            grilla[i].seg = patrocinantes[ran].seg;
                            strcpy(grilla[i].nombre,patrocinantes[ran].nombre);
                            grilla[i].tipo = 'P';
                            patrocinantes[ran].veces--;
                            grilla[i].seleccionado = 1;
                            i += patrocinantes[ran].seg;
                        }
                    }
                j++;      
            }
        }
        if(g == 0){
            i = 12*3600;
            fin = 14*3600;
        }
        if(g == 1){
            i = 18*3600;
            fin = 19*3600;
        }        
    }
}
//verifica si una cancion esta repetida en un intervalo de 4 horas tanto a la izquierda como a la derecha
int cancionRepetida(int idcancion, char *nombrecan){
    if(idcancion <= 14400){
        for(int i = 0; i <= idcancion + 14400;i++){
            if(strcmp(grilla[i].nombre,nombrecan) == 0){
                return 1;
            }
        }
    }
    else if(idcancion + 14400 >= SEGUNDOS_DIA){
        for(int i = abs(idcancion - 14400); i <= SEGUNDOS_DIA;i++){
            if(strcmp(grilla[i].nombre,nombrecan) == 0){
                return 1;
            }
        }
    }
    else{
        for(int i = abs(idcancion - 14400); i <= idcancion + 14400;i++){
            if(strcmp(grilla[i].nombre,nombrecan) == 0){
                return 1;
            }
        }
    }
    return 0;
}
//Leer canciones, shows,patrocinantes y organizar elementos por rating 
void leerCanciones(FILE *canciones,cancion *cancionespr){
    char linea[100];
    char cadenaaux[50];
    int i = 0;
    int auxm,auxs,auxr,count = 0;
    while(fscanf(canciones,"%s",cadenaaux) != EOF){
        if(isdigit(cadenaaux[0]) != 0&&count == 0){
            auxm = atoi(cadenaaux);
            cancionespr[i].min = auxm;
            count++;
            continue;
        }
        else if(isdigit(cadenaaux[0]) != 0&&count == 1){
            auxs = atoi(cadenaaux);
            cancionespr[i].seg = auxs;
            count++;
            continue;
        }
        else if(isdigit(cadenaaux[0]) != 0&&count == 2){
            auxr = atoi(cadenaaux);
            cancionespr[i].rate = auxr;
            count = 0;
            i++;
            continue;
        }     
    }
    i = -1;
    rewind(canciones);
    while(fgets(linea,sizeof(linea),canciones) != NULL){
        if(i == -1){
            i++;
            continue;
        }
        int pos;
        for(int j = 0; j < strlen(linea);j++){
            if(isdigit(linea[j]) != 0){
                pos = j;
                break;
            }
        }
        strncpy(cancionespr[i].nombre,linea,pos);
        cancionespr[i].nombre[pos - 1] = '\0';
        i++;
    }
}
int aleatorio(int minimo, int maximo){
    int numero_aleatorio = (rand() % (maximo - minimo + 1)) + minimo;
    return numero_aleatorio;
 }

void leerPublicidad(FILE *publicidad, patrocinador *patrocinantes,auxveces *cunas){
    char linea[100];
    char cadenaaux[50];
    int i = 0;
    int auxs,auxv,count = 0;
    while(fscanf(publicidad,"%s",cadenaaux) != EOF){
        if(isdigit(cadenaaux[0]) != 0&&count == 0){
            auxs = atoi(cadenaaux);
            patrocinantes[i].seg = auxs;
            count++;
            continue;
        }

        else if(isdigit(cadenaaux[0]) != 0&&count == 1){
            auxv = atoi(cadenaaux);
            patrocinantes[i].veces = auxv;
            cunas[i].veces = auxv;
            count = 0;
            i++;
            continue;
        }
    }
    i = 0;
    rewind(publicidad);
    while(fgets(linea,sizeof(linea),publicidad) != NULL){
        int pos;
        for(int j = 0; j < strlen(linea);j++){
            if(isdigit(linea[j]) != 0){
                pos = j;
                break;
            }
        }
        strncpy(patrocinantes[i].nombre,linea,pos);
        patrocinantes[i].nombre[pos] = '\0';
        i++;
    }
}

void leerShows(FILE *shows,show *showspr){
    char linea[100];
    char cadenaaux[50];
    int i = 0;
    int auxm,auxs,auxseg,auxp,count = 0;
    while(fscanf(shows,"%s",cadenaaux) != EOF){

        if(isdigit(cadenaaux[0]) != 0 && count == 0){
            auxm = atoi(cadenaaux);
            showspr[i].min = auxm;
            count++;
            continue;
        }

        else if(isdigit(cadenaaux[0]) != 0&&count == 1){
            auxs = atoi(cadenaaux);
            showspr[i].seg = auxs;
            count++;
            continue;
        }

        else if(isdigit(cadenaaux[0]) != 0&&count == 2){
            auxseg = atoi(cadenaaux);
            showspr[i].segmentos = auxseg;
            count++;
            continue;
        }
        else if(isdigit(cadenaaux[0]) != 0&&count == 3){
            auxp = atoi(cadenaaux);
            showspr[i].rate = auxp;
            count = 0;
            i++;
            continue;
        }       
    }
    i = -1;
    rewind(shows);
    while(fgets(linea,sizeof(linea),shows) != NULL){
        if(i == -1){
            i++;
            continue;
        }
        int pos;
        for(int j = 0; j < strlen(linea);j++){
            if(isdigit(linea[j]) != 0){
                pos = j;
                break;
            }
        }
        strncpy(showspr[i].nombre,linea,pos);
        showspr[i].nombre[pos] = '\0';
        i++;
    }
}

void compararCanPorRate(cancion *canciones, int nmrcan) {
     for(int i = 0; i < nmrcan;i++){
        int temp1,temp2,temp3;
        char temp4[51];
        for(int j = i; j < nmrcan;j++){
            
            if(canciones[j].rate > canciones[i].rate){
                temp1 = canciones[i].rate;
                temp2 = canciones[i].min;
                temp3 = canciones[i].seg;
                strcpy(temp4,canciones[i].nombre);
                canciones[i].rate = canciones[j].rate;
                canciones[i].min = canciones[j].min;
                canciones[i].seg = canciones[j].seg;
                strcpy(canciones[i].nombre,canciones[j].nombre);
                canciones[j].rate = temp1;
                canciones[j].min = temp2;
                canciones[j].seg = temp3;
                strcpy(canciones[j].nombre,temp4);

            }
        }
    }
}
void compararShowPorRate(show *showspr,int nmrshow) {
    for(int i = 0; i < nmrshow;i++){
        int temp1,temp2,temp3,temp4;
        char temp5[101];
        for(int j = i; j < nmrshow;j++){
            
            if(showspr[j].rate > showspr[i].rate){
                temp1 = showspr[i].rate;
                temp2 = showspr[i].min;
                temp3 = showspr[i].seg;
                temp4 = showspr[i].segmentos;
                strcpy(temp5,showspr[i].nombre);
                showspr[i].rate = showspr[j].rate;
                showspr[i].min = showspr[j].min;
                showspr[i].seg = showspr[j].seg;
                showspr[i].segmentos = showspr[j].segmentos;
                strcpy(showspr[i].nombre,showspr[j].nombre);
                showspr[j].rate = temp1;
                showspr[j].min = temp2;
                showspr[j].seg = temp3;
                showspr[j].segmentos = temp4;
                strcpy(showspr[j].nombre,temp5);

            }
        }
    }
}