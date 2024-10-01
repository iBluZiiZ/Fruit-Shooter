#include "utilidades.h"
#include "constantes.h"
#include <stdio.h>

ALLEGRO_BITMAP* cargar_bitmap(const char* ruta) {
    ALLEGRO_BITMAP* bitmap = al_load_bitmap(ruta);
    if (!bitmap) {
        fprintf(stderr, "Error al cargar bitmap: %s\n", ruta);
    }
    return bitmap;
}

ALLEGRO_DISPLAY* crear_display() {
    ALLEGRO_DISPLAY* display = al_create_display(ANCHO, LARGO);
    if (!display) {
        fprintf(stderr, "Error al crear display!\n");
        exit(-1);
    }
    return display;
}

ALLEGRO_FONT* cargar_fuente(const char* ruta, int tamano) {
    ALLEGRO_FONT* font = al_load_ttf_font(ruta, tamano, 0);
    if (!font) {
        fprintf(stderr, "Error al cargar fuente: %s\n", ruta);
        exit(-1);
    }
    return font;
}