#include <stdio.h>
#include "cine.h"
#include "procesoPago.h"

float getPrecioServicioCine(int servicio) {
    switch(servicio) {
        case 1: return 80.00f;
        case 2: return 100.00f;
        case 3: return 150.00f;
        case 4: return 200.00f;
        case 5: return 250.00f;
        case 6: return 280.00f;
        default: return 0.0f;
    }
}

  
int esServicioDisponibleCine(int cine, int servicio) {
    if (cine == 1 && servicio > 4) {
        return 0; // No disponible
    }
    if (cine == 3 && servicio == 5) {
        return 0; // No disponible
    }
    
    return 1;
}