#pragma once
#ifndef NIVEL_H
#define NIVEL_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include "frutas.h"
#include "enemigos.h"
#include "proyectil.h"

typedef struct {
    int nivel;
    int cantidad_frutas[NUM_TIPOS_FRUTAS];
    int cantidad_enemigos[NUM_TIPOS_ENEMIGOS];
} Nivel;

void cargar_nivel(Nivel* nivel, const char* ruta, int num_nivel);
void inicializar_nivel(Nivel* nivel, Fruta* frutas, Enemigo* enemigos, Personaje* personaje);
int nivel_completado(Fruta* frutas, Enemigo* enemigos);
void manejar_nivel(Nivel* nivel, Personaje* personaje, Fruta* frutas, Enemigo* enemigos, ALLEGRO_DISPLAY* display, ALLEGRO_FONT* font, int nivel_actual, int* running, Bala* bala, int tiempo_transcurrido, int puntaje);

void mostrar_pantalla_transicion(ALLEGRO_DISPLAY* display, ALLEGRO_FONT* font, int niv, int tiempo_transcurrido, Personaje* personaje, int puntaje);

#endif