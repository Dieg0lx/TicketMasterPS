#ifndef SCREEN_LOGIN_H
#define SCREEN_LOGIN_H

// Inicializa los recursos de la pantalla de login
void InitLoginScreen(void);

// Actualiza la lógica de la pantalla de login cada fotograma
void UpdateLoginScreen(void);

// Dibuja la pantalla de login cada fotograma
void DrawLoginScreen(void);

// Libera los recursos de la pantalla de login
void UnloadLoginScreen(void);




extern int loginResult;

#endif // SCREEN_LOGIN_H