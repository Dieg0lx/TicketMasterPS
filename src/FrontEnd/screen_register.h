#ifndef SCREEN_REGISTER_H
#define SCREEN_REGISTER_H

#include "raylib.h"

void InitRegisterScreen(void);
void UpdateRegisterScreen(void);
void DrawRegisterScreen(void);
void UnloadRegisterScreen(void);

extern int registerScreenResult;

int RegisterHitTest(Vector2 mouse);

#endif
