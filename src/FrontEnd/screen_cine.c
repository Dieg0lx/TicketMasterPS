#include "raylib.h"
#include "screen_cine.h"
#include "../BackEnd/cine.h"
#include "screen_pago.h" 

// --- Variables de estado de la pantalla ---
static int cineSeleccionado = 0;    // 1: Cinemark, 2: Cinepolis, 3: Cinemex
static int peliculaSeleccionada = 0; // 1: Pelicula A, 2: Pelicula B
static int servicioSeleccionado = 0; // 1: Tradicional, 2: PLUUS, etc.
static int cantidadBoletos = 1;
static float precioUnitario = 0.0f;

// Variables de comunicación con main.c
int cineScreenResult = 0;
float cineTotalCompra = 0.0f;

// --- Definición de la UI (Rectángulos) ---
// Columna Izquierda: Selección de Cine y Película
static Rectangle cinemarkButton = { 100, 150, 250, 50 };
static Rectangle cinepolisButton = { 100, 220, 250, 50 };
static Rectangle cinemexButton = { 100, 290, 250, 50 };

static Rectangle peliculaAButton = { 100, 420, 250, 50 };
static Rectangle peliculaBButton = { 100, 490, 250, 50 };

// Columna Derecha: Servicios, Cantidad y Total
static Rectangle servicioTradicionalButton = { 500, 150, 200, 40 };
static Rectangle servicioPluusButton = { 500, 200, 200, 40 };
static Rectangle servicioVipButton = { 500, 250, 200, 40 };
static Rectangle servicioJuniorButton = { 720, 150, 200, 40 };
static Rectangle servicio4dxButton = { 720, 200, 200, 40 };
static Rectangle servicioImaxButton = { 720, 250, 200, 40 };

static Rectangle minusButton = { 800, 400, 60, 60 };
static Rectangle plusButton = { 1020, 400, 60, 60 };

// Acciones
static Rectangle comprarButton = { 930, 600, 250, 60 };
static Rectangle volverButton = { 100, 600, 250, 60 };

void InitCineScreen(void) {
    cineSeleccionado = 0;
    peliculaSeleccionada = 0;
    servicioSeleccionado = 0;
    cantidadBoletos = 1;
    precioUnitario = 0.0f;
    cineScreenResult = 0;
    cineTotalCompra = 0.0f;
}

void UpdateCineScreen(void) {

    // --- Lógica de Cálculo (MOVIDA AL INICIO) ---
    // Calculamos el precio ANTES de manejar los clics para asegurar
    // que el total esté actualizado si se presiona "Comprar".
    precioUnitario = getPrecioServicioCine(servicioSeleccionado);
    cineTotalCompra = precioUnitario * cantidadBoletos;

    // --- Lógica de Clics ---
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        // Selección de Cine
        if (CheckCollisionPointRec(GetMousePosition(), cinemarkButton)) cineSeleccionado = 1;
        if (CheckCollisionPointRec(GetMousePosition(), cinepolisButton)) cineSeleccionado = 2;
        if (CheckCollisionPointRec(GetMousePosition(), cinemexButton)) cineSeleccionado = 3;

        // Selección de Película
        if (CheckCollisionPointRec(GetMousePosition(), peliculaAButton)) peliculaSeleccionada = 1;
        if (CheckCollisionPointRec(GetMousePosition(), peliculaBButton)) peliculaSeleccionada = 2;

        // Selección de Servicio (solo si el cine está seleccionado y el servicio está disponible)
        if (cineSeleccionado > 0) {
            if (esServicioDisponibleCine(cineSeleccionado, 1) && CheckCollisionPointRec(GetMousePosition(), servicioTradicionalButton)) servicioSeleccionado = 1;
            if (esServicioDisponibleCine(cineSeleccionado, 2) && CheckCollisionPointRec(GetMousePosition(), servicioPluusButton)) servicioSeleccionado = 2;
            if (esServicioDisponibleCine(cineSeleccionado, 3) && CheckCollisionPointRec(GetMousePosition(), servicioVipButton)) servicioSeleccionado = 3;
            if (esServicioDisponibleCine(cineSeleccionado, 4) && CheckCollisionPointRec(GetMousePosition(), servicioJuniorButton)) servicioSeleccionado = 4;
            if (esServicioDisponibleCine(cineSeleccionado, 5) && CheckCollisionPointRec(GetMousePosition(), servicio4dxButton)) servicioSeleccionado = 5;
            if (esServicioDisponibleCine(cineSeleccionado, 6) && CheckCollisionPointRec(GetMousePosition(), servicioImaxButton)) servicioSeleccionado = 6;
        }

        // Selector de Cantidad
        if (CheckCollisionPointRec(GetMousePosition(), minusButton) && cantidadBoletos > 1) cantidadBoletos--;
        if (CheckCollisionPointRec(GetMousePosition(), plusButton) && cantidadBoletos < 10) cantidadBoletos++;

        // Botones de Acción
        if (CheckCollisionPointRec(GetMousePosition(), volverButton)) cineScreenResult = -1;
        
        // --- ¡AQUÍ ESTÁ LA MODIFICACIÓN! ---
        if (CheckCollisionPointRec(GetMousePosition(), comprarButton) && cineSeleccionado > 0 && peliculaSeleccionada > 0 && servicioSeleccionado > 0) {
            
            // 1. (NUEVO) Llamamos a InitPagoScreen con los datos correctos
            InitPagoScreen("CINE", servicioSeleccionado, cineTotalCompra);

            // 2. (Tu código) Le decimos a main.c que cambie de pantalla
            cineScreenResult = 1;
        }
    }
}

