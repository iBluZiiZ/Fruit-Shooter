#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro.h>
#include "proyectil.h"
#include "frutas.h"
#include "enemigos.h"
#include "utilidades.h"
#include "constantes.h"
#include "personaje.h"
#include "municion.h"
#include <math.h>
#include <stdio.h>

void inicializar_proyectil(Enemigo* enemigo, Personaje* personaje, TipoBala tipo, float objetivo_x, float objetivo_y) {
    if (tipo == Balon) {
        personaje->bala.bitmap = cargar_bitmap("imagenes/pelota.png");
        personaje->bala.vel_x = 10;
        personaje->bala.vel_y = 10;
    }
    else if (tipo == Popo) {
        enemigo->bala.bitmap = cargar_bitmap("imagenes/caca.png");
        enemigo->bala.vel_x = 5;
        enemigo->bala.vel_y = 5;
    }
    int FRAME_ANCHO = 100;
    int FRAME_ALTO = 117;
    int NUM_FRAMES = 4;
    int FRAME_TIEMPO = 10; // Estos datos son los mismos que aparecene n personaje.c, los coloque mientras para una solución temporal hasta que realice de buena forma.
    int frame_x = personaje->frame * FRAME_ANCHO;
    int frame_y = 0;
    if (personaje->direccion == -1) {
        frame_y = FRAME_ALTO; // Segunda fila (movimiento izquierda)
    }
    else if (personaje->direccion == 1) {
        frame_y = 2 * FRAME_ALTO; // Tercera fila (movimiento derecha)
    }

    float personaje_centro_x = personaje->x + FRAME_ANCHO / 2;
    float personaje_centro_y = personaje->y + FRAME_ALTO / 2;
    personaje->bala.x = personaje_centro_x - al_get_bitmap_width(personaje->bala.bitmap) / 2;
    personaje->bala.y = personaje_centro_y - al_get_bitmap_height(personaje->bala.bitmap) / 2;

    float distancia_x = objetivo_x - personaje->bala.x;
    float distancia_y = objetivo_y - personaje->bala.y;
    float distancia_total = sqrt(distancia_x * distancia_x + distancia_y * distancia_y);
    personaje->bala.vel_x = distancia_x / distancia_total * 10;
    personaje->bala.vel_y = distancia_y / distancia_total * 10;
    personaje->bala.tiempo = 0;
    personaje->bala.activo = 1;
}

void mover_proyectil(Personaje* personaje, Enemigo* enemigo, TipoBala tipo) {
    ALLEGRO_SAMPLE* lanzar_caca = al_load_sample("sonidos/lanzar_caca.mp3");
    ALLEGRO_SAMPLE* hit = al_load_sample("sonidos/hit.mp3");
    float delta_time = 1.0 / FPS;
    switch (tipo) {
    case Balon:
        personaje->bala.vel_y += 0.005 * personaje->bala.tiempo;
        personaje->bala.x += personaje->bala.vel_x;
        personaje->bala.y += personaje->bala.vel_y;
        personaje->bala.tiempo++;
        break;
    case Popo:
        // Verifica si la bala está inactiva o ha llegado al suelo
        if (enemigo->bala.activo == 0 || enemigo->bala.y == SUELO + 40) {
            enemigo->bala.activo = 1;
            al_play_sample(lanzar_caca, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            float offset_x = ((rand() % (al_get_bitmap_width(enemigo->bitmap) / 2)) - (al_get_bitmap_width(enemigo->bitmap) / 4));
            enemigo->bala.x = enemigo->x + offset_x;
            enemigo->bala.y = enemigo->y;
            enemigo->charco_x = enemigo->bala.x;
        }        
        if (enemigo->bala.activo) {
            if (enemigo->bala.y < SUELO + 40) {
                enemigo->bala.y += delta_time * 200;
                if (enemigo->bala.y >= SUELO + 40) {
                    enemigo->bala.y = SUELO + 40;
                    enemigo->bala.activo = 0;
                        
                }
                
            }
            if (enemigo->bala.x < personaje->x + 100 &&
                enemigo->bala.x + al_get_bitmap_width(enemigo->bala.bitmap) > personaje->x &&
                enemigo->bala.y < personaje->y + 117 &&
                enemigo->bala.y + al_get_bitmap_height(enemigo->bala.bitmap) > personaje->y) {
                al_play_sample(hit, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                personaje->bala.tiros -= 1;
                printf("Has perdido una pelota\n");
                enemigo->bala.y = SUELO + 40;
                enemigo->bala.activo = 0;
            }
            if (enemigo->bala.y > SUELO) {
                enemigo->charco_activo = true;
            }
        }
        break;
    default:
        break;
    }
}

void dibujar_proyectil(Personaje* personaje, Enemigo* enemigo) {
    if (personaje->bala.activo) {
        al_draw_bitmap(personaje->bala.bitmap, personaje->bala.x, personaje->bala.y, 0);
    }
    if (enemigo->charco_activo) {
        //al_draw_bitmap(enemigo->charco_bitmap, enemigo->charco_x, SUELO+50, 0);
    }
}

int chequear_colision(Personaje* personaje, Fruta* frutas, Enemigo* enemigos, Caja* caja) {
    int colision = 0;
    int i = 0;

    // Verificar colisión con frutas
    if (personaje->bala.activo) {
        for (i = 0; i < NUM_TIPOS_FRUTAS; ++i) {
            if (frutas[i].estado && frutas[i].bitmap) {
                if (personaje->bala.x < frutas[i].x + al_get_bitmap_width(frutas[i].bitmap) && personaje->bala.x + al_get_bitmap_width(personaje->bala.bitmap) > frutas[i].x &&
                    personaje->bala.y < frutas[i].y + al_get_bitmap_height(frutas[i].bitmap) && personaje->bala.y + al_get_bitmap_height(personaje->bala.bitmap) > frutas[i].y)
                {
                    colision = i + 1; // IDENTICADOR COLISION - FRUTA
                    break;
                }
            }
        }

        // Verificar colisión con enemigos si no se ha detectado colisión con frutas
        if (colision == 0) {
            for (i = 0; i < NUM_TIPOS_ENEMIGOS; ++i) {
                if (enemigos[i].estado && enemigos[i].bitmap) {
                    if (personaje->bala.x < enemigos[i].x + al_get_bitmap_width(enemigos[i].bitmap) && personaje->bala.x + al_get_bitmap_width(personaje->bala.bitmap) > enemigos[i].x &&
                        personaje->bala.y < enemigos[i].y + al_get_bitmap_height(enemigos[i].bitmap) && personaje->bala.y + al_get_bitmap_height(personaje->bala.bitmap) > enemigos[i].y)
                    {
                        colision = NUM_TIPOS_FRUTAS + i + 1; // IDENTICADOR COLISION - ENEMIGO
                        break;
                    }
                }
            }
        }

        //Verifica colision con caja de municion
        if (colision == 0) {
            if (personaje->bala.x < caja->x + al_get_bitmap_width(caja->bitmap) && personaje->bala.x + al_get_bitmap_width(personaje->bala.bitmap) > caja->x &&
                personaje->bala.y < caja->y + al_get_bitmap_height(caja->bitmap) && personaje->bala.y + al_get_bitmap_height(personaje->bala.bitmap) > caja->y) {
                colision = NUM_TIPOS_ENEMIGOS + NUM_TIPOS_FRUTAS + 1; 
                caja->activo = 0; // IDENTICADOR COLISION - CAJA
            }
        }
    }
    return colision;
}

