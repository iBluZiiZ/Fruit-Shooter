#pragma once
#ifndef HUD_H
#define HUD_H

#include "personaje.h"
#include <allegro5/allegro_font.h>

void dibujar_hud(ALLEGRO_FONT* font, const Personaje* personaje, int puntaje, int nivel, ALLEGRO_TIMER* tiempo, const char* nombre);

#endif
