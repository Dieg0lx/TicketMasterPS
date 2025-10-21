#ifndef TEATRO_H
#define TEATRO_H

// Obtiene el precio para una sección específica (1: Luneta, 2: Palco, 3: Galeria)
float getPrecioSeccionTeatro(int seccion);

// Obtiene el código de vestimenta para un teatro específico
const char* getVestimentaTeatro(int idTeatro);

#endif // TEATRO_H