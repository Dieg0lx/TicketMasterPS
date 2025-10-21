#include <stdio.h>
#include <string.h>
#include "procesoPago.h"
#include "DiaLaboral.h"
#include "museo.h"


float getPrecioMuseo(int idMuseo) {
    switch(idMuseo) {
        case 1: return 25.00f; // Louvre
        case 2: return 30.00f; // Metropolitano
        case 3: return 20.00f; // Vaticano
        case 4: return 5.00f;  // Antropologia
        case 5: return 18.00f; // Catalunya
        default: return 0.0f;
    }
}

const char* getRestriccionesMuseo(int idMuseo) {
    switch(idMuseo) {
        case 1: return "No fotografias con flash, no tocar las obras";
        case 2: return "No mochilas grandes, no comida";
        case 3: return "Vestimenta apropiada, no shorts ni camisetas sin manga";
        case 4: return "No fotografias en salas especiales";
        case 5: return "No mascotas, excepto perros guia";
        default: return "N/A";
    }
}