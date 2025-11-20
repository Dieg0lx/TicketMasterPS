#ifndef NOTIFICACIONES_H
#define NOTIFICACIONES_H

#include "raylib.h"
#include <stdio.h>
#include <string.h>

static void UrlEncode(char* dest, const char* src) {
    char* d = dest;
    while (*src) {
        if (*src == ' ') {
            *d++ = '%'; *d++ = '2'; *d++ = '0';
        } else if (*src == '\n') {
            *d++ = '%'; *d++ = '0'; *d++ = 'A';
        } else if (*src == ':') {
            *d++ = '%'; *d++ = '3'; *d++ = 'A';
        } else if (*src == '/') {
            *d++ = '%'; *d++ = '2'; *d++ = 'F';
        } else {
            *d++ = *src;
        }
        src++;
    }
    *d = '\0';
}

static void EnviarResumenWhatsApp(const char* evento, int boletos, float total) {
    char mensajeRaw[1024];
    char mensajeEncoded[2048];
    char finalUrl[4096];

    sprintf(mensajeRaw, 
        "*--- TICKETMASTER PS ---*\n"
        "Confirmación de Compra\n\n"
        "*Evento:* %s\n"
        "*Boletos:* %d\n"
        "*Total:* $%.2f\n"
        "-------------------------", 
        evento, boletos, total);

    UrlEncode(mensajeEncoded, mensajeRaw);
    sprintf(finalUrl, "https://wa.me/?text=%s", mensajeEncoded);
    
    OpenURL(finalUrl);
}

static void EnviarResumenCorreo(const char* evento, int boletos, float total) {
    char asuntoRaw[100];
    char cuerpoRaw[1024];
    
    char asuntoEncoded[256];
    char cuerpoEncoded[2048];
    char finalUrl[4096];

    sprintf(asuntoRaw, "Recibo de Compra - TicketMasterPS");
    
    sprintf(cuerpoRaw, 
        "TICKETMASTER PS\n"
        "-------------------------\n"
        "Gracias por tu compra.\n\n"
        "Evento: %s\n"
        "Cantidad de Boletos: %d\n"
        "Total Pagado: $%.2f\n\n"
        "Presenta este correo en la taquilla.\n"
        "-------------------------", 
        evento, boletos, total);

    UrlEncode(asuntoEncoded, asuntoRaw);
    UrlEncode(cuerpoEncoded, cuerpoRaw);

    sprintf(finalUrl, "mailto:?subject=%s&body=%s", asuntoEncoded, cuerpoEncoded);

    OpenURL(finalUrl);
}

#endif