#include "../../include/raylib.h"
#include "../../include/raylib.h"
#include "screen_teatro.h"
#include "ui_styles.h"
#include "screen_carrito.h"
#include "../BackEnd/teatro.h"
#include <stdio.h>

static int obraSel = 0;      
static int seccionSel = 0;   
static int cantidad = 1;

int teatroScreenResult = 0;
float teatroTotalCompra = 0.0f;
static float msgTimer = 0.0f;

static Rectangle btnHamlet  = { 100, 150, 300, 80 };
static Rectangle btnCats    = { 100, 250, 300, 80 };
static Rectangle btnVIP     = { 450, 150, 250, 60 };
static Rectangle btnPlatea  = { 450, 230, 250, 60 };
static Rectangle btnGaleria = { 450, 310, 250, 60 };
static Rectangle btnMenos   = { 800, 200, 60, 60 };
static Rectangle btnMas     = { 920, 200, 60, 60 };

static Rectangle btnAgregar = { 800, 480, 250, 50 };
static Rectangle btnPagar   = { 800, 550, 250, 50 };
static Rectangle btnVolver  = { 100, 550, 150, 50 };

bool DrawToggleBtn(Rectangle r, const char* text, bool isSelected) {
    bool clicked = false;
    Vector2 m = GetMousePosition();
    bool hover = CheckCollisionPointRec(m, r);
    if (hover && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) clicked = true;
    Color base = isSelected ? COL_PRIMARY : COL_PANEL;
    if (hover && !isSelected) base = Fade(COL_PRIMARY, 0.3f);
    DrawRectangleRounded(r, 0.2f, 8, base);
    if (isSelected) DrawRectangleLinesEx(r, 2, COL_ACCENT);
    DrawText(text, r.x + 20, r.y + r.height/2 - 10, 20, COL_TEXT_WHITE);
    return clicked;
}

void LimpiarSeleccionTeatro() {
    obraSel = 0;
    seccionSel = 0;
    cantidad = 1;
    teatroTotalCompra = 0.0f;
}

void InitTeatroScreen(void) {
    LimpiarSeleccionTeatro();
    teatroScreenResult = 0; 
    msgTimer = 0.0f;
}

void UpdateTeatroScreen(void) {
    if (msgTimer > 0) {
        msgTimer -= GetFrameTime();
        return;
    }

    if (DrawToggleBtn(btnHamlet, "Hamlet\nDrama Clásico", obraSel == 1)) obraSel = 1;
    if (DrawToggleBtn(btnCats,   "Cats\nMusical",       obraSel == 2)) obraSel = 2;

    if (DrawToggleBtn(btnVIP,     "VIP ($150)",    seccionSel == 1)) seccionSel = 1;
    if (DrawToggleBtn(btnPlatea,  "Platea ($100)", seccionSel == 2)) seccionSel = 2;
    if (DrawToggleBtn(btnGaleria, "Galería ($50)", seccionSel == 3)) seccionSel = 3;

    if (DrawStyledButton(btnMenos, "-") && cantidad > 1) cantidad--;
    if (DrawStyledButton(btnMas, "+")) cantidad++;

    float precio = 0;
    if (seccionSel == 1) precio = 150;
    if (seccionSel == 2) precio = 100;
    if (seccionSel == 3) precio = 50;
    teatroTotalCompra = precio * cantidad;

    if (obraSel > 0 && seccionSel > 0) {
        const char* nombreObra = (obraSel == 1) ? "Teatro: Hamlet" : "Teatro: Cats";
        const char* nombreSecc = (seccionSel == 1) ? "Secc: VIP" : (seccionSel == 2) ? "Secc: Platea" : "Secc: Galeria";

        if (DrawStyledButton(btnAgregar, "Agregar al Carrito")) {
            AgregarAlCarrito(nombreObra, nombreSecc, precio, cantidad);
            msgTimer = 3.0f;
            LimpiarSeleccionTeatro(); 
        }
        
        if (DrawStyledButton(btnPagar, TextFormat("PAGAR $%.2f >", teatroTotalCompra))) {
            AgregarAlCarrito(nombreObra, nombreSecc, precio, cantidad);
            teatroScreenResult = 2; 
        }
    }

    if (DrawStyledButton(btnVolver, "Volver")) teatroScreenResult = -1;
}

void DrawTeatroScreen(void) {
    ClearBackground(COL_BG);
    DrawText("Cartelera de Teatro", 100, 50, 40, COL_TEXT_WHITE);
    
    DrawText("1. Obra", 100, 110, 20, COL_TEXT_GRAY);
    DrawText("2. Sección", 450, 110, 20, COL_TEXT_GRAY);
    DrawText("3. Cantidad", 800, 110, 20, COL_TEXT_GRAY);
    DrawText(TextFormat("%02d", cantidad), 870, 215, 30, COL_TEXT_WHITE);
    
    if (obraSel > 0) {
        const char* vestimenta = (obraSel == 1) ? "Formal" : "Casual";
        DrawText(TextFormat("Código de vestimenta: %s", vestimenta), 100, 350, 20, COL_ACCENT);
    }
    
    if (msgTimer > 0) DrawNotification("¡Agregado al Carrito!", msgTimer);
}

void UnloadTeatroScreen(void) {}