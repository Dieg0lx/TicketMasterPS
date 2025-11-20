#ifndef SCREEN_CINE_H
#define SCREEN_CINE_H

void InitCineScreen(void);
void UpdateCineScreen(void);
void DrawCineScreen(void);
void UnloadCineScreen(void);

extern int cineScreenResult;
extern float cineTotalCompra;

#endif