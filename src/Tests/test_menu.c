#include "screen_menu.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
    // Pruebas de hit-test (no requieren ventana ni recursos)
    assert(HitTestMenu((Vector2){610, 260}) == 1); // Teatro
    assert(HitTestMenu((Vector2){610, 360}) == 2); // Cine
    assert(HitTestMenu((Vector2){610, 460}) == 3); // Museo
    assert(HitTestMenu((Vector2){100, 100}) == 0); // Fuera de botones
    assert(HitTestMenu((Vector2){200, 200}) == 0);

    SetMenuSelectionFromPoint((Vector2){610, 260});
    assert(menuSelection == 1);

    printf("Todas las pruebas pasaron correctamente.\n");
    return 0;
}