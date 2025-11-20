#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "register.h" 
#include "database.h"
#include "sqlite3.h" 

int validarUsuario(const char *user) {
    int len = strlen(user);
    if (len < 4 || len > 15) return 0;
    
    for (int i = 0; i < len; i++) {
        if (!isalnum(user[i])) return 0;
    }
    return 1;
}

int validarContrasena(const char *pass) {
    int len = strlen(pass);
    if (len < 6 || len > 12) return 0;

    int tieneMayus = 0;
    int tieneNum = 0;
    int tieneSigno = 0;

    for (int i = 0; i < len; i++) {
        if (isupper(pass[i])) tieneMayus = 1;
        else if (isdigit(pass[i])) tieneNum = 1;
        else if (ispunct(pass[i])) tieneSigno = 1;
    }

    return (tieneMayus && tieneNum && tieneSigno);
}

int emailValido(const char *email) {
    const char *at = strchr(email, '@');
    if (!at) return 0; 
    const char *dot = strchr(at, '.');
    if (!dot) return 0; 
    
    if (strlen(dot) < 2) return 0;

    return 1;
}

int registrarUsuario(const char *correo, const char *user, const char *password, const char *confirm) {
    if (!validarUsuario(user)) return 0;
    if (!validarContrasena(password)) return 0;
    if (!emailValido(correo)) return 0;
    if (strcmp(password, confirm) != 0) return 0;

    sqlite3_stmt *stmt = NULL;
    const char *sql = "INSERT INTO Usuarios (username, password, correo) VALUES (?,?,?);";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error DB: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, user, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, correo, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) {
        return 1; 
    } else if (rc == SQLITE_CONSTRAINT) {
        return -1; 
    } else {
        return 0; 
    }
}