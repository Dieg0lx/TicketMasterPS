#ifndef SCREEN_PAGO_H
#define SCREEN_PAGO_H

void InitPagoScreen(const char* tipoServicio, int servicioId, float precioTotal);
void UpdatePagoScreen(void);
void DrawPagoScreen(void);
void UnloadPagoScreen(void);


extern int pagoScreenResult;

#endif // SCREEN_PAGO_H