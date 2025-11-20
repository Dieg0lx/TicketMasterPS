#include "../../include/raylib.h"
#include "screen_museo.h"
#include "ui_styles.h"
#include "screen_carrito.h" 
#include <stdio.h>

static int museoSel = 0;
static int cantidad = 1;
int museoScreenResult = 0;
float museoTotalCompra = 0.0f;
static float msgTimer = 0.0f;

const char* listaMuseos[] = { "Louvre (Paris)", "MET (NY)", "Vaticano (Roma)", "Antropología (MX)", "MNAC (BCN)" };
const float preciosMuseos[] = { 200, 180, 150, 80, 120 };

static Rectangle btnMenos = { 800, 250, 60, 60 };
static Rectangle btnMas   = { 920, 250, 60, 60 };

static Rectangle btnAgregar = { 800, 480, 250, 50 };
static Rectangle btnPagar   = { 800, 550, 250, 50 };
static Rectangle btnVolver  = { 100, 550, 150, 50 };

void InitMuseoScreen(void) {
    museoSel = 0; cantidad = 1; museoScreenResult = 0; museoTotalCompra = 0.0f; msgTimer = 0.0f;
}

void UpdateMuseoScreen(void) {
    if (msgTimer > 0) {
        msgTimer -= GetFrameTime();
        return;
    }

    Vector2 mouse = GetMousePosition();
    float startY = 150;
    for (int i = 0; i < 5; i++) {
        Rectangle itemRect = { 100, startY + (i * 60), 400, 50 };
        if (CheckCollisionPointRec(mouse, itemRect) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            museoSel = i + 1;
        }
    }

    if (DrawStyledButton(btnMenos, "-") && cantidad > 1) cantidad--;
    if (DrawStyledButton(btnMas, "+")) cantidad++;

    float precioUnitario = (museoSel > 0) ? preciosMuseos[museoSel-1] : 0;
    museoTotalCompra = precioUnitario * cantidad;

    if (museoSel > 0) {
        char titulo[50];
        sprintf(titulo, "Museo: %s", listaMuseos[museoSel-1]);

        if (DrawStyledButton(btnAgregar, "Agregar al Carrito")) {
            AgregarAlCarrito(titulo, "Entrada General", precioUnitario, cantidad);
            msgTimer = 3.0f;
            museoSel = 0; 
            cantidad = 1;
        }
        
        if (DrawStyledButton(btnPagar, TextFormat("PAGAR $%.2f >", museoTotalCompra))) {
            AgregarAlCarrito(titulo, "Entrada General", precioUnitario, cantidad);
            museoScreenResult = 2;
        }
    }

    if (DrawStyledButton(btnVolver, "Volver")) museoScreenResult = -1;
}

void DrawMuseoScreen(void) {
    ClearBackground(COL_BG);
    DrawText("Visitas Guiadas a Museos", 100, 50, 40, COL_TEXT_WHITE);
    
    float startY = 150;
    for (int i = 0; i < 5; i++) {
        Rectangle itemRect = { 100, startY + (i * 60), 400, 50 };
        bool selected = (museoSel == (i + 1));
        DrawRectangleRounded(itemRect, 0.3f, 6, selected ? COL_PRIMARY : COL_PANEL);
        DrawText(listaMuseos[i], itemRect.x + 20, itemRect.y + 15, 20, COL_TEXT_WHITE);
        DrawText(TextFormat("$%.0f", preciosMuseos[i]), itemRect.x + 320, itemRect.y + 15, 20, COL_SUCCESS);
    }

    DrawText(TextFormat("%02d", cantidad), 870, 265, 30, COL_TEXT_WHITE);
    
    if (msgTimer > 0) DrawNotification("¡Agregado al Carrito!", msgTimer);
}

void UnloadMuseoScreen(void) {}