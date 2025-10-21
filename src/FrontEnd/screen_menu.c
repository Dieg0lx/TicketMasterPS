#include "raylib.h"
#include "screen_menu.h"

static Rectangle teatroButton = { 440, 200, 400, 60 };
static Rectangle cineButton = { 440, 300, 400, 60 };
static Rectangle museoButton = { 440, 400, 400, 60 };

int menuSelection = 0;

void InitMenuScreen(void) {
    menuSelection = 0;
}

void UpdateMenuScreen(void) {
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), teatroButton)) menuSelection = 1;
        if (CheckCollisionPointRec(GetMousePosition(), cineButton)) menuSelection = 2;
        if (CheckCollisionPointRec(GetMousePosition(), museoButton)) menuSelection = 3;
    }
}

void DrawMenuScreen(void) {
    ClearBackground(RAYWHITE);
    DrawText("MENU PRINCIPAL", GetScreenWidth() / 2 - MeasureText("MENU PRINCIPAL", 30) / 2, 40, 30, DARKGRAY);

    DrawRectangleRec(teatroButton, LIGHTGRAY);
    if (CheckCollisionPointRec(GetMousePosition(), teatroButton)) DrawRectangleLinesEx(teatroButton, 2, MAROON);
    DrawText("1. Venta de Boletos Teatro", teatroButton.x + 20, teatroButton.y + 15, 20, BLACK);

    DrawRectangleRec(cineButton, LIGHTGRAY);
    if (CheckCollisionPointRec(GetMousePosition(), cineButton)) DrawRectangleLinesEx(cineButton, 2, MAROON);
    DrawText("2. Venta de Boletos Cine", cineButton.x + 20, cineButton.y + 15, 20, BLACK);

    DrawRectangleRec(museoButton, LIGHTGRAY);
    if (CheckCollisionPointRec(GetMousePosition(), museoButton)) DrawRectangleLinesEx(museoButton, 2, MAROON);
    DrawText("3. Venta de Boletos Museo", museoButton.x + 20, museoButton.y + 15, 20, BLACK);
}

void UnloadMenuScreen(void) {
    // Vacío por ahora
}