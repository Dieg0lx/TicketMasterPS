#ifndef SCREEN_MENU_H
#define SCREEN_MENU_H

#include "raylib.h" 

void InitMenuScreen(void);
void UpdateMenuScreen(void);
void DrawMenuScreen(void);
void UnloadMenuScreen(void);

void ShowMenuSuccessMessage(void); 

extern int menuSelection;

extern int logoutRequest;

int HitTestMenu(Vector2 mouse);
void SetMenuSelectionFromPoint(Vector2 mouse);

#endif