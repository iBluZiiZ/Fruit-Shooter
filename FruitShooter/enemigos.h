#ifndef ENEMIGOS_H
#define ENEMIGOS_H
#include <allegro5/allegro.h>
#include "bala.h"
#include "personaje.h"

typedef enum {
    PALOMA,
    HAMBURGUESA,
    NUM_TIPOS_ENEMIGOS
} TipoEnemigo;

typedef struct {
    float x, y;
    float vel_x, vel_y;
    int tiros;
    int estado, vida, cantidad, valor;
    ALLEGRO_BITMAP* bitmap;
    ALLEGRO_BITMAP* charco_bitmap;
    float charco_x;
    TipoEnemigo tipo;
    Bala bala;
    bool charco_activo;
} Enemigo;

void inicializar_enemigo(Enemigo* enemigo, TipoEnemigo tipo);
void mover_enemigo(Enemigo* enemigo, float delta_time, Personaje* personaje);
void dibujar_enemigo(const Enemigo* enemigo);
void destruir_enemigo(Enemigo* enemigo);
#endif