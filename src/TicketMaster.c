#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "raylib.h"

#include "BackEnd/teatro.h"
#include "BackEnd/cine.h"
#include "BackEnd/museo.h"
#include "BackEnd/Login.h"
#include "BackEnd/DiaLaboral.h"


#include "FrontEnd/screen_login.h"
#include "FrontEnd/screen_teatro.h"
#include "FrontEnd/screen_menu.h"
#include "FrontEnd/screen_pago.h"
#include "FrontEnd/screen_cine.h"
#include "FrontEnd/screen_museo.h"

typedef enum GameScreen {
    LOGIN,
    MAIN_MENU,
    TEATRO,
    CINE,
    MUSEO,
    PAGO
} GameScreen;

int main(void) {
    // --- Inicialización de la Ventana ---
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "TicketMaster");

    InitAudioDevice();
    Sound fxClick = LoadSound("src/resources/click.mp3");

    Image icon = LoadImage ("src/resources/icon.png");
    SetWindowIcon(icon);
    UnloadImage(icon);

    // 2. Establecemos el estado inicial
    GameScreen currentScreen = LOGIN;
    
    // Inicializamos los recursos de la primera pantalla
    InitLoginScreen();
    
    SetTargetFPS(60);

    // --- Bucle Principal de la Aplicación ---
    while (!WindowShouldClose()) {
        
   
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            PlaySound(fxClick);
        } 
        switch(currentScreen) {
            case LOGIN:
                {
                    UpdateLoginScreen(); // Actualizamos la pantalla de login
                    
                    // Verificamos si el login fue exitoso
                    if (loginResult == 1) {
                        UnloadLoginScreen();
                        InitMenuScreen();
                        currentScreen = MAIN_MENU;
                    }
                } break;
                case MAIN_MENU:
                UpdateMenuScreen();
                if (menuSelection != 0) {
                    if (menuSelection == 1 && validarDiaLaboral()) 
                    {
                        UnloadMenuScreen();
                        InitTeatroScreen();
                        currentScreen = TEATRO;
                    }
                    else if (menuSelection == 2 && validarDiaLaboral())
                     {
                        InitCineScreen();
                        currentScreen = CINE;
                    }
                    else if (menuSelection == 3 && validarDiaLaboral()) 
                    {
                        InitMuseoScreen();
                        currentScreen = MUSEO;
                    }
                    menuSelection = 0;
                }
                break;
            case TEATRO:
                {
                    UpdateTeatroScreen(); 
                    if (teatroScreenResult == 1) {
                        UnloadTeatroScreen(); 
                        InitPagoScreen(teatroTotalCompra); 
                        currentScreen = PAGO;
                    } 
                    else if (teatroScreenResult == -1) {
                        UnloadTeatroScreen();
                        InitMenuScreen();
                        currentScreen = MAIN_MENU;
                    }
                } break;
                   case CINE: 
                {
                    UpdateCineScreen();
                    if (cineScreenResult == 1) { 
                        UnloadCineScreen();
                        InitPagoScreen(cineTotalCompra);
                        currentScreen = PAGO;
                    } else if (cineScreenResult == -1) {
                        UnloadCineScreen();
                        InitMenuScreen();
                        currentScreen = MAIN_MENU;
                    }
                } break;

                case MUSEO:
                {
                    UpdateMuseoScreen();
                    if (museoScreenResult == 1) { 
                        UnloadMuseoScreen();
                        InitPagoScreen(museoTotalCompra);
                        currentScreen = PAGO;
                    } else if (museoScreenResult == -1) { // Volver
                        UnloadMuseoScreen();
                        InitMenuScreen();
                        currentScreen = MAIN_MENU;
                    }
                } break;
                case PAGO:
                {
                    UpdatePagoScreen();
                    if (pagoScreenResult == 1) { 
                        UnloadPagoScreen();
                        InitMenuScreen();
                        currentScreen = MAIN_MENU;
                    } else if (pagoScreenResult == -1) { 
                        UnloadPagoScreen();
                        InitTeatroScreen();
                        currentScreen = TEATRO;
                    }
                } break;
        
            default: break;
        }

        // --- 4. Lógica de Dibujado (Vista) ---
        BeginDrawing();
            ClearBackground(RAYWHITE);
            switch(currentScreen) {
                case LOGIN:
                    DrawLoginScreen();
                    break;
                case MAIN_MENU:
                    DrawMenuScreen();
                    break;
                case TEATRO:
                    DrawTeatroScreen();
                    break;
                    case CINE:
                    {
                    DrawCineScreen();
                    } break;
                    case MUSEO:
                    {
                    DrawMuseoScreen();
                    } break;
                    case PAGO:
                    {
                    DrawPagoScreen();
                    } break;
                default: break;
            }
        EndDrawing();
    }

    if (currentScreen == LOGIN) UnloadLoginScreen();

    UnloadSound(fxClick);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}