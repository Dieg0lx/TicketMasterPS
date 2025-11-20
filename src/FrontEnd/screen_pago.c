#include "../../include/raylib.h"
#include "screen_pago.h"
#include "ui_styles.h"
#include <string.h>
#include <stdio.h>

#include "../BackEnd/notificaciones.h" 

#include "../BackEnd/procesoPago.h"

static float montoAPagar = 0.0f;
int pagoScreenResult = 0;

static char numTarjeta[20] = {0};
static char nombre[40] = {0};
static char fecha[6] = {0};
static char cvv[5] = {0}; 

static int activeBox = 0; 
static int framesCounter = 0;

static Rectangle boxTarjeta = { 340, 180, 600, 50 };
static Rectangle boxNombre  = { 340, 280, 600, 50 };
static Rectangle boxFecha   = { 340, 380, 280, 50 };
static Rectangle boxCvv     = { 660, 380, 280, 50 };
static Rectangle btnPagar   = { 660, 500, 280, 60 };
static Rectangle btnVolver  = { 340, 500, 200, 60 };

static float notifTimer = 0.0f;      
static char errorMsg[128] = {0};     
static float errorTimer = 0.0f;
static char nombreServicioActual[64] = "Compra General"; 

void InitPagoScreen(const char* tipo, int id, float total) {
    montoAPagar = total;
    pagoScreenResult = 0;
    activeBox = 0;
    notifTimer = 0.0f;
    errorTimer = 0.0f;
    memset(errorMsg, 0, 128);
    
    if (tipo != NULL) strncpy(nombreServicioActual, tipo, 63);
    else strcpy(nombreServicioActual, "Carrito de Compras");

    memset(numTarjeta, 0, 20);
    memset(nombre, 0, 40);
    memset(fecha, 0, 6);
    memset(cvv, 0, 5);
}

void UpdatePagoScreen(void) {
    framesCounter++;
    Vector2 mouse = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        activeBox = 0;
        if (CheckCollisionPointRec(mouse, boxTarjeta)) activeBox = 1;
        else if (CheckCollisionPointRec(mouse, boxNombre)) activeBox = 2;
        else if (CheckCollisionPointRec(mouse, boxFecha)) activeBox = 3;
        else if (CheckCollisionPointRec(mouse, boxCvv)) activeBox = 4;
    }

    char* target = NULL;
    int maxLen = 0;
    if (activeBox == 1) { target = numTarjeta; maxLen = 16; } 
    if (activeBox == 2) { target = nombre; maxLen = 30; }
    if (activeBox == 3) { target = fecha; maxLen = 5; }       
    if (activeBox == 4) { target = cvv; maxLen = 4; }         

    if (target) {
        int key = GetCharPressed();
        while (key > 0) {
            int len = TextLength(target);
            if (len < maxLen) {
                target[len] = (char)key;
                target[len+1] = '\0';
            }
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

    if (DrawStyledButton(btnPagar, TextFormat("PAGAR $%.2f", montoAPagar))) {
        
        if (!validarNumeroTarjeta(numTarjeta)) {
            sprintf(errorMsg, "Error: Tarjeta inválida (16 dígitos)");
            errorTimer = 3.0f;
        }
        else if (!validarNombreTitular(nombre)) {
            sprintf(errorMsg, "Error: Nombre inválido");
            errorTimer = 3.0f;
        }
        else if (!validarFechaVencimiento(fecha)) {
            sprintf(errorMsg, "Error: Fecha inválida (MM/AA)");
            errorTimer = 3.0f;
        }
        else if (!validarCVV(cvv)) {
            sprintf(errorMsg, "Error: CVV inválido");
            errorTimer = 3.0f;
        }
        else {
            notifTimer = 3.0f; 
            
            EnviarResumenWhatsApp(nombreServicioActual, 1, montoAPagar);
            
            pagoScreenResult = 1; 
        }
    }
    
    if (DrawStyledButton(btnVolver, "Cancelar")) {
        pagoScreenResult = -1;
    }

    if (notifTimer > 0) notifTimer -= GetFrameTime();
    if (errorTimer > 0) errorTimer -= GetFrameTime();
}

void DrawPagoScreen(void) {
    ClearBackground(COL_BG);

    DrawText("Pasarela de Pago Segura", 340, 100, 30, COL_TEXT_WHITE);
    DrawText(TextFormat("Concepto: %s", nombreServicioActual), 340, 140, 20, COL_PRIMARY);

    DrawText("Número de Tarjeta", boxTarjeta.x, boxTarjeta.y - 25, 20, COL_TEXT_GRAY);
    DrawStyledInput(boxTarjeta, numTarjeta, activeBox == 1, false);

    DrawText("Nombre del Titular", boxNombre.x, boxNombre.y - 25, 20, COL_TEXT_GRAY);
    DrawStyledInput(boxNombre, nombre, activeBox == 2, false);

    DrawText("Vencimiento (MM/AA)", boxFecha.x, boxFecha.y - 25, 20, COL_TEXT_GRAY);
    DrawStyledInput(boxFecha, fecha, activeBox == 3, false);

    DrawText("CVV", boxCvv.x, boxCvv.y - 25, 20, COL_TEXT_GRAY);
    DrawStyledInput(boxCvv, cvv, activeBox == 4, true); 

    if (errorTimer > 0) {
        int textW = MeasureText(errorMsg, 20);
        DrawText(errorMsg, (GetScreenWidth() - textW)/2, 460, 20, COL_ERROR);
        DrawCircle((GetScreenWidth() - textW)/2 - 20, 470, 10, COL_ERROR);
        DrawText("!", (GetScreenWidth() - textW)/2 - 23, 462, 20, WHITE);
    }

    if (notifTimer > 0) {
        DrawNotification("¡Pago Procesado! Abriendo Correo...", notifTimer > 1 ? 1.0f : notifTimer);
    }
}

void UnloadPagoScreen(void) {}