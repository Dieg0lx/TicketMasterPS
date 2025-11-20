#include "../../include/raylib.h"
#include "screen_menu.h"
#include "ui_styles.h"
#include "screen_carrito.h" 
#include <math.h>

static Rectangle cardTeatro = { 150, 200, 300, 400 };
static Rectangle cardCine   = { 490, 200, 300, 400 };
static Rectangle cardMuseo  = { 830, 200, 300, 400 };

static Rectangle btnLogout = { 50, 650, 150, 40 };

int menuSelection = 0;
int logoutRequest = 0; 

static float time = 0.0f;
static Texture2D logo = {0};
static float successTimer = 0.0f;

void ShowMenuSuccessMessage(void) {
    successTimer = 4.0f; 
}

bool DrawEventCard(Rectangle bounds, const char* title, const char* subtitle, Color colorBase) {
    Vector2 mouse = GetMousePosition();
    bool hover = CheckCollisionPointRec(mouse, bounds) && (successTimer <= 0); 
    bool clicked = false;

    if (hover) {
        bounds.y -= 10; 
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) clicked = true;
    }

    DrawRectangleRounded(bounds, 0.1f, 10, COL_PANEL);
    Rectangle header = { bounds.x, bounds.y, bounds.width, 150 };
    DrawRectangleRounded(header, 0.1f, 10, colorBase);
    DrawRectangle(bounds.x, bounds.y + 140, bounds.width, 10, colorBase);

    DrawText(title, bounds.x + 20, bounds.y + 170, 30, COL_TEXT_WHITE);
    DrawText(subtitle, bounds.x + 20, bounds.y + 210, 20, COL_TEXT_GRAY);
    DrawText(hover ? "CLICK PARA ENTRAR" : "VER CARTELERA", bounds.x + 20, bounds.y + 350, 20, hover ? COL_PRIMARY : COL_TEXT_GRAY);
    
    if (hover) DrawRectangleLinesEx(bounds, 2, COL_PRIMARY);
    return clicked;
}

void InitMenuScreen(void) {
    menuSelection = 0;
    logoutRequest = 0; 
    successTimer = 0.0f; 
    if (logo.id == 0) logo = LoadTexture("src/resources/icon.png");
}

void UpdateMenuScreen(void) {
    time += GetFrameTime();
    
    if (successTimer > 0) {
        successTimer -= GetFrameTime();
        return; 
    }

    if (DrawEventCard(cardTeatro, "TEATRO", "Dramas y Comedias", PURPLE)) menuSelection = 1;
    if (DrawEventCard(cardCine,   "CINE",   "Estrenos Mundiales", BLUE))   menuSelection = 2;
    if (DrawEventCard(cardMuseo,  "MUSEO",  "Arte y Cultura",     ORANGE)) menuSelection = 3;

    if (CheckCollisionPointRec(GetMousePosition(), btnLogout)) {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            logoutRequest = 1; 
        }
    }

    Rectangle areaCarrito = { GetScreenWidth() - 100, 10, 90, 60 };
    if (CheckCollisionPointRec(GetMousePosition(), areaCarrito)) {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            menuSelection = 4; 
        }
    }
}

void DrawMenuScreen(void) {
    ClearBackground(COL_BG);

    if (logo.id > 0) {
        float scale = 50.0f / (float)logo.height; 
        DrawTextureEx(logo, (Vector2){20, 20}, 0.0f, scale, WHITE);
        DrawText("TicketMasterPS", 80, 30, 24, COL_TEXT_WHITE);
    }

    int items = ObtenerCantidadItems();
    DrawCartIcon(GetScreenWidth() - 60, 30, items);
    DrawText("Tu Carrito", GetScreenWidth() - 140, 35, 15, COL_TEXT_GRAY);

    DrawText("¿Qué quieres hacer hoy?", 150, 100, 40, COL_TEXT_WHITE);
    DrawRectangle(150, 150, 100, 5, COL_PRIMARY);

    DrawEventCard(cardTeatro, "TEATRO", "Obras Clásicas", PURPLE);
    DrawEventCard(cardCine,   "CINE",   "Sala IMAX 4K", COL_PRIMARY);
    DrawEventCard(cardMuseo,  "MUSEO",  "Historia Viva", ORANGE);

    Vector2 mouse = GetMousePosition();
    bool hoverLogout = CheckCollisionPointRec(mouse, btnLogout);
    Color colorBtn = hoverLogout ? RED : MAROON; 
    
    DrawRectangleRounded(btnLogout, 0.3f, 6, colorBtn);
    DrawText("Cerrar Sesión", btnLogout.x + 20, btnLogout.y + 10, 18, WHITE);

    if (successTimer > 0) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));
        Rectangle r = { GetScreenWidth()/2 - 250, GetScreenHeight()/2 - 75, 500, 150 };
        DrawRectangleRounded(r, 0.2f, 10, COL_PANEL);
        DrawRectangleLinesEx(r, 3, COL_SUCCESS); 
        
        int txtW = MeasureText("¡Compra Realizada con Éxito!", 30);
        DrawText("¡Compra Realizada con Éxito!", r.x + (r.width - txtW)/2, r.y + 40, 30, COL_SUCCESS);
        
        txtW = MeasureText("Se ha enviado el recibo a tu WhatsApp", 20);
        DrawText("Se ha enviado el recibo a tu WhatsApp", r.x + (r.width - txtW)/2, r.y + 90, 20, COL_TEXT_GRAY);
    }
}

void UnloadMenuScreen(void) {
    if (logo.id > 0) UnloadTexture(logo);
    logo.id = 0;
}

int HitTestMenu(Vector2 mouse) { return 0; }
void SetMenuSelectionFromPoint(Vector2 mouse) {}