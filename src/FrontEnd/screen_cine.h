#ifndef SCREEN_CINE_H
#define SCREEN_CINE_H

// --- Declaraciones de Funciones Públicas ---
void InitCineScreen(void);
void UpdateCineScreen(void);
void DrawCineScreen(void);
void UnloadCineScreen(void);

// --- Variables de Comunicación ---
// 0: Nada, 1: Comprar, -1: Volver
extern int cineScreenResult;
// Para pasar el monto a la pantalla de pago
extern float cineTotalCompra;

#endif // SCREEN_CINE_H