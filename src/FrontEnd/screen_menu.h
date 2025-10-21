#ifndef SCREEN_MENU_H
#define SCREEN_MENU_H

#include "raylib.h"  // Necesario para Vector2 y Rectangle

// API de la pantalla de menú
void InitMenuScreen(void);
void UpdateMenuScreen(void);
void DrawMenuScreen(void);
void UnloadMenuScreen(void);

// Estado global del menú (0 = nada, 1 = teatro, 2 = cine, 3 = museo)
extern int menuSelection;

// === API para pruebas (no requiere ventana/recursos) ===

// Devuelve qué botón contiene el punto del mouse.
// 0 = ninguno, 1 = teatro, 2 = cine, 3 = museo
int HitTestMenu(Vector2 mouse);

// Helper para tests: fija menuSelection a partir de un punto
void SetMenuSelectionFromPoint(Vector2 mouse);

#endif // SCREEN_MENU_H