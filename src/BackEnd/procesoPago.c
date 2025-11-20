#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "procesoPago.h"

int esSoloNumeros(const char *cadena) {
    for (int i = 0; cadena[i] != '\0'; i++) {
        if (!isdigit(cadena[i])) {
            return 0; 
        }
    }
    return 1; 
}

int esSoloLetras(const char *cadena) {
    for (int i = 0; cadena[i] != '\0'; i++) {
        if (!isalpha(cadena[i]) && !isspace(cadena[i])) {
            return 0; 
        }
    }
    return 1;
}

int validarNumeroTarjeta(const char *numero) {
    return esSoloNumeros(numero) && strlen(numero) == 16;
}

int validarNombreTitular(const char *nombre) {
    return strlen(nombre) > 3; 
}

int validarCVV(const char *cvv) {
    int len = strlen(cvv);
    return esSoloNumeros(cvv) && (len == 3 || len == 4);
}

int validarFechaVencimiento(const char *fecha) {
    return strlen(fecha) == 5 && fecha[2] == '/';
}

int ejecutarPago(DatosPago *pago, float monto) {

    return 1;
}