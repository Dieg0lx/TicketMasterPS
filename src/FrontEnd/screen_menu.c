#include "raylib.h"
#include "screen_menu.h"
#include <math.h> 

static Rectangle teatroButton = { 600, 250, 400, 60 };
static Rectangle cineButton = { 600, 350, 400, 60 };
static Rectangle museoButton = { 600, 450, 400, 60 };

int menuSelection = 0;

static Texture2D logo;
static float logoFloatTime = 0.0f;


void InitMenuScreen(void) {
    menuSelection = 0;
    logo = LoadTexture("src/resources/icon.png");
}

void UpdateMenuScreen(void) {

    logoFloatTime += GetFrameTime();

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), teatroButton)) menuSelection = 1;
        if (CheckCollisionPointRec(GetMousePosition(), cineButton)) menuSelection = 2;
        if (CheckCollisionPointRec(GetMousePosition(), museoButton)) menuSelection = 3;
    }
}

void DrawMenuScreen(void) {
   
    Color backgroundColor = { 25, 25, 35, 255 };    // Azul oscuro casi negro
    Color textColor = { 230, 230, 230, 255 };      // Texto blanco-grisáceo
    Color buttonColor = { 45, 45, 55, 255 };       // Botones de un gris oscuro
    Color buttonBorderColor = { 65, 65, 75, 255 }; // Borde sutil para los botones
    Color accentColor = { 0, 255, 255, 255 };      // Cian brillante para el hover (efecto neón)

    ClearBackground(backgroundColor);

    float logoY = GetScreenHeight() / 2.0f - logo.height / 2.0f + sinf(logoFloatTime * 2.0f) * 15.0f;
    DrawTexture(logo, 150, (int)logoY, WHITE);


    DrawText("MENU PRINCIPAL", 600, 150, 40, textColor);

    // --- Botón de Teatro ---
    bool teatroHover = CheckCollisionPointRec(GetMousePosition(), teatroButton);
    DrawRectangleRec(teatroButton, buttonColor); // Fondo del botón
    DrawRectangleLinesEx(teatroButton, 2, teatroHover ? accentColor : buttonBorderColor); // Borde que cambia con hover
    DrawText("Venta de Boletos Teatro", teatroButton.x + 55, teatroButton.y + 20, 20, textColor);

    // --- Botón de Cine ---
    bool cineHover = CheckCollisionPointRec(GetMousePosition(), cineButton);
    DrawRectangleRec(cineButton, buttonColor);
    DrawRectangleLinesEx(cineButton, 2, cineHover ? accentColor : buttonBorderColor);
    DrawText("Venta de Boletos Cine", cineButton.x + 65, cineButton.y + 20, 20, textColor);

    // --- Botón de Museo ---
    bool museoHover = CheckCollisionPointRec(GetMousePosition(), museoButton);
    DrawRectangleRec(museoButton, buttonColor);
    DrawRectangleLinesEx(museoButton, 2, museoHover ? accentColor : buttonBorderColor);
    DrawText("Venta de Boletos Museo", museoButton.x + 55, museoButton.y + 20, 20, textColor);
}

void UnloadMenuScreen(void) {
    UnloadTexture(logo);
}