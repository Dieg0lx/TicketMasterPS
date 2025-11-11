#ifndef LOGIN_H
#define LOGIN_H


int validarUsuario(const char *user);

int validarContrasena(const char *password);

int verificarCredenciales(const char *user, const char *password);

int emailValido(const char *correo);

int registrarUsuario(const char *correo, const char *user, const char *password, const char *confirm);

#endif // LOGIN_H