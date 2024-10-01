#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "eventos.h"
#include "constantes.h"
#include "personaje.h"
#include "proyectil.h"
#include "enemigos.h"
void registrar_eventos(ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_DISPLAY* display, ALLEGRO_TIMER* timer) {
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
}

void limpiar_recursos(ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_BITMAP* background, ALLEGRO_BITMAP* cursor, ALLEGRO_DISPLAY* display) {
    al_destroy_event_queue(event_queue);
    al_destroy_bitmap(background);
    al_destroy_bitmap(cursor);
    al_destroy_display(display);
}

void dibujar_cursor(ALLEGRO_BITMAP* cursor, float cursor_x, float cursor_y) {
    al_draw_bitmap(cursor, cursor_x - 12, cursor_y - 12, 0);
}

void manejar_evento(ALLEGRO_EVENT ev, Personaje* personaje, Enemigo* enemigo, Bala* bala, int* running, int* pausa, ALLEGRO_TIMER* timer, ALLEGRO_SAMPLE* lanzar) {

    switch (ev.type) {
    case ALLEGRO_EVENT_KEY_CHAR:
        switch (ev.keyboard.keycode) {
        case ALLEGRO_KEY_A: //Izquierda
            mover_personaje(personaje, -1);
            break;
        case ALLEGRO_KEY_D: //Derecha
            mover_personaje(personaje, 1);
            break;
        case ALLEGRO_KEY_ESCAPE:
            *running = 0;
            break;
        case ALLEGRO_KEY_P:
            *pausa =! *pausa;
            if (*pausa) {
                al_stop_timer(timer);  // Detener el temporizador
            }
            else {
                al_start_timer(timer);  // Iniciar el temporizador
            }
        }
        break;
    case ALLEGRO_EVENT_KEY_UP:
        if (ev.keyboard.keycode == ALLEGRO_KEY_A || ev.keyboard.keycode == ALLEGRO_KEY_D) {
            mover_personaje(personaje, 0);  // Detener movimiento horizontal
        }
        break;
    case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
        if (ev.mouse.button == 1 && !personaje->bala.activo && personaje->bala.tiros > 0) {
            inicializar_proyectil(enemigo, personaje, Balon, ev.mouse.x, ev.mouse.y);
            personaje->cont_tiros++; // Contador de proyectil por nivel
            personaje->bala.tiros--; // Disminuye 1 en tiros faltantes
            al_play_sample(lanzar, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        }
        break;
    case ALLEGRO_EVENT_TIMER:
        if (ev.timer.source == timer && personaje->bala.activo) {
            if (personaje->bala.x < 0 || personaje->bala.x > ANCHO || personaje->bala.y < 0 || personaje->bala.y > SUELO) {
                personaje->bala.activo = 0;  // Desactivar el proyectil
            }
        }
        break;
    }
}