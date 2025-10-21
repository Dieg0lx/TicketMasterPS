#ifndef SCREEN_TEATRO_H
#define SCREEN_TEATRO_H

// Declaraciones de las funciones públicas
void InitTeatroScreen(void);
void UpdateTeatroScreen(void);
void DrawTeatroScreen(void);
void UnloadTeatroScreen(void);

// Variables para comunicar el resultado a main.c
extern int teatroScreenResult;   // 0: Nada, 1: Comprar, -1: Volver
extern float teatroTotalCompra;  // Para pasar el monto a la pantalla de pago

#endif // SCREEN_TEATRO_H