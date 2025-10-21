#ifndef PAGO_H
#define PAGO_H

// Estructura para datos de pago
typedef struct {
    char numeroTarjeta[20];
    char nombreTitular[100];
    char cvv[5];
    char fechaVencimiento[8];
} DatosPago;

// --- Funciones de validación que el Front-End usará ---
int validarNumeroTarjeta(const char *numero);
int validarNombreTitular(const char *nombre);
int validarCVV(const char *cvv);
int validarFechaVencimiento(const char *fecha);

// Función final que simula el envío del pago
// Devuelve 1 si es exitoso, 0 si falla (simulación)
int ejecutarPago(DatosPago *pago, float monto);

#endif // PROCESOPAGO_H