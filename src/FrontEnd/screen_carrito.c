#include "../../include/raylib.h"
#include "screen_carrito.h"
#include "ui_styles.h" 
#include <stdio.h>
#include <string.h>

#define MAX_CART_ITEMS 20

static CartItem items[MAX_CART_ITEMS];
static int itemCount = 0;

int carritoScreenResult = 0;

static Rectangle btnPagar = { 800, 600, 280, 60 }; 
static Rectangle btnVolver = { 100, 600, 250, 60 }; 
static Rectangle btnLimpiar = { 100, 80, 150, 30 };

void AgregarAlCarrito(const char* titulo, const char* detalles, float precio, int cantidad) {
    if (itemCount < MAX_CART_ITEMS) {
        strncpy(items[itemCount].titulo, titulo, 63);
        strncpy(items[itemCount].detalles, detalles, 63);
        items[itemCount].precioUnitario = precio;
        items[itemCount].cantidad = cantidad;
        itemCount++;
    }
}

void LimpiarCarrito(void) {
    itemCount = 0;
}

int ObtenerCantidadItems(void) {
    return itemCount;
}

CartItem ObtenerItemCarrito(int index) {
    if (index >= 0 && index < itemCount) return items[index];
    CartItem vacio = {0};
    return vacio;
}

float ObtenerTotalCarrito(void) {
    float total = 0;
    for (int i = 0; i < itemCount; i++) {
        total += items[i].precioUnitario * items[i].cantidad;
    }
    return total;
}

void InitCarritoScreen(void) {
    carritoScreenResult = 0;
}

void UpdateCarritoScreen(void) {
    if (DrawStyledButton(btnVolver, "Seguir Comprando")) {
        carritoScreenResult = -1;
    }

    if (itemCount > 0) {
        float total = ObtenerTotalCarrito();

        if (DrawStyledButton(btnPagar, TextFormat("PAGAR TOTAL: $%.2f", total))) {
            carritoScreenResult = 1;
        }

        Vector2 mouse = GetMousePosition();
        if (CheckCollisionPointRec(mouse, btnLimpiar) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            LimpiarCarrito();
        }
    }
}

void DrawCarritoScreen(void) {
    ClearBackground(COL_BG);
    DrawText("Tu Carrito de Compras", 100, 30, 40, COL_TEXT_WHITE);
    
    if (itemCount == 0) {
        DrawText("Tu carrito está vacío :(", 400, 300, 30, COL_TEXT_GRAY);
        DrawText("Ve al menú para agregar eventos.", 380, 350, 20, COL_TEXT_GRAY);
    } else {
        float startY = 130;
        for (int i = 0; i < itemCount; i++) {

            if (startY + (i * 70) > 550) break;

            Rectangle itemRect = { 100, startY + (i * 70), 800, 60 };
            DrawRectangleRounded(itemRect, 0.2f, 6, COL_PANEL);
        
            DrawText(items[i].titulo, itemRect.x + 20, itemRect.y + 10, 20, COL_TEXT_WHITE);
            DrawText(items[i].detalles, itemRect.x + 20, itemRect.y + 35, 15, COL_TEXT_GRAY);
            
            DrawText(TextFormat("$%.2f x %d", items[i].precioUnitario, items[i].cantidad), 
                     itemRect.x + 500, itemRect.y + 20, 20, COL_TEXT_GRAY);
            DrawText(TextFormat("$%.2f", items[i].precioUnitario * items[i].cantidad), 
                     itemRect.x + 700, itemRect.y + 20, 20, COL_SUCCESS);
        }
        
        DrawText("Vaciar Carrito", btnLimpiar.x, btnLimpiar.y, 20, COL_ERROR);
        if (CheckCollisionPointRec(GetMousePosition(), btnLimpiar)) {
            DrawLine(btnLimpiar.x, btnLimpiar.y + 20, btnLimpiar.x + 130, btnLimpiar.y + 20, COL_ERROR);
        }
    }
}

void UnloadCarritoScreen(void) {}