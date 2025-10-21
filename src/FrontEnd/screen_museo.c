#include "raylib.h"
#include "screen_museo.h"
#include "../BackEnd/museo.h" 

static int museoSeleccionado = 0;
static int cantidadBoletos = 1;
static float precioUnitario = 0.0f;


int museoScreenResult = 0;
float museoTotalCompra = 0.0f;


static Rectangle museoLouvreButton = { 100, 150, 350, 50 };
static Rectangle museoMetButton = { 100, 220, 350, 50 };
static Rectangle museoVaticanoButton = { 100, 290, 350, 50 };
static Rectangle museoAntropologiaButton = { 100, 360, 350, 50 };
static Rectangle museoCatalunyaButton = { 100, 430, 350, 50 };


static Rectangle minusButton = { 800, 250, 60, 60 };
static Rectangle plusButton = { 1020, 250, 60, 60 };


static Rectangle comprarButton = { 930, 600, 250, 60 };
static Rectangle volverButton = { 100, 600, 250, 60 };

void InitMuseoScreen(void) {
    museoSeleccionado = 0;
    cantidadBoletos = 1;
    precioUnitario = 0.0f;
    museoScreenResult = 0;
    museoTotalCompra = 0.0f;
}

void UpdateMuseoScreen(void) {
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        // Selección de Museo
        if (CheckCollisionPointRec(GetMousePosition(), museoLouvreButton)) museoSeleccionado = 1;
        if (CheckCollisionPointRec(GetMousePosition(), museoMetButton)) museoSeleccionado = 2;
        if (CheckCollisionPointRec(GetMousePosition(), museoVaticanoButton)) museoSeleccionado = 3;
        if (CheckCollisionPointRec(GetMousePosition(), museoAntropologiaButton)) museoSeleccionado = 4;
        if (CheckCollisionPointRec(GetMousePosition(), museoCatalunyaButton)) museoSeleccionado = 5;

        // Selector de Cantidad
        if (CheckCollisionPointRec(GetMousePosition(), minusButton) && cantidadBoletos > 1) cantidadBoletos--;
        if (CheckCollisionPointRec(GetMousePosition(), plusButton) && cantidadBoletos < 10) cantidadBoletos++;

        // Botones de Acción
        if (CheckCollisionPointRec(GetMousePosition(), volverButton)) museoScreenResult = -1;
        if (CheckCollisionPointRec(GetMousePosition(), comprarButton) && museoSeleccionado > 0) {
            museoScreenResult = 1;
        }
    }

    // --- Lógica de Cálculo (llamando al BackEnd) ---
    precioUnitario = getPrecioMuseo(museoSeleccionado);
    museoTotalCompra = precioUnitario * cantidadBoletos;
}

void DrawMuseoScreen(void) {
    ClearBackground(RAYWHITE);
    DrawText("VENTA DE BOLETOS - MUSEO", GetScreenWidth() / 2 - MeasureText("VENTA DE BOLETOS - MUSEO", 40) / 2, 40, 40, DARKGRAY);

    // --- Columna Izquierda ---
    DrawText("1. Seleccione el Museo:", 100, 120, 20, GRAY);
    const char* museos[] = {"Museo de Louvre", "Museo Metropolitano de Arte", "Museos Vaticanos", "Museo de Antropologia", "Museo de Arte de Catalunya"};
    Rectangle botonesMuseo[] = {museoLouvreButton, museoMetButton, museoVaticanoButton, museoAntropologiaButton, museoCatalunyaButton};
    for (int i = 0; i < 5; i++) {
        bool seleccionado = (museoSeleccionado == i + 1);
        DrawRectangleRec(botonesMuseo[i], seleccionado ? MAROON : LIGHTGRAY);
        DrawText(museos[i], botonesMuseo[i].x + 20, botonesMuseo[i].y + 15, 20, seleccionado ? WHITE : BLACK);
    }

    // --- Columna Derecha ---
    DrawText("2. Cantidad:", 800, 220, 20, GRAY);
    DrawRectangleRec(minusButton, LIGHTGRAY);
    DrawText("-", minusButton.x + 22, minusButton.y + 10, 40, BLACK);
    DrawRectangleRec(plusButton, LIGHTGRAY);
    DrawText("+", plusButton.x + 20, plusButton.y + 10, 40, BLACK);
    DrawText(TextFormat("%02i", cantidadBoletos), 920, 260, 40, MAROON);

    DrawText("TOTAL:", 800, 380, 40, GRAY);
    DrawText(TextFormat("$%.2f", museoTotalCompra), 950, 380, 40, MAROON);

    // --- Información Adicional (Restricciones) ---
    if (museoSeleccionado > 0) {
        const char* restricciones = getRestriccionesMuseo(museoSeleccionado);
        DrawText("Restricciones:", 600, 450, 20, DARKGRAY);
        DrawText(restricciones, 600, 480, 20, GRAY);
    }

    // --- Acciones ---
    DrawRectangleRec(volverButton, DARKGRAY);
    DrawText("Volver al Menu", volverButton.x + 55, volverButton.y + 20, 20, WHITE);
    
    Color comprarColor = (museoSeleccionado > 0) ? GREEN : GRAY;
    DrawRectangleRec(comprarButton, comprarColor);
    DrawText("Proceder al Pago", comprarButton.x + 45, comprarButton.y + 20, 20, WHITE);
}

void UnloadMuseoScreen(void) {
    // Vacío por ahora
}