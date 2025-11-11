#include "screen_menu.h"
#include <assert.h>
#include <stdio.h>
#include "Login.h"
#include "database.h"
#include <time.h>
#include "screen_register.h"

int main(void) {
    // Pruebas de hit-test (no requieren ventana ni recursos)
    assert(HitTestMenu((Vector2){610, 260}) == 1); // Teatro
    assert(HitTestMenu((Vector2){610, 360}) == 2); // Cine
    assert(HitTestMenu((Vector2){610, 460}) == 3); // Museo
    assert(HitTestMenu((Vector2){100, 100}) == 0); // Fuera de botones
    assert(HitTestMenu((Vector2){200, 200}) == 0);

    SetMenuSelectionFromPoint((Vector2){610, 260});
    assert(menuSelection == 1);

    assert(db_init(":memory:") == 0);
    assert(registrarUsuario("Juan Perez", "juan@test.com", "juan123", "clave1234", "clave1234") == 1);
    assert(verificarCredenciales("juan123", "clave1234") == 1);
    assert(registrarUsuario("Otro", "otro@test.com", "juan123", "clave1234", "clave1234") == 2);
    assert(registrarUsuario("Pepe", "juan@test.com", "pepe123", "clave1234", "clave1234") == 2);
    assert(registrarUsuario("Ana", "ana@test.com", "an", "clave1234", "clave1234") == 0);
    assert(registrarUsuario("Ana", "anatest.com", "ana123", "clave1234", "clave1234") == 0);
    assert(registrarUsuario("Ana", "ana2@test.com", "ana123", "clave", "clave") == 0);
    assert(registrarUsuario("Ana", "ana3@test.com", "ana123", "clave1234", "x") == 0);

    clock_t t0 = clock();
    assert(registrarUsuario("Perf", "perf@test.com", "perf01", "clave1234", "clave1234") != 0);
    double elapsed = (double)(clock() - t0) / CLOCKS_PER_SEC;
    assert(elapsed < 0.5);
    db_close();

    assert(RegisterHitTest((Vector2){640, 575}) == 1);
    assert(RegisterHitTest((Vector2){810, 575}) == -1);

    printf("Todas las pruebas pasaron correctamente.\n");
    return 0;
}