#include "raylib.h"
#include "screen_pago.h"
#include "../BackEnd/procesoPago.h" // Incluimos el backend para las validaciones
#include <string.h>

// --- Variables de estado de la pantalla ---
static float montoAPagar = 0.0f;
int pagoScreenResult = 0;

// --- Variables para los campos de texto ---
static DatosPago datos; // Usamos la estructura del backend
static char errorMessage[100] = {0};
static int activeBox = 0; // 0: none, 1: tarjeta, 2: nombre, 3: fecha, 4: cvv
static int framesCounter = 0;

// --- Definición de la UI ---
static Rectangle boxTarjeta = { 340, 180, 600, 50 };
static Rectangle boxNombre = { 340, 280, 600, 50 };
static Rectangle boxFecha = { 340, 380, 280, 50 };
static Rectangle boxCvv = { 660, 380, 280, 50 };
static Rectangle pagarButton = { 660, 550, 280, 60 };
static Rectangle cancelarButton = { 340, 550, 280, 60 };

void InitPagoScreen(float monto) {
    montoAPagar = monto;
    pagoScreenResult = 0;
    strcpy(errorMessage, "");
    
    // Limpiar la estructura de datos
    memset(&datos, 0, sizeof(DatosPago));
    
    activeBox = 0;
    framesCounter = 0;
}

void UpdatePagoScreen(void) {
    // Lógica para cambiar de caja de texto activa
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), boxTarjeta)) activeBox = 1;
        else if (CheckCollisionPointRec(GetMousePosition(), boxNombre)) activeBox = 2;
        else if (CheckCollisionPointRec(GetMousePosition(), boxFecha)) activeBox = 3;
        else if (CheckCollisionPointRec(GetMousePosition(), boxCvv)) activeBox = 4;
        else activeBox = 0;
    }

    // Lógica para capturar texto
    if (activeBox > 0) {
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32) && (key <= 125)) {
                if (activeBox == 1 && strlen(datos.numeroTarjeta) < 16) strcat(datos.numeroTarjeta, (char*)&key);
                if (activeBox == 2 && strlen(datos.nombreTitular) < 49) strcat(datos.nombreTitular, (char*)&key);
                if (activeBox == 3 && strlen(datos.fechaVencimiento) < 5) strcat(datos.fechaVencimiento, (char*)&key);
                if (activeBox == 4 && strlen(datos.cvv) < 4) strcat(datos.cvv, (char*)&key);
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (activeBox == 1 && strlen(datos.numeroTarjeta) > 0) datos.numeroTarjeta[strlen(datos.numeroTarjeta) - 1] = '\0';
            if (activeBox == 2 && strlen(datos.nombreTitular) > 0) datos.nombreTitular[strlen(datos.nombreTitular) - 1] = '\0';
            if (activeBox == 3 && strlen(datos.fechaVencimiento) > 0) datos.fechaVencimiento[strlen(datos.fechaVencimiento) - 1] = '\0';
            if (activeBox == 4 && strlen(datos.cvv) > 0) datos.cvv[strlen(datos.cvv) - 1] = '\0';
        }
    }
    framesCounter++;

    // Lógica de los botones
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), cancelarButton)) {
            pagoScreenResult = -1; // Cancelado
        }
        if (CheckCollisionPointRec(GetMousePosition(), pagarButton)) {
            // --- CONEXIÓN CON EL BACKEND ---
            if (!validarNumeroTarjeta(datos.numeroTarjeta)) strcpy(errorMessage, "Error: Tarjeta debe tener 16 digitos.");
            else if (!validarNombreTitular(datos.nombreTitular)) strcpy(errorMessage, "Error: Nombre no valido.");
            else if (!validarFechaVencimiento(datos.fechaVencimiento)) strcpy(errorMessage, "Error: Fecha debe ser MM/AA.");
            else if (!validarCVV(datos.cvv)) strcpy(errorMessage, "Error: CVV debe tener 3 o 4 digitos.");
            else {
                // Si todo es válido, intentamos ejecutar el pago
                if (ejecutarPago(&datos, montoAPagar)) {
                    pagoScreenResult = 1; // ¡Éxito!
                } else {
                    strcpy(errorMessage, "Error: El pago fue rechazado.");
                }
            }
        }
    }
}

