#ifndef PAGO_H
#define PAGO_H

typedef struct {
    char numeroTarjeta[20];
    char nombreTitular[100];
    char cvv[5];
    char fechaVencimiento[8];
} DatosPago;

int validarNumeroTarjeta(const char *numero);
int validarNombreTitular(const char *nombre);
int validarCVV(const char *cvv);
int validarFechaVencimiento(const char *fecha);

int ejecutarPago(DatosPago *pago, float monto);

#endif