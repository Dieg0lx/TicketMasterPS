#ifndef SCREEN_CARRITO_H
#define SCREEN_CARRITO_H

typedef struct {
    char titulo[64];
    char detalles[64];
    float precioUnitario;
    int cantidad;
} CartItem;

void InitCarritoScreen(void);
void UpdateCarritoScreen(void);
void DrawCarritoScreen(void);
void UnloadCarritoScreen(void);

void AgregarAlCarrito(const char* titulo, const char* detalles, float precio, int cantidad);
void LimpiarCarrito(void);
int ObtenerCantidadItems(void);
float ObtenerTotalCarrito(void);
CartItem ObtenerItemCarrito(int index); 

extern int carritoScreenResult;

#endif