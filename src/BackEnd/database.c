#include <stdio.h>
#include <string.h>
#include "database.h"
#include "sqlite3.h"

sqlite3 *db = NULL;

int db_init(const char* filename) {
    int rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    fprintf(stdout, "Base de datos abierta exitosamente: %s\n", filename);



    const char *sql_create_usuarios = 
        "CREATE TABLE IF NOT EXISTS Usuarios ("
        "  ID INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  username TEXT NOT NULL UNIQUE,"
        "  password TEXT NOT NULL,"
        "  correo TEXT"
        ");";

    const char *sql_create_boletos =
        "CREATE TABLE IF NOT EXISTS Boletos ("
        "  ID INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  username TEXT NOT NULL,"
        "  tipo_servicio TEXT NOT NULL,"
        "  precio REAL NOT NULL,"
        "  fecha DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");";

    char *errMsg = 0;
    
    rc = sqlite3_exec(db, sql_create_usuarios, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error SQL (Usuarios): %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    rc = sqlite3_exec(db, sql_create_boletos, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error SQL (Boletos): %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    sqlite3_exec(db, "INSERT OR IGNORE INTO Usuarios (username, password, correo) VALUES ('admin', 'admin123', 'admin@test.com');", 0, 0, 0);

    return 0;
}

void db_close() {
    if (db) {
        sqlite3_close(db);
        printf("Base de datos cerrada.\n");
    }
}

int registrarBoletoGenerico(const char* username, const char* detalle, float precio) {
    if (!db) return 0;

    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO Boletos (username, tipo_servicio, precio) VALUES (?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparando insert: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, detalle, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, (double)precio);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) return 1; 
    return 0; 
}