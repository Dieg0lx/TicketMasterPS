#include "raylib.h" 
#include <stdio.h>
#include <string.h>

#include "FrontEnd/screen_login.h"
#include "FrontEnd/screen_register.h"
#include "FrontEnd/screen_menu.h"
#include "FrontEnd/screen_cine.h"
#include "FrontEnd/screen_teatro.h"
#include "FrontEnd/screen_museo.h"
#include "FrontEnd/screen_pago.h"
#include "FrontEnd/screen_carrito.h" 

#include "BackEnd/database.h"
#include "BackEnd/session.h" 

Sound g_fxClick = {0}; 

typedef enum {
    SCREEN_LOGIN, SCREEN_REGISTER, SCREEN_MENU, SCREEN_CINE,
    SCREEN_TEATRO, SCREEN_MUSEO, SCREEN_CARRITO, SCREEN_PAGO
} AppScreen;

int main(void) {
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "TicketMaster");
    SetTargetFPS(60);

    InitAudioDevice();
    g_fxClick = LoadSound("src/resources/click.mp3"); 
    SetSoundVolume(g_fxClick, 0.5f);

    Image icon = LoadImage("src/resources/icon.png");
    if (icon.data != NULL) { SetWindowIcon(icon); UnloadImage(icon); }

    if (db_init("ticketmaster.db") != 0) printf("Error BD\n");

    InitLoginScreen();
    InitRegisterScreen();
    InitMenuScreen();
    InitCineScreen();
    InitTeatroScreen();
    InitMuseoScreen();
    InitCarritoScreen(); 
    
    AppScreen currentScreen = SCREEN_LOGIN;

    while (!WindowShouldClose()) {
        
        switch(currentScreen) {
            case SCREEN_LOGIN:
                UpdateLoginScreen();
                if (loginResult == 1) { loginResult = 0; InitMenuScreen(); currentScreen = SCREEN_MENU; }
                if (goToRegister == 1) { goToRegister = 0; InitRegisterScreen(); currentScreen = SCREEN_REGISTER; }
                break;

            case SCREEN_REGISTER:
                UpdateRegisterScreen();
                if (registerScreenResult == 1 || registerScreenResult == -1) { 
                    registerScreenResult = 0; 
                    InitLoginScreen(); 
                    currentScreen = SCREEN_LOGIN; 
                }
                break;

            case SCREEN_MENU:
                UpdateMenuScreen();
                if (logoutRequest == 1) {
                    logoutRequest = 0; 
                    memset(g_loggedInUser, 0, 20);
                    LimpiarCarrito();
                    InitLoginScreen();
                    currentScreen = SCREEN_LOGIN;
                }

                if (menuSelection != 0) {
                    int sel = menuSelection;
                    menuSelection = 0; 
                    if (sel == 1) { InitTeatroScreen(); currentScreen = SCREEN_TEATRO; }
                    if (sel == 2) { InitCineScreen(); currentScreen = SCREEN_CINE; }
                    if (sel == 3) { InitMuseoScreen(); currentScreen = SCREEN_MUSEO; }
                    if (sel == 4) { currentScreen = SCREEN_CARRITO; } 
                }
                break;

            case SCREEN_CINE:
                UpdateCineScreen();
                if (cineScreenResult == 1) { 
                    cineScreenResult = 0; 
                } 
                else if (cineScreenResult == 2) { // PAGAR AHORA
                    cineScreenResult = 0;
                    float total = ObtenerTotalCarrito();
                    InitPagoScreen("Compra Cine Directa", 1, total);
                    currentScreen = SCREEN_PAGO;
                }
                else if (cineScreenResult == -1) { 
                    cineScreenResult = 0; 
                    currentScreen = SCREEN_MENU; 
                }
                break;

            case SCREEN_TEATRO:
                UpdateTeatroScreen();
                if (teatroScreenResult == 1) { 
                    teatroScreenResult = 0; 
                }
                else if (teatroScreenResult == 2) { // PAGAR AHORA
                    teatroScreenResult = 0;
                    float total = ObtenerTotalCarrito();
                    InitPagoScreen("Compra Teatro Directa", 2, total);
                    currentScreen = SCREEN_PAGO;
                }
                else if (teatroScreenResult == -1) { 
                    teatroScreenResult = 0; 
                    currentScreen = SCREEN_MENU; 
                }
                break;

            case SCREEN_MUSEO:
                UpdateMuseoScreen();
                if (museoScreenResult == 1) { 
                    museoScreenResult = 0; 
                }
                else if (museoScreenResult == 2) { // PAGAR AHORA
                    museoScreenResult = 0;
                    float total = ObtenerTotalCarrito();
                    InitPagoScreen("Compra Museo Directa", 3, total);
                    currentScreen = SCREEN_PAGO;
                }
                else if (museoScreenResult == -1) { 
                    museoScreenResult = 0; 
                    currentScreen = SCREEN_MENU; 
                }
                break;

            case SCREEN_CARRITO: 
                UpdateCarritoScreen();
                if (carritoScreenResult == 1) { 
                    carritoScreenResult = 0; 
                    float total = ObtenerTotalCarrito();
                    InitPagoScreen("Carrito Completo", 999, total); 
                    currentScreen = SCREEN_PAGO;
                }
                if (carritoScreenResult == -1) { 
                    carritoScreenResult = 0; 
                    currentScreen = SCREEN_MENU; 
                }
                break;

            case SCREEN_PAGO:
                UpdatePagoScreen();
                if (pagoScreenResult == 1) {
                    int totalItems = ObtenerCantidadItems();
                    for(int i=0; i < totalItems; i++) {
                        CartItem item = ObtenerItemCarrito(i);
                        char desc[128];
                        sprintf(desc, "%s (%s) x%d", item.titulo, item.detalles, item.cantidad);
                        registrarBoletoGenerico(g_loggedInUser, desc, item.precioUnitario * item.cantidad);
                    }
                    LimpiarCarrito();
                    InitMenuScreen();
                    ShowMenuSuccessMessage();
                    pagoScreenResult = 0; 
                    currentScreen = SCREEN_MENU; 
                }
                if (pagoScreenResult == -1) {
                    pagoScreenResult = 0; 
                    currentScreen = SCREEN_CARRITO; 
                }
                break;
        }

        BeginDrawing();
            ClearBackground(RAYWHITE); 
            switch(currentScreen) {
                case SCREEN_LOGIN: DrawLoginScreen(); break;
                case SCREEN_REGISTER: DrawRegisterScreen(); break;
                case SCREEN_MENU: DrawMenuScreen(); break;
                case SCREEN_CINE: DrawCineScreen(); break;
                case SCREEN_TEATRO: DrawTeatroScreen(); break;
                case SCREEN_MUSEO: DrawMuseoScreen(); break;
                case SCREEN_CARRITO: DrawCarritoScreen(); break; 
                case SCREEN_PAGO: DrawPagoScreen(); break;
            }
        EndDrawing();
    }

    UnloadLoginScreen(); UnloadMenuScreen(); UnloadCineScreen();
    UnloadTeatroScreen(); UnloadMuseoScreen(); UnloadCarritoScreen(); UnloadPagoScreen();
    UnloadSound(g_fxClick); 
    CloseAudioDevice();
    db_close();
    CloseWindow();
    return 0;
}