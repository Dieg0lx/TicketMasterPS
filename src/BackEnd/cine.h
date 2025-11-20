#ifndef CINE_H
#define CINE_H


float getPrecioServicioCine(int servicio);


int esServicioDisponibleCine(int cine, int servicio);
int registrarCompraCine(const char* username, int servicioId, float precio);

#endif