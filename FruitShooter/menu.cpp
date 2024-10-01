#include "menu.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <stdio.h>
#include "partidas.h"

#define OPCIONES_MENU 3

int mostrar_menu_principal(ALLEGRO_DISPLAY* display, ALLEGRO_FONT* font) {
    int opcion = -1;
    while (opcion < 0 || opcion >= OPCIONES_MENU) {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_text(font, al_map_rgb(255, 255, 255), 400, 100, ALLEGRO_ALIGN_CENTRE, "Menú Principal");
        al_draw_text(font, al_map_rgb(255, 255, 255), 400, 200, ALLEGRO_ALIGN_CENTRE, "0. Iniciar");
        al_draw_text(font, al_map_rgb(255, 255, 255), 400, 250, ALLEGRO_ALIGN_CENTRE, "1. Ayuda");
        al_draw_text(font, al_map_rgb(255, 255, 255), 400, 300, ALLEGRO_ALIGN_CENTRE, "2. Salir");

        al_flip_display();

        ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
        al_register_event_source(event_queue, al_get_display_event_source(display));
        al_register_event_source(event_queue, al_get_keyboard_event_source());

        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (event.keyboard.keycode) {
            case ALLEGRO_KEY_0:
                opcion = 0;
                break;
            case ALLEGRO_KEY_1:
                opcion = 1;
                break;
            case ALLEGRO_KEY_2:
                opcion = 2;
                break;
            default:
                break;
            }
        }

        al_destroy_event_queue(event_queue);
    }

    return opcion;
}

void iniciar_juego(ALLEGRO_DISPLAY* display, ALLEGRO_FONT* font) {
    int subopcion = -1;
    while (subopcion < 0 || subopcion > 2) {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_text(font, al_map_rgb(255, 255, 255), 400, 100, ALLEGRO_ALIGN_CENTRE, "Iniciar Juego");
        al_draw_text(font, al_map_rgb(255, 255, 255), 400, 200, ALLEGRO_ALIGN_CENTRE, "0. Nueva Partida");
        al_draw_text(font, al_map_rgb(255, 255, 255), 400, 250, ALLEGRO_ALIGN_CENTRE, "1. Cargar Partida");
        al_draw_text(font, al_map_rgb(255, 255, 255), 400, 300, ALLEGRO_ALIGN_CENTRE, "2. Volver");

        al_flip_display();

        ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
        al_register_event_source(event_queue, al_get_display_event_source(display));
        al_register_event_source(event_queue, al_get_keyboard_event_source());

        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (event.keyboard.keycode) {
            case ALLEGRO_KEY_0:
                nueva_partida(display, font);
                break;
            case ALLEGRO_KEY_1:
                cargar_partida(display, font);
                break;
            case ALLEGRO_KEY_2:
                return;
            default:
                break;
            }
        }

        al_destroy_event_queue(event_queue);
    }
}

void mostrar_ayuda(ALLEGRO_DISPLAY* display, ALLEGRO_FONT* font) {
    int subopcion = -1;
    while (subopcion < 0 || subopcion > 2) {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_text(font, al_map_rgb(255, 255, 255), 400, 100, ALLEGRO_ALIGN_CENTRE, "Ayuda");
        al_draw_text(font, al_map_rgb(255, 255, 255), 400, 200, ALLEGRO_ALIGN_CENTRE, "0. Frutas");
        al_draw_text(font, al_map_rgb(255, 255, 255), 400, 250, ALLEGRO_ALIGN_CENTRE, "1. Enemigos");
        al_draw_text(font, al_map_rgb(255, 255, 255), 400, 300, ALLEGRO_ALIGN_CENTRE, "2. Volver");

        al_flip_display();

        ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
        al_register_event_source(event_queue, al_get_display_event_source(display));
        al_register_event_source(event_queue, al_get_keyboard_event_source());

        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (event.keyboard.keycode) {
            case ALLEGRO_KEY_0:
                mostrar_frutas(display, font);
                break;
            case ALLEGRO_KEY_1:
                mostrar_enemigos(display, font);
                break;
            case ALLEGRO_KEY_2:
                return;
            default:
                break;
            }
        }

        al_destroy_event_queue(event_queue);
    }
}