void DrawCineScreen(void) {
    ClearBackground(RAYWHITE);
    DrawText("VENTA DE BOLETOS - CINE", GetScreenWidth() / 2 - MeasureText("VENTA DE BOLETOS - CINE", 40) / 2, 40, 40, DARKGRAY);

    // --- Columna Izquierda ---
    DrawText("1. Seleccione el Cine:", 100, 120, 20, GRAY);
    DrawRectangleRec(cinemarkButton, cineSeleccionado == 1 ? MAROON : LIGHTGRAY);
    DrawText("Cinemark", cinemarkButton.x + 80, cinemarkButton.y + 15, 20, cineSeleccionado == 1 ? WHITE : BLACK);
    DrawRectangleRec(cinepolisButton, cineSeleccionado == 2 ? MAROON : LIGHTGRAY);
    DrawText("Cinepolis", cinepolisButton.x + 80, cinepolisButton.y + 15, 20, cineSeleccionado == 2 ? WHITE : BLACK);
    DrawRectangleRec(cinemexButton, cineSeleccionado == 3 ? MAROON : LIGHTGRAY);
    DrawText("Cinemex", cinemexButton.x + 85, cinemexButton.y + 15, 20, cineSeleccionado == 3 ? WHITE : BLACK);

    DrawText("2. Seleccione la Pelicula:", 100, 390, 20, GRAY);
    DrawRectangleRec(peliculaAButton, peliculaSeleccionada == 1 ? MAROON : LIGHTGRAY);
    DrawText("Dune: Part Two", peliculaAButton.x + 55, peliculaAButton.y + 15, 20, peliculaSeleccionada == 1 ? WHITE : BLACK);
    DrawRectangleRec(peliculaBButton, peliculaSeleccionada == 2 ? MAROON : LIGHTGRAY);
    DrawText("Godzilla x Kong", peliculaBButton.x + 50, peliculaBButton.y + 15, 20, peliculaSeleccionada == 2 ? WHITE : BLACK);

    // --- Columna Derecha ---
    DrawText("3. Seleccione el Servicio:", 500, 120, 20, cineSeleccionado > 0 ? GRAY : LIGHTGRAY);
    
    // Dibujamos los botones de servicio, pero los mostramos desactivados si no están disponibles
    const char* servicios[] = {"Tradicional", "PLUUS", "VIP", "Junior", "4DX", "IMAX"};
    Rectangle botonesServicio[] = {servicioTradicionalButton, servicioPluusButton, servicioVipButton, servicioJuniorButton, servicio4dxButton, servicioImaxButton};
    for (int i = 0; i < 6; i++) {
        bool disponible = cineSeleccionado > 0 && esServicioDisponibleCine(cineSeleccionado, i + 1);
        Color colorFondo = disponible ? (servicioSeleccionado == i + 1 ? GOLD : LIGHTGRAY) : Fade(LIGHTGRAY, 0.5f);
        Color colorTexto = disponible ? (servicioSeleccionado == i + 1 ? BLACK : DARKGRAY) : GRAY;
        DrawRectangleRec(botonesServicio[i], colorFondo);
        DrawText(servicios[i], botonesServicio[i].x + 20, botonesServicio[i].y + 10, 20, colorTexto);
    }

    DrawText("4. Cantidad:", 800, 370, 20, GRAY);
    DrawRectangleRec(minusButton, LIGHTGRAY);
    DrawText("-", minusButton.x + 22, minusButton.y + 10, 40, BLACK);
    DrawRectangleRec(plusButton, LIGHTGRAY);
    DrawText("+", plusButton.x + 20, plusButton.y + 10, 40, BLACK);
    DrawText(TextFormat("%02i", cantidadBoletos), 920, 410, 40, MAROON);

    DrawText("TOTAL:", 800, 500, 40, GRAY);
    DrawText(TextFormat("$%.2f", cineTotalCompra), 950, 500, 40, MAROON);

    // --- Acciones ---
    DrawRectangleRec(volverButton, DARKGRAY);
    DrawText("Volver al Menu", volverButton.x + 55, volverButton.y + 20, 20, WHITE);
    
    Color comprarColor = (cineSeleccionado > 0 && peliculaSeleccionada > 0 && servicioSeleccionado > 0) ? GREEN : GRAY;
    DrawRectangleRec(comprarButton, comprarColor);
    DrawText("Proceder al Pago", comprarButton.x + 45, comprarButton.y + 20, 20, WHITE);
}

void UnloadCineScreen(void) {
    // Vacío por ahora
}