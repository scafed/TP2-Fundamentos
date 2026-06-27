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
#define FILA_MIN_TABLERO 0
#define FILA_MAX_TABLERO 9
#define COLUMNA_MIN_TABLERO 0
#define COLUMNA_MAX_TABLERO 9
#define FILA_MIN_USUARIO 1
#define FILA_MAX_USUARIO 10
#define COLUMNA_MIN_USUARIO 1
#define COLUMNA_MAX_USUARIO 10
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
#define EMOJI_BARCO "\U0001F6A2"
#define EMOJI_AGUA "\U0001f7e6"
#define EMOJI_TOCADO "\U0001F4A5"
#define EMOJI_HUNDIDO "\u2620\uFE0F "
#define EMOJI_VACIO "\u2B1B"
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
    bool impactos[LARGO_BARCO_MAXIMO];
} estado_barco_t;

/*
 * PRE: -
 * POST: Devuelve true si la cantidad de argumentos que se reciben es igual a CANT_ARGUMENTOS, false en caso contrario.
 */
bool validar_argumentos(int argc) {
    return (argc == CANT_ARGUMENTOS);
}

/*
 * PRE: La coordenada recibida por parámetro debe estar cargada con fila y columna.
 * POST: Devuelve true si la coordenada está dentro de los indices del tablero (entre las coordenadas (FILA_MIN_TABLERO, COLUMNA_MIN_TABLERO) y (FILA_MAX_TABLERO, COLUMNA_MAX_TABLERO)), false en caso contrario.
 */
bool es_coordenada_valida_tablero(coordenada_t coordenada) {
    return ((coordenada.fila >= FILA_MIN_TABLERO) &&
            (coordenada.fila <= FILA_MAX_TABLERO) &&
            (coordenada.columna >= COLUMNA_MIN_TABLERO) &&
            (coordenada.columna <= COLUMNA_MAX_TABLERO));
}

/*
 * PRE: La coordenada recibida por parámetro debe estar cargada con fila y columna.
 * POST: Devuelve true si la coordenada está dentro del tablero (entre las coordenadas (FILA_MIN_USUARIO, COLUMNA_MIN_USUARIO) y (FILA_MAX_USUARIO, COLUMNA_MAX_USUARIO)), false en caso contrario.
 */
bool es_coordenada_valida_usuario(coordenada_t coordenada) {
    return ((coordenada.fila >= FILA_MIN_USUARIO) && 
            (coordenada.fila <= FILA_MAX_USUARIO) && 
            (coordenada.columna >= COLUMNA_MIN_USUARIO) && 
            (coordenada.columna <= COLUMNA_MAX_USUARIO));
}

/*
 * PRE: -
 * POST: Devuelve true si la dirección es igual a DIRECCION_NORTE o DIRECCION SUR o DIRECCION_ESTE o DIRECCION_OESTE, false en caso contrario.
 */
bool es_direccion_valida(char direccion) {
    return ((direccion == DIRECCION_NORTE) || (direccion == DIRECCION_SUR) || (direccion == DIRECCION_ESTE) || (direccion == DIRECCION_OESTE));
}

/*
 * PRE: El largo del barco debe estar cargado previamente.
 * POST: Devuelve true si el largo del barco está dentro de los límites LARGO_BARCO_MINIMO y LARGO_BARCO_MAXIMO, false en caso contrario.
 */
bool es_largo_valido(int largo) {
    return ((largo >= LARGO_BARCO_MINIMO) && (largo <= LARGO_BARCO_MAXIMO));
}

/*
 * PRE: La coordenada, direccion y largo del barco deben ser válidos.
 * POST: Crea al barco del jugador cargando sus coordenadas en función de la direccion, largo, y coordenada inicial. Devuelve el estado de crear el barco: en caso de que se salga del límite del tablero o haya un error de memoria, devuelve el número de error, sino devuelve OK.
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
        if (!es_coordenada_valida_tablero(coordenada)) {
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
 * PRE: Debe haber por lo menos un barco cargado.
 * POST: Devuelve true si existe superposcion de coordenadas entre barcos, false en caso contrario.
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
 * PRE: Deben estar los barcos creados previamente.
 * POST: Devuelve true si la composicion de la flota (CANT_BARCOS_LARGO_2, CANT_BARCOS_LARGO_3, CANT_BARCOS_LARGO_4, CANT_BARCOS_LARGO_5) coincide, false en caso contrario.
 */
