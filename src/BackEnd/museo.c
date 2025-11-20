#include <stdio.h>
#include "museo.h"
#include "database.h"
#include "sqlite3.h"

float getPrecioMuseo(int museoId) {
    switch(museoId) {
        case 1: return 200.0f; 
        case 2: return 180.0f; 
        case 3: return 150.0f; 
        case 4: return 80.0f;  
        case 5: return 120.0f; 
        default: return 0.0f;
    }
}

const char* getRestriccionesMuseo(int museoId) {
    switch(museoId) {
        case 1: return "No flash, no tripie";
        case 2: return "No mochilas grandes, no comida";
        case 3: return "Vestimenta apropiada, no shorts ni camisetas sin manga";
        case 4: return "No fotografias en salas especiales";
        case 5: return "No mascotas, excepto perros guia";
        default: return "N/A";
    }
}