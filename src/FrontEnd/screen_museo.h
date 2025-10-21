#ifndef SCREEN_MUSEO_H
#define SCREEN_MUSEO_H

// --- Declaraciones de Funciones Públicas ---
void InitMuseoScreen(void);
void UpdateMuseoScreen(void);
void DrawMuseoScreen(void);
void UnloadMuseoScreen(void);

// --- Variables de Comunicación ---
// 0: Nada, 1: Comprar, -1: Volver
extern int museoScreenResult;
// Para pasar el monto a la pantalla de pago
extern float museoTotalCompra;

#endif // SCREEN_MUSEO_H