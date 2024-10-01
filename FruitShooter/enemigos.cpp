#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "enemigos.h"
#include "constantes.h"
#include "utilidades.h"
#include "personaje.h"
#include "proyectil.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int frame_actual;
int frame_ancho;
int frame_alto;
int num_frames;
float tiempo_acumulado;
float duracion_frame;

void inicializar_enemigo(Enemigo* enemigo, TipoEnemigo tipo) {
    enemigo->tipo = tipo;
    enemigo->estado = 1;
    const char* filename = NULL;
    bool activo_charco;
    switch (tipo) {
    case HAMBURGUESA:
        enemigo->bitmap = cargar_bitmap("imagenes/hamburguesa.png");
        enemigo->valor = 20;
        filename = "archivos/init_hamburguesa.txt";
        tiempo_acumulado = 0;
        break;
    case PALOMA:
        enemigo->bitmap = cargar_bitmap("imagenes/pajaro.png");
        enemigo->bala.bitmap = cargar_bitmap("imagenes/caca.png");
        enemigo->charco_bitmap = al_load_bitmap("imagenes/charco.png");
        filename = "archivos/init_murcielago.txt";
        enemigo->valor = 15;
        frame_ancho = 64;  // Ancho de cada frame
        frame_alto = 64;  // Alto de cada frame
        num_frames = 4;  // Número de frames en la imagen
        frame_actual = 0;
        tiempo_acumulado = 0;
        duracion_frame = 0.18f;  // Duración de cada frame en segundos
        enemigo->tiros = 2;
        enemigo->bala.x=-1;
        enemigo->bala.y = -1;
        enemigo->bala.activo = 0;
        enemigo->charco_x = 0;
        enemigo->charco_activo = false;
        break;

    default:
        break;
    }

    if (filename) {
        FILE* file = NULL;
        fopen_s(&file, filename, "r");
        if (file) {
            fscanf_s(file, "%f %f %f %f %d", &enemigo->x, &enemigo->y, &enemigo->vel_x, &enemigo->vel_y, &enemigo->vida);
            fclose(file);
        }
        else {
            // Si no se puede abrir el archivo
            enemigo->x = 0;
            enemigo->y = 0;
            enemigo->vel_x = 0;
            enemigo->vel_y = 0;
            enemigo->vida = 1;  // Vida por defecto
        }
    }
    else {
        enemigo->x = 0;
        enemigo->y = 0;
        enemigo->vel_x = 0;
        enemigo->vel_y = 0;
        enemigo->vida = 1;  // Vida por defecto
    }
}

void mover_enemigo(Enemigo* enemigo, float delta_time, Personaje* personaje) {
    if (!enemigo->estado) return;
    enemigo->x += enemigo->vel_x * delta_time;
    enemigo->y += enemigo->vel_y * delta_time;
    switch (enemigo->tipo) {
    case HAMBURGUESA:
        if (enemigo->x + al_get_bitmap_width(enemigo->bitmap) < 0 || enemigo->y > SUELO) {
            enemigo->estado = 0;
            inicializar_enemigo(enemigo, HAMBURGUESA);
        }
        break;
    case PALOMA:
        enemigo->bala.activo++;
        tiempo_acumulado += delta_time;
        if (tiempo_acumulado >= duracion_frame) {
            tiempo_acumulado = 0;
            frame_actual = (frame_actual + 1) % num_frames;
        }
        if (enemigo->x < 0 || enemigo->x > ANCHO) {
            enemigo->estado = 0;
            inicializar_enemigo(enemigo, PALOMA);
        }
        mover_proyectil(personaje,enemigo, Popo);
        break;

    default:
        break;
    }
}

void dibujar_enemigo(const Enemigo* enemigo) {
    if (enemigo->estado && enemigo->bitmap) {
        int frame_x;
        switch (enemigo->tipo) {
        case HAMBURGUESA:
            al_draw_bitmap(enemigo->bitmap, enemigo->x, enemigo->y, 0);
            break;
        case PALOMA:
            frame_x = frame_actual * frame_ancho;
            al_draw_bitmap_region(enemigo->bitmap, frame_x, 0, frame_ancho, frame_alto, enemigo->x, enemigo->y, 0);
            if(enemigo->bala.x >= 0) {
                al_draw_bitmap(enemigo->bala.bitmap, enemigo->bala.x, enemigo->bala.y, 0);
            }
            break;
        default:
            break;
        }
    }
}
void destruir_enemigo(Enemigo* enemigo) {
    if (enemigo->bitmap) {
        al_destroy_bitmap(enemigo->bitmap);
    }
    if (enemigo->bala.bitmap) {
        al_destroy_bitmap(enemigo->bala.bitmap);
    }
}
