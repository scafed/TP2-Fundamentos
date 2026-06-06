#include <stdio.h>
#include <stdbool.h>
#include "oponente.h"

#define ARCHIVO_BARCOS "barcos.csv"
#define ARCHIVO_REPORTE "reporte.csv"
#define MODO_ESCRITURA "w"

const int CANT_ARGUMENTOS_MIN = 2;
const int ERROR_ARGUMENTOS = -1;
const int ERROR_ARCHIVO_APERTURA = -2;

bool validar_argumentos(int argc) {
    if (argc != CANT_ARGUMENTOS_MIN) {
        printf("ERROR AL EJECUTAR EL PROGRAMA\nSe debe ejecutar el programa con un argumento: ./ejecutable <archivo_barcos> <archivo_reporte>\n");
        return false;
    }
    return true;
}




int main(int argc) {

    if (!validar_argumentos(argc)) {
        return ERROR_ARGUMENTOS;
    }

    FILE* archivo_barcos = fopen(ARCHIVO_BARCOS, MODO_ESCRITURA);
    if (archivo_barcos == NULL) {
        printf("ERROR AL ABRIR EL ARCHIVO DE BARCOS\n");
        return ERROR_ARCHIVO_APERTURA;
    }

    FILE* archivo_reporte = fopen(ARCHIVO_REPORTE, MODO_ESCRITURA);
    if (archivo_reporte == NULL) {
        printf("ERROR AL ABRIR EL ARCHIVO DE REPORTE\n");
        fclose(archivo_barcos);
        return ERROR_ARCHIVO_APERTURA;
    }




    fclose(archivo_barcos);
    fclose(archivo_reporte);

    return 0;
}

