#include "raylib.h"
#include "../BackEnd/Login.h"
#include "screen_register.h"
#include <string.h>
#include <ctype.h>

static char correo[40] = {0};
static char usuario[20] = {0};
static char pass[20] = {0};
static char confirm[20] = {0};

static int correoCount = 0;
static int usuarioCount = 0;
static int passCount = 0;
static int confirmCount = 0;


static Rectangle correoBox  = { 440, 200, 400, 50 }; // Y: 270 -> 200
static Rectangle userBox    = { 440, 270, 400, 50 }; // Y: 340 -> 270
static Rectangle passBox    = { 440, 340, 400, 50 }; // Y: 410 -> 340
static Rectangle confirmBox = { 440, 410, 400, 50 }; // Y: 480 -> 410
static Rectangle crearButton = { 565, 480, 150, 50 }; // Y: 550 -> 480
static Rectangle volverButton = { 735, 480, 150, 50 }; // Y: 550 -> 480

static int activeBox = 0;
static int framesCounter = 0;
static char errorMessage[128] = {0};

int registerScreenResult = 0;

void InitRegisterScreen(void) {
    strcpy(correo, "");
    strcpy(usuario, "");
    strcpy(pass, "");
    strcpy(confirm, "");
    correoCount = usuarioCount = passCount = confirmCount = 0;
    activeBox = 0;
    framesCounter = 0;
    strcpy(errorMessage, "");
    registerScreenResult = 0;
}

void UpdateRegisterScreen(void) {
    framesCounter++;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 m = GetMousePosition();
        if (CheckCollisionPointRec(m, correoBox)) activeBox = 1;  // Era 2
        else if (CheckCollisionPointRec(m, userBox)) activeBox = 2;   // Era 3
        else if (CheckCollisionPointRec(m, passBox)) activeBox = 3;   // Era 4
        else if (CheckCollisionPointRec(m, confirmBox)) activeBox = 4; // Era 5
        else activeBox = 0;
    }

    int key = GetCharPressed();
    if (activeBox == 1) {
        if ((key >= 32 && key <= 125) && (correoCount < 39)) correo[correoCount++] = (char)key;
        if (IsKeyPressed(KEY_BACKSPACE) && correoCount > 0) correo[--correoCount] = '\0';
    } else if (activeBox == 2) {
        if (isalnum(key) && (usuarioCount < 19)) {
            usuario[usuarioCount++] = (char)key; }
        if (IsKeyPressed(KEY_BACKSPACE) && usuarioCount > 0) usuario[--usuarioCount] = '\0';
    } else if (activeBox == 3) {
        if ((key >= 32 && key <= 125) && (passCount < 19)) pass[passCount++] = (char)key;
        if (IsKeyPressed(KEY_BACKSPACE) && passCount > 0) pass[--passCount] = '\0';
    } else if (activeBox == 4) {
        if ((key >= 32 && key <= 125) && (confirmCount < 19)) confirm[confirmCount++] = (char)key;
        if (IsKeyPressed(KEY_BACKSPACE) && confirmCount > 0) confirm[--confirmCount] = '\0';
    }

    if (CheckCollisionPointRec(GetMousePosition(), crearButton) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        
        if (strchr(correo, '@') == NULL) {
            strcpy(errorMessage, "Correo invalido (debe incluir @).");
        } 
        else if (!validarUsuario(usuario)) {
            strcpy(errorMessage, "Usuario debe tener Solo caracteres numericos");
        } 
        else if (strlen(pass) < 8 || strlen(pass) > 12) {
            strcpy(errorMessage, "Contrasena debe tener 8-12 caracteres.");
        } 
        else if (strcmp(pass, confirm) != 0) {
            strcpy(errorMessage, "Las contrasenas no coinciden.");
        } else {
            int r = registrarUsuario(correo, usuario, pass, confirm);
            
            if (r == 1) {
                registerScreenResult = 1;
            } else if (r == 2) {
                strcpy(errorMessage, "Correo o usuario existente.");
            } else {
                strcpy(errorMessage, "Error al registrar.");
            }
        }
    }
    if (CheckCollisionPointRec(GetMousePosition(), volverButton) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        registerScreenResult = -1;
    }
}

void DrawRegisterScreen(void) {
    ClearBackground(RAYWHITE);
    DrawText("REGISTRO", GetScreenWidth()/2 - MeasureText("REGISTRO", 40)/2, 120, 40, DARKGRAY);


    DrawText("Correo:", correoBox.x, correoBox.y - 25, 20, GRAY);
    DrawRectangleRec(correoBox, LIGHTGRAY);
    if (activeBox == 1) DrawRectangleLinesEx(correoBox, 2, MAROON);
    DrawText(correo, (int)correoBox.x + 10, (int)correoBox.y + 15, 20, MAROON);

    DrawText("Usuario:", userBox.x, userBox.y - 25, 20, GRAY);
    DrawRectangleRec(userBox, LIGHTGRAY);
    if (activeBox == 2) DrawRectangleLinesEx(userBox, 2, MAROON);
    DrawText(usuario, (int)userBox.x + 10, (int)userBox.y + 15, 20, MAROON);

    DrawText("Contrasena:", passBox.x, passBox.y - 25, 20, GRAY);
    DrawRectangleRec(passBox, LIGHTGRAY);
    if (activeBox == 3) DrawRectangleLinesEx(passBox, 2, MAROON);
    char passA[21] = {0};
    for (int i = 0; i < passCount; i++) passA[i] = '*';
    DrawText(passA, (int)passBox.x + 10, (int)passBox.y + 15, 20, MAROON);

    DrawText("Confirmar:", confirmBox.x, confirmBox.y - 25, 20, GRAY);
    DrawRectangleRec(confirmBox, LIGHTGRAY);
    if (activeBox == 4) DrawRectangleLinesEx(confirmBox, 2, MAROON);
    char confA[21] = {0};
    for (int i = 0; i < confirmCount; i++) confA[i] = '*';
    DrawText(confA, (int)confirmBox.x + 10, (int)confirmBox.y + 15, 20, MAROON);

    if ((framesCounter / 30) % 2 == 0) {
        if (activeBox == 1) DrawText("|", (int)correoBox.x + 12 + MeasureText(correo, 20), (int)correoBox.y + 15, 20, MAROON);
        if (activeBox == 2) DrawText("|", (int)userBox.x + 12 + MeasureText(usuario, 20), (int)userBox.y + 15, 20, MAROON);
        if (activeBox == 3) DrawText("|", (int)passBox.x + 12 + MeasureText(passA, 20), (int)passBox.y + 15, 20, MAROON);
        if (activeBox == 4) DrawText("|", (int)confirmBox.x + 12 + MeasureText(confA, 20), (int)confirmBox.y + 15, 20, MAROON);
    }

    DrawRectangleRec(crearButton, MAROON);
    DrawText("Crear cuenta", crearButton.x + crearButton.width/2 - MeasureText("Crear cuenta", 20)/2, crearButton.y + 15, 20, WHITE);
    DrawRectangleRec(volverButton, DARKGRAY);
    DrawText("Volver", volverButton.x + volverButton.width/2 - MeasureText("Volver", 20)/2, volverButton.y + 15, 20, WHITE);
    
    DrawText(errorMessage, GetScreenWidth() / 2 - MeasureText(errorMessage, 20) / 2, 550, 20, RED);
}

void UnloadRegisterScreen(void) {
}

int RegisterHitTest(Vector2 mouse) {
    if (CheckCollisionPointRec(mouse, crearButton)) return 1;
    if (CheckCollisionPointRec(mouse, volverButton)) return -1;
    return 0;
}