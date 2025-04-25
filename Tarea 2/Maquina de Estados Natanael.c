#include <stdio.h>
#include <stdbool.h>

// Definición de estados normal para poder verlo en la consola
typedef enum {
    ESTADO_INIT,
    ESTADO_CERRADO,
    ESTADO_ABIERTO,
    ESTADO_CERRANDO,
    ESTADO_ABRIENDO,
    ESTADO_DETENIDO,
    ESTADO_ERROR
} EstadoPuerta;

// Códigos de error con documentación
typedef enum {
    ERROR_OK = 0,
    ERROR_LIMIT_SWITCH = 1,  
    ERROR_TIMEOUT = 2        
} CodigoError;

// Estructura para control de puerta con nombres igual que controles 1 
typedef struct {
    bool limitSwitchCerrado;    
    bool limitSwitchAbierto;    
    bool botonPulsador;        
    bool motorAbriendo;         
    bool motorCerrando;        
    unsigned int contadorTiempo; 
    bool ledAbierto;           
    bool ledCerrado;           
    bool ledError;             
    CodigoError codigoError;   
    bool datosListos;          
} ControlPuerta;


#define TIEMPO_MAXIMO_OPERACION 180  

// etado de la puerta
EstadoPuerta manejarEstadoInicial(ControlPuerta *puerta);
EstadoPuerta manejarEstadoAbierto(ControlPuerta *puerta);
EstadoPuerta manejarEstadoAbriendo(ControlPuerta *puerta);
EstadoPuerta manejarEstadoCerrado(ControlPuerta *puerta);
EstadoPuerta manejarEstadoCerrando(ControlPuerta *puerta);
EstadoPuerta manejarEstadoDetenido(ControlPuerta *puerta);
EstadoPuerta manejarEstadoError(ControlPuerta *puerta);

//Codigo principal
int main() {
    EstadoPuerta estadoActual = ESTADO_INIT;
    ControlPuerta puerta = {0};
    
    while (true) {
        switch (estadoActual) {
            case ESTADO_INIT:      estadoActual = manejarEstadoInicial(&puerta);      break;
            case ESTADO_ABIERTO:   estadoActual = manejarEstadoAbierto(&puerta);     break;
            case ESTADO_ABRIENDO:  estadoActual = manejarEstadoAbriendo(&puerta);    break;
            case ESTADO_CERRADO:  estadoActual = manejarEstadoCerrado(&puerta);     break;
            case ESTADO_CERRANDO: estadoActual = manejarEstadoCerrando(&puerta);    break;
            case ESTADO_DETENIDO: estadoActual = manejarEstadoDetenido(&puerta);    break;
            case ESTADO_ERROR:    estadoActual = manejarEstadoError(&puerta);       break;
            default:              estadoActual = ESTADO_ERROR;                      break;
        }
    }
    
    return 0;
}

EstadoPuerta manejarEstadoInicial(ControlPuerta *puerta) {
    // Inicialización de lo actuadore
    puerta->motorAbriendo = puerta->motorCerrando = false;
    puerta->ledAbierto = puerta->ledCerrado = puerta->ledError = false;
    puerta->codigoError = ERROR_OK;
    puerta->contadorTiempo = 0;
    puerta->datosListos = false;

    
    while (!puerta->datosListos) {
        
    }

    
    if (puerta->limitSwitchCerrado && !puerta->limitSwitchAbierto) {
        return ESTADO_CERRADO;
    }
    if (!puerta->limitSwitchCerrado && !puerta->limitSwitchAbierto) {
        return ESTADO_CERRANDO;
    }
    if (puerta->limitSwitchCerrado && puerta->limitSwitchAbierto) {
        puerta->codigoError = ERROR_LIMIT_SWITCH;
        return ESTADO_ERROR;
    }
    
    return ESTADO_INIT;
}


EstadoPuerta manejarEstadoError(ControlPuerta *puerta) {
    puerta->motorAbriendo = puerta->motorCerrando = false;
    puerta->ledError = true;

    switch (puerta->codigoError) {
        case ERROR_TIMEOUT:
            printf("\n[ERROR] Tiempo de operación excedido. Revise mecanismo.\n");
            break;
        case ERROR_LIMIT_SWITCH:
            printf("\n[ERROR] Conflicto en sensores de límite. Verifique conexiones.\n");
            break;
        default:
            printf("\n[ERROR] Código de error desconocido.\n");
    }

    
    while (!puerta->botonPulsador) {}
    
    puerta->codigoError = ERROR_OK;
    return ESTADO_INIT;
}