#include <stdio.h>
#include <string.h>
#include "database.h"
#include "sqlite3.h"

sqlite3 *db = NULL;

static int column_exists(const char *table, const char *column) {
    sqlite3_stmt *stmt = NULL;
    int exists = 0;
    char sql[256];
    snprintf(sql, sizeof(sql), "PRAGMA table_info(%s);", table);
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char *colName = sqlite3_column_text(stmt, 1);
            if (colName && strcmp((const char *)colName, column) == 0) {
                exists = 1;
                break;
            }
        }
    }
    if (stmt) sqlite3_finalize(stmt);
    return exists;
}

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
        return 1;
    }

    rc = sqlite3_exec(db, sql_create_boletos, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error SQL (Boletos): %s\n", errMsg);
        sqlite3_free(errMsg);
        return 1;
    }

    rc = sqlite3_exec(db, "CREATE UNIQUE INDEX IF NOT EXISTS idx_Usuarios_correo ON Usuarios(correo);", 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error creando índice único en correo: %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    const char *sql_insert_user = "INSERT OR IGNORE INTO Usuarios (username, password, correo) VALUES ('Diego', 'password123', 'diego@gmail.com');";
    sqlite3_exec(db, sql_insert_user, 0, 0, &errMsg);

    return 0;
}

void db_close() {
    if (db) {
        sqlite3_close(db);
        db = NULL;
        fprintf(stdout, "Base de datos cerrada.\n");
    }
}