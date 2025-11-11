#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "Login.h"
#include "database.h"
#include "sqlite3.h" 

// Esta función se mantiene, es una buena validación del lado del servidor.
int validarUsuario(const char *user) {
    int len = strlen(user);
    if (len < 4 || len > 10) return 0;
    for (int i = 0; i < len; i++) {
        if (!isalnum(user[i])) return 0;
    }
    return 1;
}


int validarContrasena(const char *password) {
    int len = strlen(password);
    // --- CAMBIO: Sincronizado con la regla de screen_register.c ---
    if (len < 8 || len > 12) return 0; 
    
    // Nota: Tu bucle original (i=1; i < len-1) ignoraba el primer y último
    // carácter. Lo corregí para que revise todos (i=0; i < len).
    for (int i = 0; i < len; i++) {
        if (!isalnum(password[i])) return 0;
    }
    return 1;
}

// Esta función no necesita cambios
int verificarCredenciales(const char *user, const char *password) {
    sqlite3_stmt *stmt;
    int result = 0; 

    const char *sql = "SELECT 1 FROM Usuarios WHERE username = ? AND password = ?;";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, user, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW) {
        result = 1;
    } else if (rc == SQLITE_DONE) {
        result = 0;
    } else {
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
        result = 0;
    }

    sqlite3_finalize(stmt);
    
    return result;
}

int emailValido(const char *correo) {
    if (!correo) return 0;
    // --- CAMBIO: Sincronizado con la regla de screen_register.c ---
    // Solo se asegura de que contenga un '@'
    if (strchr(correo, '@') == NULL) return 0;
    
    return 1;
}

// --- CAMBIO PRINCIPAL: Se eliminó el parámetro 'const char *nombre' ---
int registrarUsuario(const char *correo, const char *user, const char *password, const char *confirm) {
    
    // Las validaciones del lado del servidor (backend) se mantienen
    if (!validarUsuario(user)) return 0;
    if (!validarContrasena(password)) return 0;
    if (!emailValido(correo)) return 0;
    if (!password || !confirm || strcmp(password, confirm) != 0) return 0;

    sqlite3_stmt *stmt = NULL;
    
    const char *sql = "INSERT INTO Usuarios (username, password, correo) VALUES (?,?,?);";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al preparar inserción: %s\n", sqlite3_errmsg(db));
        if (stmt) sqlite3_finalize(stmt);
        return 0;
    }
    sqlite3_bind_text(stmt, 1, user, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, correo, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return 1; // Éxito
    } else {
        // --- CAMBIO: Mejor manejo de error para duplicados ---
        int dup = (rc == SQLITE_CONSTRAINT); // Código 19 = Violación de restricción (UNIQUE)
        fprintf(stderr, "Error al insertar usuario: rc=%d, %s\n", rc, sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return dup ? 2 : 0; // 2 = Correo o usuario ya existe
    }
}