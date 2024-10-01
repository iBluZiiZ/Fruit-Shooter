#pragma once
#ifndef FRUTAS_H
#define FRUTAS_H

#include <allegro5/allegro.h>

typedef enum {
    MANZANA,
    NARANJA,
    SANDIA,
    PERA,
    LIMON,
    NUM_TIPOS_FRUTAS
} TipoFruta;

typedef struct {
    float x, y;
    float vel_x, vel_y;
    int estado, vida, cantidad, valor;
    ALLEGRO_BITMAP* bitmap;
    TipoFruta tipo;
} Fruta;
// Estado 0 no visible - 1 visible

void inicializar_fruta(Fruta* fruta, TipoFruta tipo);
void mover_fruta(Fruta* fruta);
void dibujar_fruta(const Fruta* fruta);
void destruir_fruta(Fruta* fruta);

#endif