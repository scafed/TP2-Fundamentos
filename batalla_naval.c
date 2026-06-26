#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "oponente.h"

#define CANT_ARGUMENTOS 3
#define MODO_LECTURA "r"
#define MODO_ESCRITURA "w"
#define FORMATO_ARCHIVO_BARCO "%d;%d;%c;%d"
#define FORMATO_DISPARO_JUGADOR "%d;%d"
#define CANTIDAD_DATOS_POR_LINEA 4
#define DIRECCION_NORTE 'N'
#define DIRECCION_SUR 'S'
#define DIRECCION_ESTE 'E'
#define DIRECCION_OESTE 'O'
#define FILA_MIN_TABLERO 1
#define FILA_MAX_TABLERO 10
#define COLUMNA_MIN_TABLERO 1
#define COLUMNA_MAX_TABLERO 10
#define CANT_BARCOS 5
#define CANT_BARCOS_LARGO_2 1
#define CANT_BARCOS_LARGO_3 2
#define CANT_BARCOS_LARGO_4 1
#define CANT_BARCOS_LARGO_5 1
#define LARGO_BARCO_2 2
#define LARGO_BARCO_3 3
#define LARGO_BARCO_4 4
#define LARGO_BARCO_5 5
#define LARGO_BARCO_MINIMO 2
#define LARGO_BARCO_MAXIMO 5
#define AGUA 'A'
#define TOCADO 'T'
#define HUNDIDO 'H'
#define VACIO '-'
#define BARCO 'B'
#define ERROR_AL_RESERVAR_MEMORIA -1
#define ERROR_ARGUMENTOS -2
#define ERROR_ARCHIVO_APERTURA -3
#define ERROR_ARCHIVO_BARCOS_VACIO -4
#define ERROR_CANT_BARCOS_ARCHIVO_BARCOS -5
#define ERROR_LEER_FORMATO_ARCHIVO_BARCOS -6
#define ERROR_BARCO_FUERA_DEL_TABLERO -7
#define ERROR_BARCO_SUPERPOSICION -8
#define ERROR_BARCO_COMPOSICION_FLOTA -9
#define ERROR_AL_CREAR_OPONENTE -10
#define OK 0

const int POSICION_ARCHIVO_BARCOS = 1;
const int POSICION_ARCHIVO_REPORTE = 2;

typedef struct estado_barco {
    barco_t* barco;
    bool impactos[5];
} estado_barco_t;

/*
 * PRE: 
 * POST: 
 */
bool validar_argumentos(int argc) {
    return (argc == CANT_ARGUMENTOS);
}

/*
 * PRE: 
 * POST: 
 */
bool es_coordenada_valida(coordenada_t coordenada) {
    return !((coordenada.fila < FILA_MIN_TABLERO) || (coordenada.fila > FILA_MAX_TABLERO) || (coordenada.columna < COLUMNA_MIN_TABLERO) || (coordenada.columna > COLUMNA_MAX_TABLERO));
}

/*
 * PRE: 
 * POST: 
 */
bool es_direccion_valida(char direccion) {
    return !((direccion != DIRECCION_NORTE) && (direccion != DIRECCION_SUR) && (direccion != DIRECCION_ESTE) && (direccion != DIRECCION_OESTE));
}

/*
 * PRE: 
 * POST: 
 */
bool es_largo_valido(int largo) {
    return !((largo < LARGO_BARCO_MINIMO) || (largo > LARGO_BARCO_MAXIMO));
}

/*
 * PRE: 
 * POST: 
 */
int crear_barco(barco_t *barco, coordenada_t coordenada, char direccion, int largo) {
    int i = 0;
    bool es_coordenada_siguiente_valida = true;
    
    barco->largo = largo;

    barco->posiciones = malloc((size_t)largo * sizeof(coordenada_t));
    if (barco->posiciones == NULL) {
        return ERROR_AL_RESERVAR_MEMORIA;
    }

    while (i < barco->largo && es_coordenada_siguiente_valida) {
        if (!es_coordenada_valida(coordenada)) {
            es_coordenada_siguiente_valida = false;
        }
        else {
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
        }

        i++;
    }

    if (!es_coordenada_siguiente_valida) {
        free(barco->posiciones);
        return ERROR_BARCO_FUERA_DEL_TABLERO;
    }

    return OK;
}

/*
 * PRE: 
 * POST: 
 */
