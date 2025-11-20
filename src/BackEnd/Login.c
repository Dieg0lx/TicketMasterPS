#include <stdio.h>
#include <string.h>
#include "Login.h"
#include "database.h"
#include "sqlite3.h" 

int verificarCredenciales(const char *user, const char *password) {
    if (!db) return 0;

    sqlite3_stmt *stmt;
    const char *sql = "SELECT 1 FROM Usuarios WHERE username = ? AND password = ?;";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparando query Login: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, user, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    int found = (rc == SQLITE_ROW);
    
    sqlite3_finalize(stmt);
    return found;
}