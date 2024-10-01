#pragma once
#ifndef EVENTOS_H
#define EVENTOS_H

#include <allegro5/allegro.h>
#include "personaje.h"
#include "proyectil.h"

void registrar_eventos(ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_DISPLAY* display, ALLEGRO_TIMER* timer);
void limpiar_recursos(ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_BITMAP* background, ALLEGRO_BITMAP* cursor, ALLEGRO_DISPLAY* display);
void dibujar_cursor(ALLEGRO_BITMAP* cursor, float cursor_x, float cursor_y);
void manejar_evento(ALLEGRO_EVENT ev, Personaje* personaje, Enemigo* enemigo, Bala* bala, int* running, int* pausa, ALLEGRO_TIMER* timer, ALLEGRO_SAMPLE* lanzar);

#endif
