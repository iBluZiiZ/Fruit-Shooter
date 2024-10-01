#include "personaje.h"
#include "constantes.h"
#include "utilidades.h" // Para cargar_bitmap
#include <stdio.h>

int FRAME_ANCHO = 100;
int FRAME_ALTO = 117;
int NUM_FRAMES = 4;
int FRAME_TIEMPO = 10; // Tiempo entre frames en ticks

void inicializar_personaje(Personaje* personaje) {
    personaje->sprite = cargar_bitmap("imagenes/sprite.png");
    personaje->cara = cargar_bitmap("imagenes/cara.png");
    personaje->x = LARGO / 2;
    personaje->y = SUELO - 60;
    personaje->vel = 7.0;   // velocidad
    personaje->bala.tiros = 12; // Tiros Iniciales aninado
    personaje->direccion = 0;
    personaje->frame = 0;
    personaje->tiempo_entre_frames = FRAME_TIEMPO;
    personaje->contador_tiempo = 0;
    personaje->bala.bitmap = cargar_bitmap("imagenes/pelota.png");
    personaje->cont_tiros = 0;  // contador tiros
    personaje->gordito = false;
}

void mover_personaje(Personaje* personaje, int direccion) {
    personaje->direccion = direccion;
    if (direccion == -1) {
        personaje->x -= personaje->vel;
    }
    else if (direccion == 1) {
        personaje->x += personaje->vel;
    }
    if (personaje->x < 0) {
        personaje->x = 0;
    }
    if (personaje->x > ANCHO - FRAME_ANCHO) {
        personaje->x = ANCHO - FRAME_ANCHO;
    }
}

void dibujar_personaje(Personaje* personaje) {
    personaje->contador_tiempo++;
    if (personaje->contador_tiempo >= personaje->tiempo_entre_frames) {
        personaje->frame = (personaje->frame + 1) % NUM_FRAMES;
        personaje->contador_tiempo = 0;
    }
    int frame_x = personaje->frame * FRAME_ANCHO;
    int frame_y = 0;
    if (personaje->direccion == -1) {
        frame_y = FRAME_ALTO; // Segunda fila (movimiento izquierda)
    }
    else if (personaje->direccion == 1) {
        frame_y = 2 * FRAME_ALTO; // Tercera fila (movimiento derecha)
    }

    if (personaje->gordito) {
        al_draw_tinted_bitmap_region(personaje->sprite, al_map_rgba_f(255, 99, 71, 0.4),frame_x, frame_y, FRAME_ANCHO, FRAME_ALTO, personaje->x, personaje->y, 0);
    }
    else
    {
        al_draw_bitmap_region(personaje->sprite, frame_x, frame_y, FRAME_ANCHO, FRAME_ALTO, personaje->x, personaje->y, 0);
    }
}