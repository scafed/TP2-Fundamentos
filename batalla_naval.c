#include <stdio.h>
#include <stdbool.h>
#include "oponente.h"

#define CANT_ARGUMENTOS 3
#define MODO_LECTURA "r"
#define MODO_ESCRITURA "w"
#define FORMATO_ARCHIVO_BARCO "%d;%d;%c;%d"
#define CANTIDAD_DATOS_POR_LINEA 4
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
const int ERROR_ARCHIVO_BARCOS_VACIO = 5;
const int ERROR_LEER_FORMATO_ARCHIVO_BARCOS = 6;
const int ERROR_BARCO_FUERA_DEL_TABLERO = 7;
const int ERROR_AL_RESERVAR_MEMORIA = -1;
const int OK = 1;
const int POSICION_ARCHIVO_BARCO = 1;
const int POSICION_ARCHIVO_REPORTE = 2;


bool validar_argumentos(int argc) {
    if (argc != CANT_ARGUMENTOS) {
        printf("ERROR AL EJECUTAR EL PROGRAMA\nSe debe ejecutar el programa con un argumento: ./ejecutable <archivo_barcos> <archivo_reporte>\n");
        return false;
    }
    return true;
}

int jugador_crear (FILE* archivo_barcos, barco_t barcos_jugador[]) {
    coordenada_t coordenada = {0, 0}; 
    int largo = 0;
    char direccion = ' ';

    int cant_barcos_actual = 0;
    int datos_leidos = 0;
    bool error_formato_archivo = false;
    bool error_barco_invalido = false;
    bool error_al_reservar_memoria = false;
    
    datos_leidos = fscanf(archivo_barcos, FORMATO_ARCHIVO_BARCO, &coordenada.fila, &coordenada.columna, &direccion, &largo);

    if (datos_leidos == EOF) {
        printf("ERROR AL LEER EL ARCHIVO BARCOS, SE ENCUENTRA VACÍO\n");
        return ERROR_ARCHIVO_BARCOS_VACIO;
    }
    
    while (datos_leidos != EOF && cant_barcos_actual < CANT_BARCOS && !error_formato_archivo && !error_barco_invalido && !error_al_reservar_memoria) {
        if (datos_leidos != CANTIDAD_DATOS_POR_LINEA) {
            error_formato_archivo = true;
        } 
        else {
            if (!es_coordenada_valida(coordenada) || !es_direccion_valida(direccion) || !es_largo_valido(largo)) {
                error_barco_invalido = true;
            }
            else {
                if (crear_barco(&barcos_jugador[cant_barcos_actual], coordenada, direccion, largo) == ERROR_AL_RESERVAR_MEMORIA) {
                    error_al_reservar_memoria = true;
                }
                else {
                    cant_barcos_actual++;
                    if (hay_superposicion_de_barcos(barcos_jugador, cant_barcos_actual)) {
                        error_barco_invalido = true;
                    }
                }
            }
        }
        
        datos_leidos = fscanf(archivo_barcos, FORMATO_ARCHIVO_BARCO, &coordenada.fila, &coordenada.columna, &direccion, &largo);
    }

    if (error_formato_archivo) {
        printf("ERROR AL LEER EL ARCHIVO BARCOS, EL FORMATO DE LOS DATOS ES INVÁLIDO\n");
        return ERROR_LEER_FORMATO_ARCHIVO_BARCOS;
    }

    if (error_barco_invalido) {
        printf("ERROR AL CREAR EL BARCO, SUS COORDENADAS ESTÁN FUERA DEL TABLERO\n");
        return ERROR_BARCO_FUERA_DEL_TABLERO;
    }
    if (error_al_reservar_memoria) {
        printf("ERROR AL RESERVAR MEMORIA PARA EL BARCO\n");
        return ERROR_AL_RESERVAR_MEMORIA;
    }

    return OK;
}

bool es_coordenada_valida(coordenada_t coordenada) {
    if (coordenada.fila < FILA_MIN_TABLERO || coordenada.fila > FILA_MAX_TABLERO || coordenada.columna < COLUMNA_MIN_TABLERO || coordenada.columna > COLUMNA_MAX_TABLERO) {
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

int crear_barco(barco_t *barco, coordenada_t coordenada, char direccion, int largo) {
    int i = 0;
    bool es_coordenada_siguiente_valida = true;
    
    barco->largo = largo;

    barco->posiciones = malloc(largo * sizeof(coordenada_t));
    if (barco->posiciones == NULL) {
        return ERROR_AL_RESERVAR_MEMORIA;
    }

    while (i < barco->largo && es_coordenada_siguiente_valida) {
        barco->posiciones[i].fila = coordenada.fila;
        barco->posiciones[i].columna = coordenada.columna;

        switch (direccion) {
            case DIRECCION_NORTE:
                coordenada.fila--;
                break;
            case DIRECCION_SUR:
                coordenada.fila++;
                break;
            case DIRECCION_ESTE:
                coordenada.columna++;
                break;
            case DIRECCION_OESTE:
                coordenada.columna--;
                break;
        }

        if (!es_coordenada_valida(coordenada)) {
            es_coordenada_siguiente_valida = false;
        }

        i++;
    }

    if (!es_coordenada_siguiente_valida) {
        return ERROR_BARCO_FUERA_DEL_TABLERO;
    }

    return OK;
}

bool hay_superposicion_de_barcos(barco_t barcos_jugador[], int cant_barcos_leidos) {
    int ultimo_barco = cant_barcos_leidos - 1;
    bool hay_superposicion = false;

    int barco_anterior = 0;

    while (barco_anterior < ultimo_barco && !hay_superposicion) {

        int posicion_anterior = 0;

        while (posicion_anterior < barcos_jugador[barco_anterior].largo &&
               !hay_superposicion) {

            int posicion_ultimo = 0;

            while (posicion_ultimo < barcos_jugador[ultimo_barco].largo &&
                   !hay_superposicion) {

                if (barcos_jugador[barco_anterior].posiciones[posicion_anterior].fila ==
                        barcos_jugador[ultimo_barco].posiciones[posicion_ultimo].fila &&
                    barcos_jugador[barco_anterior].posiciones[posicion_anterior].columna ==
                        barcos_jugador[ultimo_barco].posiciones[posicion_ultimo].columna) {

                    hay_superposicion = true;
                }

                posicion_ultimo++;
            }

            posicion_anterior++;
        }

        barco_anterior++;
    }

    return hay_superposicion;
}

void eliminar_barcos_jugador() {

}


int main(int argc, char* argv[]) {

    if (!validar_argumentos(argc)) {
        return ERROR_ARGUMENTOS;
    }

    FILE* archivo_barcos = fopen(argv[POSICION_ARCHIVO_BARCO], MODO_LECTURA);
    if (archivo_barcos == NULL) {
        printf("ERROR AL ABRIR EL ARCHIVO DE BARCOS\n");
        return ERROR_ARCHIVO_APERTURA;
    }

    barco_t barcos_jugador[CANT_BARCOS];
     
    jugador_crear(archivo_barcos, barcos_jugador);

    

    

    FILE* archivo_reporte = fopen(argv[POSICION_ARCHIVO_REPORTE], MODO_ESCRITURA);
    if (archivo_reporte == NULL) {
        printf("ERROR AL ABRIR EL ARCHIVO DE REPORTE\n");
        fclose(archivo_barcos);
        return ERROR_ARCHIVO_APERTURA;
    }




    fclose(archivo_barcos);
    fclose(archivo_reporte);

    return OK;
}

