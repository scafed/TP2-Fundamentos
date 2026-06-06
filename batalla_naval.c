#include <stdio.h>
#include <stdbool.h>
#include "oponente.h"

#define CANT_ARGUMENTOS 3
#define MODO_LECTURA "r"
#define MODO_ESCRITURA "w"

const int ERROR_ARGUMENTOS = -1;
const int ERROR_ARCHIVO_APERTURA = -2;
const int POS_ARCHIVO_BARCO = 1;
const int POS_ARCHIVO_REPORTE = 2;

// PRE: -
// POS: Devuelve true si la cantidad de argumentos es igual a CANT_ARGUMENTOS, false en caso contrario, y se imprime un mensaje de error.
bool validar_argumentos(int argc) {
    if (argc != CANT_ARGUMENTOS) {
        printf("ERROR AL EJECUTAR EL PROGRAMA\nSe debe ejecutar el programa con un argumento: ./ejecutable <archivo_barcos> <archivo_reporte>\n");
        return false;
    }
    return true;
}

// PRE: El archivo_barcos debe estar abierto para lectura, y debe estar previamente creado con el formato correcto.
// POS: 
void leer_barcos (FILE* archivo_barcos, barco_t barcos_jugador[]) {
    return;
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

