#include "../../include/raylib.h"
#include "screen_register.h"
#include "ui_styles.h"
#include "../BackEnd/register.h"
#include <string.h>
#include <stdio.h>

static char correo[40] = {0};
static char usuario[20] = {0};
static char pass[20] = {0};
static char confirm[20] = {0};

static Rectangle boxCorreo  = { 340, 180, 600, 50 };
static Rectangle boxUser    = { 340, 270, 600, 50 };
static Rectangle boxPass    = { 340, 360, 280, 50 };
static Rectangle boxConfirm = { 660, 360, 280, 50 };
static Rectangle btnCrear   = { 660, 450, 280, 50 };
static Rectangle btnVolver  = { 340, 450, 200, 50 };

static int activeBox = 0;
int registerScreenResult = 0;

static char errorMsg[128] = {0};
static float errorTimer = 0.0f;

void InitRegisterScreen(void) {
    memset(correo, 0, 40);
    memset(usuario, 0, 20);
    memset(pass, 0, 20);
    memset(confirm, 0, 20);
    memset(errorMsg, 0, 128);
    activeBox = 0;
    registerScreenResult = 0;
    errorTimer = 0.0f;
}

void UpdateRegisterScreen(void) {
    Vector2 mouse = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        activeBox = 0;
        if (CheckCollisionPointRec(mouse, boxCorreo))  activeBox = 1;
        else if (CheckCollisionPointRec(mouse, boxUser))    activeBox = 2;
        else if (CheckCollisionPointRec(mouse, boxPass))    activeBox = 3;
        else if (CheckCollisionPointRec(mouse, boxConfirm)) activeBox = 4;
    }

    char* target = NULL;
    int maxLen = 0;
    if (activeBox == 1) { target = correo; maxLen = 39; }
    if (activeBox == 2) { target = usuario; maxLen = 19; }
    if (activeBox == 3) { target = pass; maxLen = 19; }
    if (activeBox == 4) { target = confirm; maxLen = 19; }

    if (target) {
        int key = GetCharPressed();
        while (key > 0) {
            int len = TextLength(target);
            if (len < maxLen) { target[len] = (char)key; target[len+1] = '\0'; }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE)) {
            int len = TextLength(target);
            if (len > 0) target[len-1] = '\0';
        }
        if (IsKeyPressed(KEY_TAB)) {
            activeBox++;
            if (activeBox > 4) activeBox = 1;
        }
    }

    if (DrawStyledButton(btnCrear, "Registrarse")) {
        
        if (TextLength(correo) == 0 || TextLength(usuario) == 0 || TextLength(pass) == 0) {
            sprintf(errorMsg, "Error: Todos los campos son obligatorios");
            errorTimer = 3.0f;
        }
        else if (strcmp(pass, confirm) != 0) {
            sprintf(errorMsg, "Error: Las contraseñas no coinciden");
            errorTimer = 3.0f;
        }
        else if (!validarContrasena(pass)) {
            sprintf(errorMsg, "La contraseña debe contener: 6-12 caracteres, Letra + Numero + Simbolo");
            errorTimer = 5.0f; 
        }
        else if (!emailValido(correo)) {
            sprintf(errorMsg, "Error: Formato de correo inválido (ej: a@b.com)");
            errorTimer = 3.0f;
        }
        else if (!validarUsuario(usuario)) {
            sprintf(errorMsg, "Error: Usuario debe ser alfanumérico (4-15 chars)");
            errorTimer = 3.0f;
        }
        else {
            int resultado = registrarUsuario(correo, usuario, pass, confirm);
            
            if (resultado == 1) {
                registerScreenResult = 1; 
            } 
            else if (resultado == -1) {
                sprintf(errorMsg, "Error: Usuario o correo ya registrado");
                errorTimer = 3.0f;
            } 
            else {
                sprintf(errorMsg, "Error en la base de datos");
                errorTimer = 5.0f;
            }
        }
    }

    if (DrawStyledButton(btnVolver, "Cancelar")) {
        registerScreenResult = -1;
    }
    
    if (errorTimer > 0) errorTimer -= GetFrameTime();
}

void DrawRegisterScreen(void) {
    ClearBackground(COL_BG);

    DrawText("Crear Nueva Cuenta", 340, 100, 30, COL_TEXT_WHITE);
    DrawText("Únete a TicketMasterPS", 340, 140, 20, COL_TEXT_GRAY);

    DrawText("Correo Electrónico", boxCorreo.x, boxCorreo.y - 25, 18, COL_TEXT_GRAY);
    DrawStyledInput(boxCorreo, correo, activeBox == 1, false);

    DrawText("Nombre de Usuario", boxUser.x, boxUser.y - 25, 18, COL_TEXT_GRAY);
    DrawStyledInput(boxUser, usuario, activeBox == 2, false);

    DrawText("Contraseña", boxPass.x, boxPass.y - 25, 18, COL_TEXT_GRAY);
    DrawStyledInput(boxPass, pass, activeBox == 3, true);

    DrawText("Confirmar Contraseña", boxConfirm.x, boxConfirm.y - 25, 18, COL_TEXT_GRAY);
    DrawStyledInput(boxConfirm, confirm, activeBox == 4, true);

    if (errorTimer > 0) {
        int textW = MeasureText(errorMsg, 20);
        DrawText(errorMsg, (GetScreenWidth() - textW)/2, 520, 20, COL_ERROR);
        DrawCircle((GetScreenWidth() - textW)/2 - 20, 530, 10, COL_ERROR);
        DrawText("!", (GetScreenWidth() - textW)/2 - 23, 522, 20, WHITE);
    }
}

void UnloadRegisterScreen(void) {}
int RegisterHitTest(Vector2 mouse) { return 0; }