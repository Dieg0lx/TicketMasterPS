#include "../../include/raylib.h"
#include "screen_cine.h"
#include "../../include/raylib.h"
#include "screen_cine.h"
#include "ui_styles.h"
#include "screen_carrito.h" 
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>   

#define ROWS 5
#define COLS 8
#define SEAT_SIZE 50
#define SEAT_PAD 15

static int seats[ROWS][COLS];
static float totalCine = 0.0f;
static int boletosSeleccionados = 0;
static char asientosTexto[64] = {0}; 

static Rectangle btnAgregar = { 850, 510, 250, 50 };
static Rectangle btnPagar   = { 850, 580, 250, 50 };
static Rectangle btnVolver  = { 50, 580, 150, 50 };

int cineScreenResult = 0; 
static float msgTimer = 0.0f;

void LimpiarSeleccionCine() {
    for(int i=0; i<ROWS; i++) {
        for(int j=0; j<COLS; j++) {
            if (seats[i][j] == 2) seats[i][j] = 0; 
        }
    }
    boletosSeleccionados = 0;
    totalCine = 0.0f;
}

void InitCineScreen(void) {
    cineScreenResult = 0;
    totalCine = 0.0f;
    boletosSeleccionados = 0;
    msgTimer = 0.0f;
    srand(time(NULL));

    for(int i=0; i<ROWS; i++) {
        for(int j=0; j<COLS; j++) {
            seats[i][j] = 0; 
            if ((rand() % 100) < 30) seats[i][j] = 1; 
        }
    }
}

void UpdateCineScreen(void) {
    if (msgTimer > 0) {
        msgTimer -= GetFrameTime();
        return; 
    }

    Vector2 mouse = GetMousePosition();

    if (CheckCollisionPointRec(mouse, btnAgregar) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        if (boletosSeleccionados > 0) {
            sprintf(asientosTexto, "Sala 1 - %d Asientos", boletosSeleccionados);
            AgregarAlCarrito("CINE: Avengers Endgame", asientosTexto, 85.0f, boletosSeleccionados);
            
            msgTimer = 3.0f; 
            LimpiarSeleccionCine(); 
        } 
    }

    if (CheckCollisionPointRec(mouse, btnPagar) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        if (boletosSeleccionados > 0) {
            sprintf(asientosTexto, "Sala 1 - %d Asientos", boletosSeleccionados);
            AgregarAlCarrito("CINE: Avengers Endgame", asientosTexto, 85.0f, boletosSeleccionados);
            cineScreenResult = 2; 
        }
    }

    if (CheckCollisionPointRec(mouse, btnVolver) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        cineScreenResult = -1;
    }

    int startX = 100;
    int startY = 150;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        for(int i=0; i<ROWS; i++) {
            for(int j=0; j<COLS; j++) {
                Rectangle r = { startX + j*(SEAT_SIZE+SEAT_PAD), startY + i*(SEAT_SIZE+SEAT_PAD), SEAT_SIZE, SEAT_SIZE };
                if (CheckCollisionPointRec(mouse, r)) {
                    if (seats[i][j] == 0) {
                        seats[i][j] = 2; 
                        boletosSeleccionados++;
                        totalCine += 85.0f; 
                    } else if (seats[i][j] == 2) {
                        seats[i][j] = 0; 
                        boletosSeleccionados--;
                        totalCine -= 85.0f;
                    }
                }
            }
        }
    }
}

void DrawCineScreen(void) {
    ClearBackground(COL_BG);

    DrawText("Selecciona tus Asientos", 100, 30, 40, COL_TEXT_WHITE);
    DrawText("Sala 1: Avengers - $85.00 c/u", 100, 80, 20, COL_TEXT_GRAY);

    DrawRectangleRounded((Rectangle){150, 120, 450, 10}, 0.5f, 5, COL_TEXT_GRAY);
    DrawText("PANTALLA", 320, 105, 10, COL_TEXT_GRAY);

    int startX = 100;
    int startY = 150;

    for(int i=0; i<ROWS; i++) {
        for(int j=0; j<COLS; j++) {
            Rectangle r = { startX + j*(SEAT_SIZE+SEAT_PAD), startY + i*(SEAT_SIZE+SEAT_PAD), SEAT_SIZE, SEAT_SIZE };
            Color color = COL_SEAT_FREE;
            if (seats[i][j] == 1) color = COL_SEAT_BUSY;
            if (seats[i][j] == 2) color = COL_SEAT_SEL;
            DrawRectangleRounded(r, 0.3f, 6, color);
            if (j==0) DrawText(TextFormat("%c", 'A'+i), r.x - 25, r.y + 15, 20, COL_TEXT_GRAY);
        }
    }

    Rectangle panel = { 850, 150, 250, 300 };
    DrawRectangleRounded(panel, 0.1f, 10, COL_PANEL);
    DrawText("RESUMEN", panel.x + 20, panel.y + 20, 20, COL_PRIMARY);
    
    DrawText(TextFormat("Boletos: %d", boletosSeleccionados), panel.x + 20, panel.y + 60, 20, COL_TEXT_WHITE);
    DrawText("Subtotal:", panel.x + 20, panel.y + 100, 20, COL_TEXT_GRAY);
    DrawText(TextFormat("$%.2f", totalCine), panel.x + 20, panel.y + 130, 30, COL_SUCCESS);

    int leyY = 470;
    DrawRectangle(850, leyY, 20, 20, COL_SEAT_FREE); DrawText("Libre", 880, leyY, 10, COL_TEXT_GRAY);
    DrawRectangle(920, leyY, 20, 20, COL_SEAT_SEL);  DrawText("Tuyo", 950, leyY, 10, COL_TEXT_GRAY);
    DrawRectangle(990, leyY, 20, 20, COL_SEAT_BUSY); DrawText("Ocupado", 1020, leyY, 10, COL_TEXT_GRAY);


    if (boletosSeleccionados > 0 && msgTimer <= 0) {
        DrawStyledButton(btnAgregar, "Agregar al Carrito"); 
        DrawStyledButton(btnPagar, "Pagar Ahora >");      
    }

    DrawStyledButton(btnVolver, "< Volver");

    if (msgTimer > 0) DrawNotification("¡Agregado al Carrito!", msgTimer);
}

void UnloadCineScreen(void) {}