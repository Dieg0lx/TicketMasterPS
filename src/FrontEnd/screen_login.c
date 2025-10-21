#include "raylib.h"
#include "../BackEnd/Login.h" 
#include "screen_login.h"
#include <string.h>

// --- Variables locales del módulo ---
static char username[20] = {0};
static char password[20] = {0};
static int userLetterCount = 0;
static int passLetterCount = 0;

static Rectangle userBox = { 440, 250, 400, 50 };
static Rectangle passBox = { 440, 350, 400, 50 };
static Rectangle loginButton = { 565, 450, 150, 50 };

static int activeBox = 0; // 0: none, 1: userBox, 2: passBox
static char errorMessage[100] = {0};
static int framesCounter = 0;

int loginResult = 0;

void InitLoginScreen(void) {
    // Reiniciar todos los valores
    strcpy(username, "");
    strcpy(password, "");
    strcpy(errorMessage, "");
    userLetterCount = 0;
    passLetterCount = 0;
    activeBox = 0;
    loginResult = 0;
}

// --- FUNCIÓN CORREGIDA ---
void UpdateLoginScreen(void) {
    framesCounter++;

    // Lógica para activar las cajas de texto
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if(CheckCollisionPointRec(GetMousePosition(), userBox)) {
            activeBox = 1;
        } else if (CheckCollisionPointRec(GetMousePosition(), passBox)) {
            activeBox = 2;
        } else {
            activeBox = 0;
        }
    }

    // Captura de texto (ahora dentro de la función)
    int key = GetCharPressed(); // La variable 'key' se declara aquí

    if (activeBox == 1) { // Usuario
        if ((key >= 32 && key <= 125) && (userLetterCount < 19)) {
            username[userLetterCount++] = (char)key;
        }
        if (IsKeyPressed(KEY_BACKSPACE) && userLetterCount > 0) {
            username[--userLetterCount] = '\0';
        }
    } else if (activeBox == 2) { // Contraseña
        if ((key >= 32 && key <= 125) && (passLetterCount < 19)) {
            password[passLetterCount++] = (char)key;
        }
        if (IsKeyPressed(KEY_BACKSPACE) && passLetterCount > 0) {
            password[--passLetterCount] = '\0';
        }
    }

    // Lógica del botón (ahora dentro de la función)
    if (CheckCollisionPointRec(GetMousePosition(), loginButton) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        if (!validarUsuario(username)) {
            strcpy(errorMessage, "Error: Usuario debe tener 4-10 caracteres alfanumericos.");
        } else if (!validarContrasena(password)) {
            strcpy(errorMessage, "Error: Contraseña debe tener 8-16 caracteres.");
        } else if (verificarCredenciales(username, password)) {
            loginResult = 1; // ¡Éxito!
        } else {
            strcpy(errorMessage, "Error: Usuario o contraseña incorrectos.");
        }
    }
} // <-- La función UpdateLoginScreen termina aquí correctamente

void DrawLoginScreen(void) {
    ClearBackground(RAYWHITE);
    DrawText("INICIO DE SESION", GetScreenWidth() / 2 - MeasureText("INICIO DE SESION", 40) / 2, 150, 40, DARKGRAY);

    // Cajas de texto
    DrawText("Usuario:", userBox.x, userBox.y - 25, 20, GRAY);
    DrawRectangleRec(userBox, LIGHTGRAY);
    if (activeBox == 1) DrawRectangleLinesEx(userBox, 2, MAROON);
    DrawText(username, (int)userBox.x + 10, (int)userBox.y + 25, 20, MAROON);

    DrawText("Contraseña:", passBox.x, passBox.y - 25, 20, GRAY);
    DrawRectangleRec(passBox, LIGHTGRAY);
    if (activeBox == 2) DrawRectangleLinesEx(passBox, 2, MAROON);
    char passAsterisks[21] = {0};
    for(int i=0; i < passLetterCount; i++) passAsterisks[i] = '*';
    DrawText(passAsterisks, (int)passBox.x + 10, (int)passBox.y + 10, 20, MAROON);

    // Cursor parpadeante
    if ((framesCounter / 30) % 2 == 0) {
        if (activeBox == 1) DrawText("|", (int)userBox.x + 12 + MeasureText(username, 20), (int)userBox.y + 12, 20, MAROON);
        if (activeBox == 2) DrawText("|", (int)passBox.x + 12 + MeasureText(passAsterisks, 20), (int)passBox.y + 12, 20, MAROON);
    }

    // Botón y mensaje de error
    DrawRectangleRec(loginButton, MAROON);
    DrawText("Ingresar", loginButton.x + loginButton.width / 2 - MeasureText("Ingresar", 20) / 2, loginButton.y + 15, 20, WHITE);
    DrawText(errorMessage, GetScreenWidth() / 2 - MeasureText(errorMessage, 20) / 2, 520, 20, RED);
}

void UnloadLoginScreen(void) {
    // No hay texturas que descargar, así que esta función está vacía por ahora.
}