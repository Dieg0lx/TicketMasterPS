#include "raylib.h"
#include "screen_menu.h"
#include <math.h>

static Rectangle teatroButton = { 600, 250, 400, 60 };
static Rectangle cineButton   = { 600, 350, 400, 60 };
static Rectangle museoButton  = { 600, 450, 400, 60 };

int menuSelection = 0;

static Texture2D logo = {0};      // Asegura cero-inicializado
static float logoFloatTime = 0.0f;

// ----- funciones para pruebas (puras) -----
int HitTestMenu(Vector2 mouse) {
    if (CheckCollisionPointRec(mouse, teatroButton)) return 1;
    if (CheckCollisionPointRec(mouse, cineButton))   return 2;
    if (CheckCollisionPointRec(mouse, museoButton))  return 3;
    return 0;
}

void SetMenuSelectionFromPoint(Vector2 mouse) {
    menuSelection = HitTestMenu(mouse);
}

// ----- ciclo de vida de la pantalla -----
void InitMenuScreen(void) {
    menuSelection = 0;
    // Cargar textura solo si hay ventana (evita crash en tests sin ventana)
    if (IsWindowReady()) {
        logo = LoadTexture("src/resources/icon.png");
    } else {
        logo = (Texture2D){0};
    }
}

void UpdateMenuScreen(void) {
    logoFloatTime += GetFrameTime();

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        menuSelection = HitTestMenu(GetMousePosition());
    }
}

void DrawMenuScreen(void) {
    Color backgroundColor    = (Color){ 25, 25, 35, 255 };
    Color textColor          = (Color){ 230, 230, 230, 255 };
    Color buttonColor        = (Color){ 45, 45, 55, 255 };
    Color buttonBorderColor  = (Color){ 65, 65, 75, 255 };
    Color accentColor        = (Color){ 0, 255, 255, 255 };

    ClearBackground(backgroundColor);

    float logoY = GetScreenHeight()/2.0f - logo.height/2.0f + sinf(logoFloatTime*2.0f)*15.0f;
    if (logo.id > 0) { // Dibuja solo si está cargado correctamente
        DrawTexture(logo, 150, (int)logoY, WHITE);
    }

    DrawText("MENU PRINCIPAL", 600, 150, 40, textColor);

    bool teatroHover = CheckCollisionPointRec(GetMousePosition(), teatroButton);
    DrawRectangleRec(teatroButton, buttonColor);
    DrawRectangleLinesEx(teatroButton, 2, teatroHover ? accentColor : buttonBorderColor);
    DrawText("Venta de Boletos Teatro", teatroButton.x + 55, teatroButton.y + 20, 20, textColor);

    bool cineHover = CheckCollisionPointRec(GetMousePosition(), cineButton);
    DrawRectangleRec(cineButton, buttonColor);
    DrawRectangleLinesEx(cineButton, 2, cineHover ? accentColor : buttonBorderColor);
    DrawText("Venta de Boletos Cine", cineButton.x + 65, cineButton.y + 20, 20, textColor);

    bool museoHover = CheckCollisionPointRec(GetMousePosition(), museoButton);
    DrawRectangleRec(museoButton, buttonColor);
    DrawRectangleLinesEx(museoButton, 2, museoHover ? accentColor : buttonBorderColor);
    DrawText("Venta de Boletos Museo", museoButton.x + 55, museoButton.y + 20, 20, textColor);
}

void UnloadMenuScreen(void) {
    if (logo.id > 0) {      // Evita UnloadTexture sobre un texture inválido
        UnloadTexture(logo);
        logo = (Texture2D){0};
    }
}