#pragma once
#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

ALLEGRO_BITMAP* cargar_bitmap(const char* ruta);
ALLEGRO_DISPLAY* crear_display();
ALLEGRO_FONT* cargar_fuente(const char* ruta, int tamano);

#endif