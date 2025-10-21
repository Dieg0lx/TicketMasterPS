#ifndef SCREEN_PAGO_H
#define SCREEN_PAGO_H

void InitPagoScreen(float monto); // Recibe el monto a pagar
void UpdatePagoScreen(void);
void DrawPagoScreen(void);
void UnloadPagoScreen(void);


// 0: Nada, 1: Pago Exitoso, -1: Cancelado/Volver
extern int pagoScreenResult;

#endif // SCREEN_PAGO_H