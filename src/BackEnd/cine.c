#include <stdio.h>
#include "cine.h"
#include "procesoPago.h"
#include "database.h" 
#include "sqlite3.h"

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

int registrarCompraCine(const char* username, int servicioId, float precio) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO Boletos (username, tipo_servicio, precio) VALUES (?, ?, ?);";

    // 1. Preparar la consulta
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al preparar INSERT: %s\n", sqlite3_errmsg(db));
        return 0; // Falla
    }

    // 2. Asignar valores
    char servicioStr[20];
    sprintf(servicioStr, "Cine_Servicio_%d", servicioId);

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, servicioStr, -1, SQLITE_TRANSIENT); // Transient porque servicioStr es local
    sqlite3_bind_double(stmt, 3, (double)precio);

    // 3. Ejecutar
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error al ejecutar INSERT: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 0; // Falla
    }

    // 4. Finalizar
    sqlite3_finalize(stmt);
    printf("Compra de cine registrada para %s.\n", username);
    return 1; // Éxito
}