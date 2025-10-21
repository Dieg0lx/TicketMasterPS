#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "procesoPago.h"

// Verifica si una cadena contiene solo dígitos numéricos
int esSoloNumeros(const char *cadena) {
    for (int i = 0; cadena[i] != '\0'; i++) {
        if (!isdigit(cadena[i])) {
            return 0; 
        }
    }
    return 1; 
}

// Verifica si una cadena contiene solo letras y espacios
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
    return esSoloLetras(nombre) && strlen(nombre) > 3;
}

int validarCVV(const char *cvv) {
    int len = strlen(cvv);
    return esSoloNumeros(cvv) && (len == 3 || len == 4);
}

int validarFechaVencimiento(const char *fecha) {
    return strlen(fecha) == 5 && fecha[2] == '/' && isdigit(fecha[0]) && isdigit(fecha[1]) && isdigit(fecha[3]) && isdigit(fecha[4]);
}

int ejecutarPago(DatosPago *pago, float monto) {

    if (validarNumeroTarjeta(pago->numeroTarjeta) &&
        validarNombreTitular(pago->nombreTitular) &&
        validarCVV(pago->cvv) &&
        validarFechaVencimiento(pago->fechaVencimiento))
    {
        return 1;
    }
    return 0; 
}