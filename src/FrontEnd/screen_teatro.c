#include "raylib.h"
#include "screen_teatro.h"
#include "../BackEnd/teatro.h" // Incluimos el backend para los precios

// --- Variables de estado de esta pantalla ---
static int obraSeleccionada = 0; // 1: Hamlet, 2: Cats
static int seccionSeleccionada = 0; // 1: VIP, 2: Platea, 3: Galeria
static int cantidadBoletos = 1;
static float precioUnitario = 0.0f;

// Variables de comunicación con main.c
int teatroScreenResult = 0;
float teatroTotalCompra = 0.0f;

// --- Definición de los elementos de la UI (Rectángulos) ---
// Obras
static Rectangle hamletButton = { 100, 150, 250, 50 };
static Rectangle catsButton = { 100, 220, 250, 50 };
// Secciones
static Rectangle vipButton = { 100, 350, 250, 50 };
static Rectangle plateaButton = { 100, 420, 250, 50 };
static Rectangle galeriaButton = { 100, 490, 250, 50 };
// Cantidad
static Rectangle minusButton = { 800, 250, 60, 60 };
static Rectangle plusButton = { 1020, 250, 60, 60 };
// Acciones
static Rectangle comprarButton = { 930, 600, 250, 60 };
static Rectangle volverButton = { 100, 600, 250, 60 };


void InitTeatroScreen(void) {
    // Reiniciamos el estado de la pantalla cada vez que entramos
    obraSeleccionada = 0;
    seccionSeleccionada = 0;
    cantidadBoletos = 1;
    precioUnitario = 0.0f;
    teatroScreenResult = 0;
    teatroTotalCompra = 0.0f;
}

void UpdateTeatroScreen(void) {
    // --- Lógica de Clics ---
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        // Selección de Obra
        if (CheckCollisionPointRec(GetMousePosition(), hamletButton)) obraSeleccionada = 1;
        if (CheckCollisionPointRec(GetMousePosition(), catsButton)) obraSeleccionada = 2;

        // Selección de Sección
        if (CheckCollisionPointRec(GetMousePosition(), vipButton)) seccionSeleccionada = 1;
        if (CheckCollisionPointRec(GetMousePosition(), plateaButton)) seccionSeleccionada = 2;
        if (CheckCollisionPointRec(GetMousePosition(), galeriaButton)) seccionSeleccionada = 3;

        // Selector de Cantidad
        if (CheckCollisionPointRec(GetMousePosition(), minusButton) && cantidadBoletos > 1) cantidadBoletos--;
        if (CheckCollisionPointRec(GetMousePosition(), plusButton) && cantidadBoletos < 10) cantidadBoletos++;

        // Botones de Acción
        if (CheckCollisionPointRec(GetMousePosition(), volverButton)) teatroScreenResult = -1; // Volver
        if (CheckCollisionPointRec(GetMousePosition(), comprarButton) && obraSeleccionada > 0 && seccionSeleccionada > 0) {
            teatroScreenResult = 1; // Comprar
        }
    }

    // --- Lógica de Cálculo (llamando al BackEnd) ---
    precioUnitario = getPrecioSeccionTeatro(seccionSeleccionada);
    teatroTotalCompra = precioUnitario * cantidadBoletos;
}

void DrawTeatroScreen(void) {
    ClearBackground(RAYWHITE);
    DrawText("VENTA DE BOLETOS - TEATRO", 150, 30, 30, DARKGRAY);

    // --- Dibujar Obras ---
    DrawText("1. Seleccione la Obra:", 100, 120, 20, GRAY);
    DrawRectangleRec(hamletButton, obraSeleccionada == 1 ? MAROON : LIGHTGRAY);
    DrawText("Hamlet", hamletButton.x + 45, hamletButton.y + 10, 20, obraSeleccionada == 1 ? WHITE : BLACK);
    DrawRectangleRec(catsButton, obraSeleccionada == 2 ? MAROON : LIGHTGRAY);
    DrawText("Cats", catsButton.x + 55, catsButton.y + 10, 20, obraSeleccionada == 2 ? WHITE : BLACK);

    // --- Dibujar Secciones ---
    DrawText("2. Seleccione la Seccion:", 100, 320, 20, GRAY);
    DrawRectangleRec(vipButton, seccionSeleccionada == 1 ? GOLD : LIGHTGRAY);
    DrawText("VIP", vipButton.x + 35, vipButton.y + 10, 20, seccionSeleccionada == 1 ? BLACK : DARKGRAY);
    DrawRectangleRec(plateaButton, seccionSeleccionada == 2 ? GOLD : LIGHTGRAY);
    DrawText("Platea", plateaButton.x + 20, plateaButton.y + 10, 20, seccionSeleccionada == 2 ? BLACK : DARKGRAY);
    DrawRectangleRec(galeriaButton, seccionSeleccionada == 3 ? GOLD : LIGHTGRAY);
    DrawText("Galeria", galeriaButton.x + 15, galeriaButton.y + 10, 20, seccionSeleccionada == 3 ? BLACK : DARKGRAY);

    // --- Dibujar Cantidad ---
    DrawText("3. Cantidad:", 800, 220, 20, GRAY);
    DrawRectangleRec(minusButton, LIGHTGRAY);
    DrawText("-", minusButton.x + 15, minusButton.y + 10, 40, BLACK);
    DrawRectangleRec(plusButton, LIGHTGRAY);
    DrawText("+", plusButton.x + 12, plusButton.y + 10, 40, BLACK);
    DrawText(TextFormat("%02i", cantidadBoletos), 920, 260, 40, MAROON);

    // --- Dibujar Total ---
    DrawText("TOTAL:", 800, 450, 20, GRAY);
    DrawText(TextFormat("$%.2f", teatroTotalCompra), 950, 450, 40, MAROON);

    // Solo mostramos la vestimenta si se ha seleccionado una obra
    if (obraSeleccionada > 0) {
        // 1. Llamamos al Back-End para obtener el texto
        const char* vestimenta = getVestimentaTeatro(obraSeleccionada);
        
        // 2. Dibujamos el texto que nos devolvió el Back-End
        DrawText(TextFormat("Vestimenta Requerida: %s", vestimenta), 800, 520, 20, DARKGRAY);
    }

    // --- Dibujar Acciones ---
    DrawRectangleRec(volverButton, DARKGRAY);
    DrawText("Volver", volverButton.x + 55, volverButton.y + 20, 20, WHITE);
    
    // El botón de comprar solo se "activa" si se ha seleccionado todo
    Color comprarColor = (obraSeleccionada > 0 && seccionSeleccionada > 0) ? GREEN : GRAY;
    DrawRectangleRec(comprarButton, comprarColor);
    DrawText("Comprar", comprarButton.x + 35, comprarButton.y + 20, 20, WHITE);
}

void UnloadTeatroScreen(void) {
    // Vacío por ahora
}