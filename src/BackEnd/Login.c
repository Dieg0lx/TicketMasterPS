#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "Login.h"

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
    if (len < 8 || len > 16) return 0;
    for (int i = 1; i < len - 1; i++) {
        if (!isalnum(password[i])) return 0;
    }
    return 1;
}

int verificarCredenciales(const char *user, const char *password) {
    const char *userFinal = "usuario";
    const char *passwordFinal = "claveSegura";

    if (strcmp(user, userFinal) == 0 && strcmp(password, passwordFinal) == 0) {
        return 1; 
    }
    return 0; 
}

