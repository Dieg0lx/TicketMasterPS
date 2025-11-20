#include "../../include/raylib.h"
#include "screen_login.h"
#include "ui_styles.h" 
#include <string.h>
#include <stdio.h>
#include "../BackEnd/Login.h"
#include "../BackEnd/session.h"

static char username[20] = {0};
static char password[20] = {0};
static Rectangle userBox = { 440, 250, 400, 50 };
static Rectangle passBox = { 440, 350, 400, 50 };
static Rectangle btnLogin = { 440, 450, 190, 50 };
static Rectangle btnRegister = { 650, 450, 190, 50 };

static int activeBox = 0;
int loginResult = 0;
int goToRegister = 0;

static char errorMsg[64] = {0};
static float errorTimer = 0.0f;

static Texture2D logo = {0};

void InitLoginScreen(void) {
    memset(username, 0, 20);
    memset(password, 0, 20);
    memset(errorMsg, 0, 64); 
    activeBox = 0;
    loginResult = 0;
    goToRegister = 0;
    errorTimer = 0.0f;
    
    if (logo.id == 0) {
        logo = LoadTexture("src/resources/icon.png");
    }
}

void UpdateLoginScreen(void) {
    Vector2 mouse = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        activeBox = 0;
        if (CheckCollisionPointRec(mouse, userBox)) activeBox = 1;
        else if (CheckCollisionPointRec(mouse, passBox)) activeBox = 2;
    }

    char* target = (activeBox == 1) ? username : (activeBox == 2) ? password : NULL;
    if (target) {
        int key = GetCharPressed();
        while (key > 0) {
            int len = TextLength(target);
            if (len < 19) { target[len] = (char)key; target[len+1] = '\0'; }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE)) {
            int len = TextLength(target);
            if (len > 0) target[len-1] = '\0';
        }
    }

    if (DrawStyledButton(btnLogin, "Iniciar Sesión")) {
        if (TextLength(username) == 0 || TextLength(password) == 0) {
            sprintf(errorMsg, "Por favor llena todos los campos");
            errorTimer = 3.0f;
        } 
        else {
            if (verificarCredenciales(username, password)) {
                strcpy(g_loggedInUser, username);
                loginResult = 1; 
            } else {
                sprintf(errorMsg, "Usuario o contraseña incorrectos");
                errorTimer = 3.0f;
            }
        }
    }
    
    if (DrawStyledButton(btnRegister, "Crear Cuenta")) {
        goToRegister = 1;
    }

    if (errorTimer > 0) {
        errorTimer -= GetFrameTime();
        if (errorTimer <= 0) memset(errorMsg, 0, 64);
    }
}

void DrawLoginScreen(void) {
    ClearBackground(COL_BG);

    if (logo.id > 0) {
        float scale = 60.0f / (float)logo.height; 
        DrawTextureEx(logo, (Vector2){20, 20}, 0.0f, scale, WHITE);
        DrawText("TicketMaster", 90, 35, 30, COL_TEXT_WHITE);
    } else {
        DrawText("TM", 20, 20, 40, COL_PRIMARY);
    }

    DrawText("Bienvenido de nuevo", 530, 160, 20, COL_TEXT_GRAY);

    DrawText("Usuario", userBox.x, userBox.y - 25, 20, COL_TEXT_GRAY);
    DrawStyledInput(userBox, username, activeBox == 1, false);

    DrawText("Contraseña", passBox.x, passBox.y - 25, 20, COL_TEXT_GRAY);
    DrawStyledInput(passBox, password, activeBox == 2, true);

    if (errorTimer > 0) {
        int textW = MeasureText(errorMsg, 20);
        DrawText(errorMsg, (GetScreenWidth() - textW)/2, 410, 20, COL_ERROR);
    }

}

void UnloadLoginScreen(void) {
    if (logo.id > 0) UnloadTexture(logo);
    logo.id = 0; 
}