#ifndef SCREEN_LOGIN_H
#define SCREEN_LOGIN_H

void InitLoginScreen(void);
void UpdateLoginScreen(void);
void DrawLoginScreen(void);
void UnloadLoginScreen(void);

extern int loginResult;
extern int goToRegister;

#endif