bool composicion_flota_valida(barco_t barcos_jugador[CANT_BARCOS]) {
    int cant_largo_2 = 0;
    int cant_largo_3 = 0;
    int cant_largo_4 = 0;
    int cant_largo_5 = 0;

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

    return ((cant_largo_2 == CANT_BARCOS_LARGO_2) && (cant_largo_3 == CANT_BARCOS_LARGO_3) && (cant_largo_4 == CANT_BARCOS_LARGO_4) && (cant_largo_5 == CANT_BARCOS_LARGO_5));
}

/*
 * PRE: Debe haber por lo menos un barco creado previamente con crear_barco().
 * POST: Elimina los barcos creados, liberando la memoria.
 */
void eliminar_barcos_jugador(barco_t barcos_jugador[CANT_BARCOS], int tope_barcos) {
    int i = 0;

    while (i < tope_barcos) {
        free(barcos_jugador[i].posiciones);
        i++;
    }
}

/*
 * PRE: El archivo archivo_barcos debe estar abierto correctamente en modo lectura.
 * POST: Crea todos los barcos del jugador, validando los barcos y el archivo. Devuelve el código de error en caso de un error, sino devuelve OK.
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

    coordenada.fila--;
    coordenada.columna--;

    if (datos_leidos == EOF) {
        return ERROR_ARCHIVO_BARCOS_VACIO;
    }
    
    while ((datos_leidos != EOF) && (cant_barcos_actual < CANT_BARCOS) && (!error_formato_archivo) && (!error_barco_invalido) && (!error_al_reservar_memoria) && (!error_barco_superposicion)) {
        
        if (datos_leidos != CANTIDAD_DATOS_POR_LINEA) {
            error_formato_archivo = true;
        } 
        else {
            if (!es_coordenada_valida_tablero(coordenada) || !es_direccion_valida(direccion) || !es_largo_valido(largo)) {
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

        coordenada.fila--;
        coordenada.columna--;
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
 * PRE: Los barcos del jugador debe estar creada previamente con jugador_crear().
 * POST: Crea la flota de barcos del jugador con los barcos creados previamente, y en cada coordenada de cada barco se pone false como impacto.
 */
