#include <stdio.h>
#include "teatro.h"
#include <string.h>
#include "procesoPago.h"
#include "DiaLaboral.h"

// Esta función solo calcula y devuelve el precio. No usa printf ni scanf.
float getPrecioSeccionTeatro(int seccion) {
    // Usamos los mismos precios que tenías en tu versión de consola.
    switch (seccion) {
        case 1: return 150.0f; // Precio Luneta
        case 2: return 200.0f; // Precio Palco
        case 3: return 100.0f; // Precio Galeria
        default: return 0.0f;  // Si no se ha seleccionado nada o la sección no es válida
    }
}

// Esta función solo devuelve el texto de la vestimenta.
const char* getVestimentaTeatro(int idTeatro) {
    switch(idTeatro) {
        case 1: return "Formal/Etiqueta";
        case 2: return "Formal/Smoking";
        case 3: return "Casual elegante";
        default: return "N/A";
    }
}