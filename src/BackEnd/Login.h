#ifndef LOGIN_H
#define LOGIN_H


int validarUsuario(const char *user);

int validarContrasena(const char *password);

int verificarCredenciales(const char *user, const char *password);

#endif // LOGIN_H