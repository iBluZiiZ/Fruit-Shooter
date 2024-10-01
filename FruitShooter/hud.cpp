#include "hud.h"
#include "constantes.h"
#include "personaje.h"
#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

void dibujar_hud(ALLEGRO_FONT* font, const Personaje* personaje, int puntaje, int nivel, ALLEGRO_TIMER* tiempo, const char* nombre) {
    float base_x = 20;
    float base_y = LARGO-120;
    float radio_circulo = 30;
    al_draw_filled_circle(base_x + radio_circulo, base_y + radio_circulo, radio_circulo, al_map_rgb(0, 0, 0));
    al_draw_bitmap(personaje->cara, base_x, base_y, 0);
    al_draw_text(font, al_map_rgb(255, 255, 255), base_x + 2 * radio_circulo + 10, base_y, ALLEGRO_ALIGN_LEFT, nombre);
    al_draw_textf(font, al_map_rgb(255, 255, 255), base_x + 2 * radio_circulo + 10, base_y + 20, ALLEGRO_ALIGN_LEFT, "Puntaje: %d", puntaje);
    al_draw_textf(font, al_map_rgb(255, 255, 255), base_x + 2 * radio_circulo + 10, base_y + 40, ALLEGRO_ALIGN_LEFT, "Nivel: %d", nivel);
    al_draw_textf(font, al_map_rgb(255, 255, 255), base_x + 2 * radio_circulo + 10, base_y + 60, ALLEGRO_ALIGN_LEFT, "Tiros: %d", personaje->bala.tiros);
}