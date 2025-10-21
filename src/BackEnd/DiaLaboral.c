#include <stdio.h>
#include <time.h>
#include "DiaLaboral.h"

// Función para validar día de la semana (evitar fines de semana)
int validarDiaLaboral() {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    int dia_semana = tm_info->tm_wday;
    
    if (dia_semana == 0 || dia_semana == 6) {
        return 0;
    }
    return 1;
}