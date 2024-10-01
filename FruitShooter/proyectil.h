#pragma once
#ifndef PROYECTIL_H
#define PROYECTIL_H

#include <allegro5/allegro.h>
#include "personaje.h"
#include "frutas.h"
#include "enemigos.h"
#include "municion.h"

void inicializar_proyectil(Enemigo* enemigo, Personaje* personaje, TipoBala tipo, float objetivo_x, float objetivo_y);
void mover_proyectil(Personaje*personaje, Enemigo* enemigo, TipoBala tipo);
void dibujar_proyectil(Personaje* personaje, Enemigo* enemigo);
int chequear_colision(Personaje* personaje, Fruta* frutas, Enemigo* enemigos, Caja* caja);

#endif