void DrawPagoScreen(void) {
    ClearBackground(RAYWHITE);
    DrawText("PROCESO DE PAGO", GetScreenWidth() / 2 - MeasureText("PROCESO DE PAGO", 40) / 2, 60, 40, DARKGRAY);
    DrawText(TextFormat("MONTO A PAGAR: $%.2f", montoAPagar), GetScreenWidth() / 2 - MeasureText(TextFormat("MONTO A PAGAR: $%.2f", montoAPagar), 30) / 2, 120, 30, MAROON);

    // Dibujar cajas de texto
    DrawRectangleRec(boxTarjeta, LIGHTGRAY);
    DrawRectangleRec(boxNombre, LIGHTGRAY);
    DrawRectangleRec(boxFecha, LIGHTGRAY);
    DrawRectangleRec(boxCvv, LIGHTGRAY);

    // Dibujar bordes de caja activa
    if (activeBox == 1) DrawRectangleLinesEx(boxTarjeta, 2, MAROON);
    if (activeBox == 2) DrawRectangleLinesEx(boxNombre, 2, MAROON);
    if (activeBox == 3) DrawRectangleLinesEx(boxFecha, 2, MAROON);
    if (activeBox == 4) DrawRectangleLinesEx(boxCvv, 2, MAROON);

    // Dibujar texto dentro de las cajas
    DrawText(datos.numeroTarjeta, boxTarjeta.x + 5, boxTarjeta.y + 10, 20, BLACK);
    DrawText(datos.nombreTitular, boxNombre.x + 5, boxNombre.y + 10, 20, BLACK);
    DrawText(datos.fechaVencimiento, boxFecha.x + 5, boxFecha.y + 10, 20, BLACK);
    DrawText(datos.cvv, boxCvv.x + 5, boxCvv.y + 10, 20, BLACK);
    
    // Dibujar etiquetas
    DrawText("Numero de Tarjeta:", boxTarjeta.x, boxTarjeta.y - 20, 10, GRAY);
    DrawText("Nombre del Titular:", boxNombre.x, boxNombre.y - 20, 10, GRAY);
    DrawText("Vencimiento (MM/AA):", boxFecha.x, boxFecha.y - 20, 10, GRAY);
    DrawText("CVV:", boxCvv.x, boxCvv.y - 20, 10, GRAY);

    // Dibujar cursor parpadeante
    if ((framesCounter / 30) % 2 == 0) {
        if (activeBox == 1) DrawText("|", boxTarjeta.x + 8 + MeasureText(datos.numeroTarjeta, 20), boxTarjeta.y + 12, 20, MAROON);
        if (activeBox == 2) DrawText("|", boxNombre.x + 8 + MeasureText(datos.nombreTitular, 20), boxNombre.y + 12, 20, MAROON);
        if (activeBox == 3) DrawText("|", boxFecha.x + 8 + MeasureText(datos.fechaVencimiento, 20), boxFecha.y + 12, 20, MAROON);
        if (activeBox == 4) DrawText("|", boxCvv.x + 8 + MeasureText(datos.cvv, 20), boxCvv.y + 12, 20, MAROON);
    }

    // Dibujar botones y mensaje de error
    DrawRectangleRec(cancelarButton, DARKGRAY);
    DrawText("Cancelar", cancelarButton.x + 50, cancelarButton.y + 15, 20, WHITE);
    DrawRectangleRec(pagarButton, GREEN);
    DrawText("Pagar", pagarButton.x + 60, pagarButton.y + 15, 20, WHITE);
    DrawText(errorMessage, GetScreenWidth() / 2 - MeasureText(errorMessage, 20) / 2, 480, 20, RED);
}

void UnloadPagoScreen(void) {
    // No hay nada que liberar por ahora
}