void inicializar_estado_flota(estado_barco_t flota_jugador[CANT_BARCOS], barco_t barcos_jugador[CANT_BARCOS]) {
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
 * PRE: -
 * POST: Inicializa el valor de cada fila y columna del tablero con un VACIO.
 */
void inicializar_tablero(char tablero[FILA_MAX_TABLERO + 1][COLUMNA_MAX_TABLERO + 1]) {
    int fila = FILA_MIN_TABLERO;
    int columna = COLUMNA_MIN_TABLERO;

    while (fila <= FILA_MAX_TABLERO) {

        columna = COLUMNA_MIN_TABLERO;

        while (columna <= COLUMNA_MAX_TABLERO) {
            tablero[fila][columna] = VACIO;
            columna++;
        }

        fila++;
    }
}

/*
 * PRE: Los barcos deben estar previamente creados con jugador_crear().
 * POST: Carga los barcos en el tablero, cargando en cada fila y columna del tablero, un BARCO en cada coordenada correspondiente.
 */
void cargar_barcos_en_tablero(char tablero[FILA_MAX_TABLERO + 1][COLUMNA_MAX_TABLERO + 1], barco_t barcos_jugador[]) {
    int i = 0;
    int j = 0;

    int fila = FILA_MIN_TABLERO;
    int columna = COLUMNA_MIN_TABLERO;

    while (i < CANT_BARCOS) {

        j = 0;

        while (j < barcos_jugador[i].largo) {
            fila = barcos_jugador[i].posiciones[j].fila;
            columna = barcos_jugador[i].posiciones[j].columna;

            tablero[fila][columna] = BARCO;

            j++;
        }

        i++;
    }
}

/*
 * PRE: -
 * POST: Imprime por pantalla el emoji según el tipo de caracter recibido.
 */
void imprimir_emoticon(char caracter) {

    switch (caracter) {

        case BARCO:
            printf("%s", EMOJI_BARCO);
            break;

        case AGUA:
            printf("%s", EMOJI_AGUA);
            break;

        case TOCADO:
            printf("%s", EMOJI_TOCADO);
            break;

        case HUNDIDO:
            printf("%s", EMOJI_HUNDIDO);
            break;

        case VACIO:
            printf("%s", EMOJI_VACIO);
            break;
    }
}

/*
 * PRE: -
 * POST: Muestra los tableros de jugador y de oponente por pantalla.
 */
void mostrar_tableros(char tablero_jugador[FILA_MAX_TABLERO + 1][COLUMNA_MAX_TABLERO + 1], char tablero_oponente[FILA_MAX_TABLERO + 1][COLUMNA_MAX_TABLERO + 1]) {
    int fila = FILA_MIN_TABLERO;
    int columna = COLUMNA_MIN_TABLERO;

    printf("  ===================================================================\n");
    printf("\t\t\t\tBATALLA NAVAL\n");
    printf("  ===================================================================\n");

    printf("\n\t  TABLERO JUGADOR\t\t  TABLERO OPONENTE\n\n");

    while (fila <= FILA_MAX_TABLERO) {

        columna = COLUMNA_MIN_TABLERO;
        
        printf("\t");

        while (columna <= COLUMNA_MAX_TABLERO) {
            imprimir_emoticon(tablero_jugador[fila][columna]);
            columna++;
        }

        printf("\t\t");

        columna = COLUMNA_MIN_TABLERO;

        while (columna <= COLUMNA_MAX_TABLERO) {
            imprimir_emoticon(tablero_oponente[fila][columna]);
            columna++;
        }

        printf("\n");
        fila++;
    }

    printf("\n");
}

/*
 * PRE: -
 * POST: Imprime por pantalla un mensaje solicitando el disparo al jugador y lee el disparo en FORMATO_DISPARO_JUGADOR, hasta que la coordenada sea válida. Devuelve el disparo.
 */
coordenada_t pedir_disparo_jugador() {
    coordenada_t disparo;

    do {
        printf("Ingrese disparo dentro del tablero (fila;columna): ");
        scanf(FORMATO_DISPARO_JUGADOR, &disparo.fila, &disparo.columna);
    } while (!es_coordenada_valida_usuario(disparo));

    disparo.fila--;
    disparo.columna--;

    return disparo;
}

/*
 * PRE: El tablero debe estar previamente creado y cargado, el disparo del jugador debe ser válido.
 * POST: Actualiza el tablero del oponente, colocando en la coordenada del disparo del jugador, el resultado del disparo.
 */
void actualizar_tablero_enemigo(char tablero_oponente[FILA_MAX_TABLERO + 1][COLUMNA_MAX_TABLERO + 1], coordenada_t disparo_jugador, char resultado_del_disparo_jugador) {
    tablero_oponente[disparo_jugador.fila][disparo_jugador.columna] = resultado_del_disparo_jugador;
}

/*
 * PRE: El disparo del enemigo debe ser válido. La flota de los barcos debe ser creada previamente y debe ser válida.
 * POST: Actualiza el tablero del jugador, en función del disparo enemigo. Coloca AGUA si falla el disparo, o TOCADO si el disparo acierta a un barco.
 */
void procesar_disparo_enemigo(coordenada_t disparo_enemigo, estado_barco_t flota_jugador[CANT_BARCOS], char tablero_jugador[FILA_MAX_TABLERO + 1][COLUMNA_MAX_TABLERO + 1]) {
    bool impacto = false;

    int i = 0;
    int j = 0;

    while (i < CANT_BARCOS && !impacto) {

        j = 0;

        while (j < flota_jugador[i].barco->largo && !impacto) {

            if ((flota_jugador[i].barco->posiciones[j].fila == disparo_enemigo.fila) && (flota_jugador[i].barco->posiciones[j].columna == disparo_enemigo.columna)) {
                flota_jugador[i].impactos[j] = true;
                tablero_jugador[disparo_enemigo.fila][disparo_enemigo.columna] = TOCADO;
                impacto = true;
            }

            j++;
        }

        i++;
    }

    if (!impacto) {
        tablero_jugador[disparo_enemigo.fila][disparo_enemigo.columna] = AGUA;
    }
}

/*
 * PRE: El barco debe estar previamente creado, e inicializado el estado del impacto.
 * POST: Devuelve true si un barco fue hundido, es decir si en todas las coordenadas del barco, hubo impacto. False en caso contrario.
 */
bool es_barco_hundido(estado_barco_t barco) {
    int i = 0;
    bool es_hundido = true;

    while (i < barco.barco->largo && es_hundido) {

        if (!barco.impactos[i]) {
            es_hundido = false;
        }

        i++;
    }

    return es_hundido;
}

/*
 * PRE: Los barcos deben estar creados previamente, e inicializados todos los estados de impacto.
 * POST: Devuelve true si todos los barcos fueron hundidos, es decir si en todas las coordenadas de todos los barcos hubo impacto. False en caso contrario.
 */
bool es_flota_jugador_hundida(estado_barco_t flota_jugador[CANT_BARCOS]) {
    int i = 0;
    bool flota_hundida = true;

    while (i < CANT_BARCOS && flota_hundida) {

        if (!es_barco_hundido(flota_jugador[i])) {
            flota_hundida = false;
        }

        i++;
    }

    return flota_hundida;
}

/*
 * PRE: El archivo archivo_reporte debe estar abierto previamente en modo escritura.
 * POST: Escribe en el archivo las estadísticas del juego: balas aliadas y enemigas acertadas, balas aliadas y enemigas erradas, barcos enemigos hundidos, barcos aliados sobrevivientes.
 */
void generar_reporte(FILE* archivo_reporte, 
                    int balas_aliadas_acertadas, 
                    int balas_aliadas_erradas, 
                    int balas_enemigas_acertadas, 
                    int balas_enemigas_erradas, 
                    int barcos_enemigos_hundidos, 
                    int barcos_aliados_sobrevivientes) {

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
 * PRE: -
 * POST: Imprime por pantalla un mensaje de error en función del código de estado recibido. En caso de que no haya un error no se imprime nada.
 */
void mostrar_error_jugador_crear(int estado_jugador_crear) {
    
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

/*
 * PRE: -
 * POST: Actualiza la cantidad de balas erradas, balas acertadas y de barcos hundidos, del jugador en función del resultado de su disparo.
 */
void actualizar_reporte_por_disparo_jugador(char resultado_disparo, int *balas_acertadas, int *balas_erradas, int *barcos_hundidos) {
    if (resultado_disparo == AGUA) {
        (*balas_erradas)++;
    } else {
        (*balas_acertadas)++;
    }

    if (resultado_disparo == HUNDIDO) {
        (*barcos_hundidos)++;
    }
}

/*
 * PRE: El oponente, la flota de barcos del jugador y el tablero del jugador deben estar previamente creados.
 * POST: Obtiene disparos del enemigo hasta que la coordenada generada sea válida. Actualiza el estado del tablero del jugador y las estadísticas de balas enemigas según el resultado del disparo.
 */
bool jugar_turno_enemigo(oponente_t *oponente, 
                         estado_barco_t flota_jugador[CANT_BARCOS], 
                         char tablero_jugador[FILA_MAX_TABLERO + 1][COLUMNA_MAX_TABLERO + 1], 
                         int *balas_enemigas_acertadas, 
                         int *balas_enemigas_erradas) {

    coordenada_t disparo_enemigo;

    do {
        disparo_enemigo = oponente_realiza_disparo(oponente);
    } while (!es_coordenada_valida_tablero(disparo_enemigo));

    procesar_disparo_enemigo(disparo_enemigo, flota_jugador, tablero_jugador);

    if (tablero_jugador[disparo_enemigo.fila][disparo_enemigo.columna] == AGUA) {
        (*balas_enemigas_erradas)++;
    } else {
        (*balas_enemigas_acertadas)++;
    }

    return es_flota_jugador_hundida(flota_jugador);
}

/*
 * PRE: La flota del jugador debe estar previamente creada, con sus estados de impacto de cada barco.
 * POST: Devuelve la cantidad de barcos que no fueron hundidos, es decir los barcos que no tienen en todas sus coordenadas un impacto.
 */
int contar_barcos_aliados_sobrevivientes(estado_barco_t flota_jugador[CANT_BARCOS]) {
    int i = 0;
    int barcos_sobrevivientes = 0;

    while (i < CANT_BARCOS) {

        if (!es_barco_hundido(flota_jugador[i])) {
            barcos_sobrevivientes++;
        }

        i++;
    }

    return barcos_sobrevivientes;
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

    char tablero_jugador[FILA_MAX_TABLERO + 1][COLUMNA_MAX_TABLERO + 1];
    char tablero_oponente[FILA_MAX_TABLERO + 1][COLUMNA_MAX_TABLERO + 1];

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

        system("clear");

        mostrar_tableros(tablero_jugador, tablero_oponente);

        coordenada_t disparo_jugador = pedir_disparo_jugador();

        char resultado_del_disparo_jugador = oponente_recibe_disparo(oponente, disparo_jugador);

        actualizar_tablero_enemigo(tablero_oponente, disparo_jugador, resultado_del_disparo_jugador);

        actualizar_reporte_por_disparo_jugador(resultado_del_disparo_jugador, &balas_aliadas_acertadas, &balas_aliadas_erradas, &barcos_enemigos_hundidos);

        juego_terminado = (barcos_enemigos_hundidos == CANT_BARCOS);

        if (!juego_terminado) {
            juego_terminado = jugar_turno_enemigo(oponente, flota_jugador, tablero_jugador, &balas_enemigas_acertadas, &balas_enemigas_erradas);
        }
    }

    int barcos_aliados_sobrevivientes = contar_barcos_aliados_sobrevivientes(flota_jugador);

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