#include <stdio.h>
#include "teatro.h"
#include <string.h>
#include "procesoPago.h"
#include "DiaLaboral.h"

float getPrecioSeccionTeatro(int seccion) {
    switch (seccion) {
        case 1: return 150.0f;
        case 2: return 200.0f;
        case 3: return 100.0f;
        default: return 0.0f;
    }
}

const char* getVestimentaTeatro(int idTeatro) {
    switch(idTeatro) {
        case 1: return "Formal/Etiqueta";
        case 2: return "Formal/Smoking";
        case 3: return "Casual elegante";
        default: return "N/A";
    }
}