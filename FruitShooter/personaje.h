#ifndef PERSONAJE_H
#define PERSONAJE_H
#include <allegro5/allegro.h>
#include "bala.h"

typedef struct {
    float x, y;
    float vel;
    ALLEGRO_BITMAP* sprite;
    int direccion;
    int cont_tiros;
    int frame;
    int tiempo_entre_frames;
    int contador_tiempo;
    bool gordito;
    ALLEGRO_BITMAP* cara;
    Bala bala;
} Personaje;

void inicializar_personaje(Personaje* personaje);
void mover_personaje(Personaje* personaje, int direccion);
void dibujar_personaje(Personaje* personaje);
#endif