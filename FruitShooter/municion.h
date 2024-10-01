#pragma once
#ifndef MUNICION_H
#define MUNICION_H
#include <allegro5/allegro5.h>

typedef struct {
    float x, y, vel_x;
    int activo;
    ALLEGRO_BITMAP* bitmap;
} Caja;

void inicializar_caja(Caja* caja);
void mover_Caja(Caja* caja);
void dibujar_Caja(Caja* caja);

#endif