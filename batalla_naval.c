#include <stdio.h>
#include <stdbool.h>
#include "oponente.h"

#define CANT_ARGUMENTOS 3
#define MODO_LECTURA "r"
#define MODO_ESCRITURA "w"
#define FORMATO_ARCHIVO_BARCO "%d;%d;%c;%d\n"
#define DIRECCION_NORTE 'N'
#define DIRECCION_SUR 'S'
#define DIRECCION_ESTE 'E'
#define DIRECCION_OESTE 'O'
#define LARGO_BARCO_MINIMO 2
#define LARGO_BARCO_MAXIMO 5
#define FILA_MIN_TABLERO 1
#define FILA_MAX_TABLERO 10
#define COLUMNA_MIN_TABLERO 1
#define COLUMNA_MAX_TABLERO 10

const int ERROR_ARGUMENTOS = 4;
const int ERROR_ARCHIVO_APERTURA = 3;
const int POS_ARCHIVO_BARCO = 1;
const int POS_ARCHIVO_REPORTE = 2;


bool validar_argumentos(int argc) {
    if (argc != CANT_ARGUMENTOS) {
        printf("ERROR AL EJECUTAR EL PROGRAMA\nSe debe ejecutar el programa con un argumento: ./ejecutable <archivo_barcos> <archivo_reporte>\n");
        return false;
    }
    return true;
}

void leer_barcos (FILE* archivo_barcos, barco_t barcos_jugador[]) {
    int fila = 0, columna = 0, largo = 0;
    char direccion = ' ';

    int cant_barcos_leidos = 0;
    int datos_leidos = 0;
    bool es_valido = true;

    datos_leidos = fscanf(archivo_barcos, FORMATO_ARCHIVO_BARCO, &fila, &columna, &direccion, &largo);
    
    while (datos_leidos != EOF && es_valido) {

        if (cant_barcos_leidos >= CANT_BARCOS) {
            es_valido = false;
        }

        if (es_valido && !es_coordenada_valida(fila, columna)) {
            es_valido = false;
        }

        if (es_valido && !es_direccion_valida(direccion)) {
            es_valido = false;
        }

        if (es_valido && !es_largo_valido(largo)) {
            es_valido = false;
        }

        if (es_valido) {
            if (!crear_barco(&barcos_jugador[cant_barcos_leidos], fila, columna, direccion, largo)) {
                es_valido = false;
            } else {
                cant_barcos_leidos++;
            }
        }

        datos_leidos = fscanf(archivo_barcos, FORMATO_ARCHIVO_BARCO, &fila, &columna, &direccion, &largo);
    }

    return;
}

bool es_coordenada_valida(int fila, int columna) {
    if (fila < FILA_MIN_TABLERO || fila > FILA_MAX_TABLERO || columna < COLUMNA_MIN_TABLERO || columna > COLUMNA_MAX_TABLERO) {
        return false;
    }
    return true;
}

bool es_direccion_valida(char direccion) {
    if (direccion != DIRECCION_NORTE && direccion != DIRECCION_SUR && direccion != DIRECCION_ESTE && direccion != DIRECCION_OESTE) {
        return false;
    }
    return true;
}

bool es_largo_valido(int largo) {
    if (largo < LARGO_BARCO_MINIMO || largo > LARGO_BARCO_MAXIMO) {
        return false;
    }
    return true;
}

bool crear_barco(barco_t *barco, int fila, int columna, char direccion, int largo) {
    int i = 0;
    bool es_valido = true;
    
    barco->largo = largo;

    barco->posiciones = malloc(largo * sizeof(coordenada_t));
    if (barco->posiciones == NULL) {
        printf("ERROR AL RESERVAR MEMORIA PARA EL BARCO\n");
        return false;
    }

    while (i < largo && es_valido) {
        barco->posiciones[i].fila = fila;
        barco->posiciones[i].columna = columna;

        switch (direccion) {
            case DIRECCION_NORTE:
                fila--;
                break;
            case DIRECCION_SUR:
                fila++;
                break;
            case DIRECCION_ESTE:
                columna++;
                break;
            case DIRECCION_OESTE:
                columna--;
                break;
        }

        if (!es_coordenada_valida(fila, columna)) {
            printf("ERROR AL CREAR EL BARCO, SE SUPERA EL LÍMITE DEL TABLERO\n");
            es_valido = false;
        }

        if (esta_ocupada_por_barco(barco->posiciones, i)) {
            printf("ERROR AL CREAR EL BARCO, LA COORDENADA YA ESTA OCUPADA\n");
            es_valido = false;
        }

        i++;
    }

    if (!es_valido) {
        free(barco->posiciones);
    }

    return es_valido;
}


int main(int argc, char* argv[]) {

    if (!validar_argumentos(argc)) {
        return ERROR_ARGUMENTOS;
    }

    FILE* archivo_barcos = fopen(argv[POS_ARCHIVO_BARCO], MODO_LECTURA);
    if (archivo_barcos == NULL) {
        printf("ERROR AL ABRIR EL ARCHIVO DE BARCOS\n");
        return ERROR_ARCHIVO_APERTURA;
    }

    barco_t barcos_jugador[CANT_BARCOS];

    leer_barcos(archivo_barcos, barcos_jugador);

    validar_barcos(barcos_jugador);

    FILE* archivo_reporte = fopen(argv[POS_ARCHIVO_REPORTE], MODO_ESCRITURA);
    if (archivo_reporte == NULL) {
        printf("ERROR AL ABRIR EL ARCHIVO DE REPORTE\n");
        fclose(archivo_barcos);
        return ERROR_ARCHIVO_APERTURA;
    }




    fclose(archivo_barcos);
    fclose(archivo_reporte);

    return 0;
}