bool hay_superposicion_de_barcos(barco_t barcos_jugador[CANT_BARCOS], int cant_barcos_leidos) {
    int ultimo_barco = cant_barcos_leidos - 1;
    bool hay_superposicion = false;

    int barco_anterior = 0;
    int posicion_anterior = 0;
    int posicion_ultimo = 0;

    while (barco_anterior < ultimo_barco && !hay_superposicion) {
        posicion_anterior = 0;

        while (posicion_anterior < barcos_jugador[barco_anterior].largo && !hay_superposicion) {
            posicion_ultimo = 0;

            while (posicion_ultimo < barcos_jugador[ultimo_barco].largo && !hay_superposicion) {

                if (barcos_jugador[barco_anterior].posiciones[posicion_anterior].fila == barcos_jugador[ultimo_barco].posiciones[posicion_ultimo].fila &&
                    barcos_jugador[barco_anterior].posiciones[posicion_anterior].columna == barcos_jugador[ultimo_barco].posiciones[posicion_ultimo].columna) {

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

/*
 * PRE: 
 * POST: 
 */
bool composicion_flota_valida(barco_t barcos_jugador[CANT_BARCOS]) {
    int cant_largo_2 = 0;
    int cant_largo_3 = 0;
    int cant_largo_4 = 0;
    int cant_largo_5 = 0;
    bool es_composicion_valida = false;

    int i = 0;

    while (i < CANT_BARCOS) {

        switch (barcos_jugador[i].largo) {

            case LARGO_BARCO_2:
                cant_largo_2++;
                break;
            case LARGO_BARCO_3:
                cant_largo_3++;
                break;
            case LARGO_BARCO_4:
                cant_largo_4++;
                break;
            case LARGO_BARCO_5:
                cant_largo_5++;
                break;
        }

        i++;
    }

    if (cant_largo_2 == CANT_BARCOS_LARGO_2 && cant_largo_3 == CANT_BARCOS_LARGO_3 && cant_largo_4 == CANT_BARCOS_LARGO_4 && cant_largo_5 == CANT_BARCOS_LARGO_5) {
        es_composicion_valida = true;
    }

    return es_composicion_valida;
}

/*
 * PRE: 
 * POST: 
 */
void eliminar_barcos_jugador(barco_t barcos_jugador[CANT_BARCOS], int tope_barcos) {
    int i = 0;

    while (i < tope_barcos) {
        free(barcos_jugador[i].posiciones);
        i++;
    }
}

/*
 * PRE: 
 * POST: 
 */
int jugador_crear (FILE* archivo_barcos, barco_t barcos_jugador[CANT_BARCOS]) {
    coordenada_t coordenada = {0, 0}; 
    int largo = 0;
    char direccion = ' ';

    int cant_barcos_actual = 0;
    int datos_leidos = 0;
    int estado_crear_barco = 0;
    bool error_formato_archivo = false;
    bool error_barco_invalido = false;
    bool error_al_reservar_memoria = false;
    bool error_barco_superposicion = false;
    
    datos_leidos = fscanf(archivo_barcos, FORMATO_ARCHIVO_BARCO, &coordenada.fila, &coordenada.columna, &direccion, &largo);

    if (datos_leidos == EOF) {
        return ERROR_ARCHIVO_BARCOS_VACIO;
    }
    
    while ((datos_leidos != EOF) && (cant_barcos_actual < CANT_BARCOS) && (!error_formato_archivo) && (!error_barco_invalido) && (!error_al_reservar_memoria) && (!error_barco_superposicion)) {
        
        if (datos_leidos != CANTIDAD_DATOS_POR_LINEA) {
            error_formato_archivo = true;
        } 
        else {
            if (!es_coordenada_valida(coordenada) || !es_direccion_valida(direccion) || !es_largo_valido(largo)) {
                error_barco_invalido = true;
            }
            else {
                estado_crear_barco = crear_barco(&barcos_jugador[cant_barcos_actual], coordenada, direccion, largo);
                if (estado_crear_barco == OK) {
                    cant_barcos_actual++;
                    if (hay_superposicion_de_barcos(barcos_jugador, cant_barcos_actual)) {
                        error_barco_superposicion = true;
                    }
                }
                else { 
                    if (estado_crear_barco == ERROR_AL_RESERVAR_MEMORIA) {
                        error_al_reservar_memoria = true;
                    }
                    else if (estado_crear_barco == ERROR_BARCO_FUERA_DEL_TABLERO) {
                        error_barco_invalido = true;
                    }
                }
            }
        }
        
        datos_leidos = fscanf(archivo_barcos, FORMATO_ARCHIVO_BARCO, &coordenada.fila, &coordenada.columna, &direccion, &largo);
    }

    if (error_formato_archivo) {
        eliminar_barcos_jugador(barcos_jugador, cant_barcos_actual);
        return ERROR_LEER_FORMATO_ARCHIVO_BARCOS;
    }
    if (error_barco_invalido) {
        eliminar_barcos_jugador(barcos_jugador, cant_barcos_actual);
        return ERROR_BARCO_FUERA_DEL_TABLERO;
    }
    if (error_al_reservar_memoria) {
        eliminar_barcos_jugador(barcos_jugador, cant_barcos_actual);
        return ERROR_AL_RESERVAR_MEMORIA;
    }
    if (error_barco_superposicion) {
        eliminar_barcos_jugador(barcos_jugador, cant_barcos_actual);
        return ERROR_BARCO_SUPERPOSICION;
    }
    if (cant_barcos_actual != CANT_BARCOS) {
        eliminar_barcos_jugador(barcos_jugador, cant_barcos_actual);
        return ERROR_CANT_BARCOS_ARCHIVO_BARCOS;
    }
    if (!composicion_flota_valida(barcos_jugador)) {
        eliminar_barcos_jugador(barcos_jugador, cant_barcos_actual);
        return ERROR_BARCO_COMPOSICION_FLOTA;
    }

    return OK;
}

/*
 * PRE: 
 * POST: 
 */
void inicializar_estado_flota(estado_barco_t flota_jugador[], barco_t barcos_jugador[]) {
    int i = 0;
    int j = 0;

    while (i < CANT_BARCOS) {
        flota_jugador[i].barco = &barcos_jugador[i];
        j = 0;
        
        while (j < LARGO_BARCO_MAXIMO) {
            flota_jugador[i].impactos[j] = false;
            j++;
        }

        i++;
    }
}

/*
 * PRE: 
 * POST: 
 */
void inicializar_tablero(char tablero[FILA_MAX_TABLERO][COLUMNA_MAX_TABLERO]) {
    int fila = 0;
    int columna = 0;

    while (fila < FILA_MAX_TABLERO) {

        columna = 0;

        while (columna < COLUMNA_MAX_TABLERO) {
            tablero[fila][columna] = VACIO;
            columna++;
        }

        fila++;
    }
}

/*
 * PRE: 
 * POST: 
 */
void cargar_barcos_en_tablero(char tablero[FILA_MAX_TABLERO][COLUMNA_MAX_TABLERO], barco_t barcos_jugador[]) {
    int i = 0;
    int j = 0;

    int fila = 0;
    int columna = 0;

    while (i < CANT_BARCOS) {

        j = 0;

        while (j < barcos_jugador[i].largo) {
            fila = barcos_jugador[i].posiciones[j].fila - 1;
            columna = barcos_jugador[i].posiciones[j].columna - 1;

            tablero[fila][columna] = BARCO;

            j++;
        }

        i++;
    }
}

/*
 * PRE: 
 * POST: 
 */
void mostrar_tableros(char tablero_jugador[FILA_MAX_TABLERO][COLUMNA_MAX_TABLERO], char tablero_oponente[FILA_MAX_TABLERO][COLUMNA_MAX_TABLERO]) {
    int fila = 0;
    int columna = 0;

    printf("\nTABLERO JUGADOR\t\tTABLERO OPONENTE\n\n");

    while (fila < FILA_MAX_TABLERO) {

        columna = 0;

        while (columna < COLUMNA_MAX_TABLERO) {
            printf("%c ", tablero_jugador[fila][columna]);
            columna++;
        }

        printf("\t");

        columna = 0;

        while (columna < COLUMNA_MAX_TABLERO) {
            printf("%c ", tablero_oponente[fila][columna]);
            columna++;
        }

        printf("\n");
        fila++;
    }

    printf("\n");
}

/*
 * PRE: 
 * POST: 
 */
coordenada_t pedir_disparo_jugador()
{
    coordenada_t disparo;

    do {
        printf("Ingrese disparo (fila;columna): ");
        scanf(FORMATO_DISPARO_JUGADOR, &disparo.fila, &disparo.columna);
    } while (!es_coordenada_valida(disparo));

    return disparo;
}

/*
 * PRE: 
 * POST: 
 */
void actualizar_tablero_enemigo(char tablero_oponente[FILA_MAX_TABLERO][COLUMNA_MAX_TABLERO], coordenada_t disparo, char resultado) {
    tablero_oponente[disparo.fila - 1][disparo.columna - 1] = resultado;
}

/*
 * PRE: 
 * POST: 
 */
void procesar_disparo_enemigo(coordenada_t disparo, estado_barco_t flota_jugador[], char tablero_jugador[FILA_MAX_TABLERO][COLUMNA_MAX_TABLERO]) {
    bool impacto = false;

    int i = 0;
    int j = 0;

    while (i < CANT_BARCOS && !impacto) {

        j = 0;

        while (j < flota_jugador[i].barco->largo && !impacto) {

            if (flota_jugador[i].barco->posiciones[j].fila == disparo.fila && flota_jugador[i].barco->posiciones[j].columna == disparo.columna) {
                flota_jugador[i].impactos[j] = true;
                tablero_jugador[disparo.fila - 1][disparo.columna - 1] = TOCADO;
                impacto = true;
            }

            j++;
        }

        i++;
    }

    if (!impacto) {
        tablero_jugador[disparo.fila - 1][disparo.columna - 1] = AGUA;
    }
}

/*
 * PRE: 
 * POST: 
 */
bool es_barco_hundido(estado_barco_t barco) {
    int i = 0;

    while (i < barco.barco->largo) {

        if (!barco.impactos[i]) {
            return false;
        }

        i++;
    }

    return true;
}

/*
 * PRE: 
 * POST: 
 */
bool es_flota_jugador_hundida(estado_barco_t flota_jugador[]) {
    int i = 0;

    while (i < CANT_BARCOS) {

        if (!es_barco_hundido(flota_jugador[i])) {
            return false;
        }

        i++;
    }

    return true;
}

/*
 * PRE: 
 * POST: 
 */
void generar_reporte(FILE* archivo_reporte, 
                    int balas_aliadas_acertadas, 
                    int balas_aliadas_erradas, 
                    int balas_enemigas_acertadas, 
                    int balas_enemigas_erradas, 
                    int barcos_enemigos_hundidos, 
                    int barcos_aliados_sobrevivientes) 
{
    fprintf(archivo_reporte,
            "Balas aliadas acertadas: %d\n"
            "Balas aliadas erradas: %d\n"
            "Balas enemigas acertadas: %d\n"
            "Balas enemigas erradas: %d\n"
            "Barcos enemigos hundidos: %d\n"
            "Barcos aliados sobrevivientes: %d\n",
            balas_aliadas_acertadas,
            balas_aliadas_erradas,
            balas_enemigas_acertadas,
            balas_enemigas_erradas,
            barcos_enemigos_hundidos,
            barcos_aliados_sobrevivientes);
}

/*
 * PRE: 
 * POST: 
 */
void mostrar_error_jugador_crear(int estado_jugador_crear)
{
    switch (estado_jugador_crear) {

        case ERROR_ARCHIVO_BARCOS_VACIO:
            printf("ERROR AL LEER EL ARCHIVO BARCOS, SE ENCUENTRA VACÍO\n");
            break;

        case ERROR_CANT_BARCOS_ARCHIVO_BARCOS:
            printf("ERROR AL LEER EL ARCHIVO BARCOS, LA CANTIDAD DE BARCOS ES INVÁLIDA\n");
            break;

        case ERROR_BARCO_COMPOSICION_FLOTA:
            printf("ERROR AL LEER EL ARCHIVO BARCOS, LA COMPOSICIÓN DE LOS BARCOS ES INVÁLIDA\n");
            break;

        case ERROR_LEER_FORMATO_ARCHIVO_BARCOS:
            printf("ERROR AL LEER EL ARCHIVO BARCOS, EL FORMATO DE LOS DATOS ES INVÁLIDO\n");
            break;

        case ERROR_BARCO_FUERA_DEL_TABLERO:
            printf("ERROR AL CREAR EL BARCO, SUS COORDENADAS ESTÁN FUERA DEL TABLERO\n");
            break;

        case ERROR_AL_RESERVAR_MEMORIA:
            printf("ERROR AL RESERVAR MEMORIA PARA EL BARCO\n");
            break;

        case ERROR_BARCO_SUPERPOSICION:
            printf("ERROR AL CREAR EL BARCO, HAY SUPERPOSICIÓN, LAS COORDENADAS COINCIDEN CON OTRO BARCO\n");
            break;
        
        default:
            break;
    }
}


int main(int argc, char* argv[]) {

    if (!validar_argumentos(argc)) {
        printf("ERROR AL EJECUTAR EL PROGRAMA\nSe debe ejecutar el programa con dos argumentos: ./ejecutable <archivo_barcos> <archivo_reporte>\n");
        return ERROR_ARGUMENTOS;
    }

    FILE* archivo_barcos = fopen(argv[POSICION_ARCHIVO_BARCOS], MODO_LECTURA);
    if (archivo_barcos == NULL) {
        printf("ERROR AL ABRIR EL ARCHIVO DE BARCOS\n");
        return ERROR_ARCHIVO_APERTURA;
    }

    barco_t barcos_jugador[CANT_BARCOS];
     
    int estado_jugador = jugador_crear(archivo_barcos, barcos_jugador);

    if (estado_jugador != OK) {
        mostrar_error_jugador_crear(estado_jugador);
        fclose(archivo_barcos);
        return estado_jugador;
    }

    FILE* archivo_reporte = fopen(argv[POSICION_ARCHIVO_REPORTE], MODO_ESCRITURA);
    if (archivo_reporte == NULL) {
        printf("ERROR AL ABRIR EL ARCHIVO DE REPORTE\n");
        eliminar_barcos_jugador(barcos_jugador, CANT_BARCOS);
        fclose(archivo_barcos);
        return ERROR_ARCHIVO_APERTURA;
    }

    oponente_t* oponente = oponente_crear(barcos_jugador);
    if (oponente == NULL) {
        printf("ERROR AL CREAR AL OPONENTE\n");
        eliminar_barcos_jugador(barcos_jugador, CANT_BARCOS);
        fclose(archivo_barcos);
        fclose(archivo_reporte);
        return ERROR_AL_CREAR_OPONENTE;
    }

    estado_barco_t flota_jugador[CANT_BARCOS];
    inicializar_estado_flota(flota_jugador, barcos_jugador);

    char tablero_jugador[FILA_MAX_TABLERO][COLUMNA_MAX_TABLERO];
    char tablero_oponente[FILA_MAX_TABLERO][COLUMNA_MAX_TABLERO];

    inicializar_tablero(tablero_jugador);
    inicializar_tablero(tablero_oponente);

    cargar_barcos_en_tablero(tablero_jugador, barcos_jugador);

    int balas_aliadas_acertadas = 0;
    int balas_aliadas_erradas = 0;
    int balas_enemigas_acertadas = 0;
    int balas_enemigas_erradas = 0;

    int barcos_enemigos_hundidos = 0;

    bool juego_terminado = false;

    while (!juego_terminado) {

        mostrar_tableros(tablero_jugador, tablero_oponente);

        coordenada_t disparo_jugador = pedir_disparo_jugador();

        char resultado = oponente_recibe_disparo(oponente, disparo_jugador);

        actualizar_tablero_enemigo(tablero_oponente, disparo_jugador, resultado);

        if (resultado == AGUA) {
            balas_aliadas_erradas++;
        } else {
            balas_aliadas_acertadas++;
        }

        if (resultado == HUNDIDO) {
            barcos_enemigos_hundidos++;
        }

        if (barcos_enemigos_hundidos == CANT_BARCOS) {
            juego_terminado = true;
        }

        if (!juego_terminado) {

            coordenada_t disparo_enemigo = oponente_realiza_disparo(oponente);

            procesar_disparo_enemigo(disparo_enemigo, flota_jugador, tablero_jugador);

            if (tablero_jugador[disparo_enemigo.fila - 1][disparo_enemigo.columna - 1] == AGUA) {
                balas_enemigas_erradas++;
            } else {
                balas_enemigas_acertadas++;
            }

            if (es_flota_jugador_hundida(flota_jugador)) {
                juego_terminado = true;
            }
        }
    }

    int barcos_aliados_sobrevivientes = 0;

    int i = 0;
    while (i < CANT_BARCOS) {

        if (!es_barco_hundido(flota_jugador[i])) {
            barcos_aliados_sobrevivientes++;
        }

        i++;
    }

    generar_reporte(
        archivo_reporte,
        balas_aliadas_acertadas,
        balas_aliadas_erradas,
        balas_enemigas_acertadas,
        balas_enemigas_erradas,
        barcos_enemigos_hundidos,
        barcos_aliados_sobrevivientes
    );

    oponente_destruir(oponente);

    eliminar_barcos_jugador(barcos_jugador, CANT_BARCOS);

    fclose(archivo_barcos);
    fclose(archivo_reporte);

    return OK;
}