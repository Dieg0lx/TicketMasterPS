#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "raylib.h"
#include "BackEnd/database.h"

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
#include "FrontEnd/screen_register.h"

typedef enum GameScreen {
    LOGIN,
    REGISTER,
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

    GameScreen currentScreen = LOGIN;
    
    InitLoginScreen();
    
    SetTargetFPS(60);

    if (db_init("ticketmaster.db") != 0) {
        return -1; 
    }

    while (!WindowShouldClose()) {
        
   
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            PlaySound(fxClick);
        } 
        switch(currentScreen) {
            case LOGIN:
                {
                    UpdateLoginScreen(); 
                    if (loginResult == 1) {
                        UnloadLoginScreen();
                        InitMenuScreen();
                        currentScreen = MAIN_MENU;
                    }
                    else if (goToRegister == 1) {
                        UnloadLoginScreen();
                        InitRegisterScreen();
                        currentScreen = REGISTER;
                    }
                } break;
            case REGISTER:
                {
                    UpdateRegisterScreen();
                    if (registerScreenResult != 0) {
                        UnloadRegisterScreen();
                        InitLoginScreen();
                        currentScreen = LOGIN;
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
                        currentScreen = PAGO;
                    } else if (museoScreenResult == -1) { 
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
        BeginDrawing();
            ClearBackground(RAYWHITE);
            switch(currentScreen) {
                case LOGIN:
                    DrawLoginScreen();
                    break;
                case REGISTER:
                    DrawRegisterScreen();
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
    db_close();
    CloseWindow();
    return 0;
}