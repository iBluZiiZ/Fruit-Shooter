#ifndef BALA_H
#define BALA_H

#include <allegro5/allegro.h>

typedef enum {
    Balon,
    Popo,
} TipoBala;

typedef struct {
    float x, y;
    float vel_x, vel_y;
    float tiempo;
    int activo, tiros;
    ALLEGRO_BITMAP* bitmap;
    TipoBala tipo;
} Bala;


#endif