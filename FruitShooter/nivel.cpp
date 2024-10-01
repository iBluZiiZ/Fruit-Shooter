#include "nivel.h"
#include "frutas.h"
#include "proyectil.h"
#include <stdio.h>
#include <stdlib.h>
#include "constantes.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

void cargar_nivel(Nivel* nivel, const char* ruta, int num_nivel) {
    char filename[100];
    int i = 0;
    snprintf(filename, sizeof(filename), "%s/nivel%d.txt", ruta, num_nivel);
    FILE* archivo;
    if (fopen_s(&archivo, filename, "r") != 0) {
        fprintf(stderr, "Error al abrir el archivo de nivel: %s\n", filename);
        exit(1);
    }
    else {
        printf("Se abrió archivo de nivel: %s\n", filename);
    }

    for (i = 0; i < NUM_TIPOS_FRUTAS; i++) {
        fscanf_s(archivo, "%d", &nivel->cantidad_frutas[i]);
        //printf("%d", nivel->cantidad_frutas[i]); Reconoce el archivo
    }
    for (i = 0; i < NUM_TIPOS_ENEMIGOS; i++) {
        fscanf_s(archivo, "%d", &nivel->cantidad_enemigos[i]);
    }
    nivel->nivel = num_nivel;

    fclose(archivo);
}

void inicializar_nivel(Nivel* nivel, Fruta* frutas, Enemigo* enemigos, Personaje* personaje) {
    int i = 0;
    for (i = 0; i < NUM_TIPOS_FRUTAS; i++) {
        frutas[i].cantidad = nivel->cantidad_frutas[i];
        if (frutas[i].cantidad == 0) {
            frutas[i].estado = 0;
        }
        else {
            inicializar_fruta(&frutas[i], (TipoFruta)i);
        }
    }

    for (i = 0; i < NUM_TIPOS_ENEMIGOS; i++) {
        enemigos[i].cantidad = nivel->cantidad_enemigos[i];
        if (enemigos[i].cantidad == 0) {
            enemigos[i].estado = 0;
        }
        else {
            inicializar_enemigo(&enemigos[i], (TipoEnemigo)i);
        }
    }
    inicializar_personaje(personaje);

}

int nivel_completado(Fruta* frutas, Enemigo* enemigos) {
    int i = 0;
    for (i = 0; i < NUM_TIPOS_FRUTAS; i++) {
        if (frutas[i].cantidad > 0) {
            return 0;
        }
    }
    return 1;
}

void manejar_nivel(Nivel* nivel, Personaje* personaje, Fruta* frutas, Enemigo* enemigos, ALLEGRO_DISPLAY* display, ALLEGRO_FONT* font, int nivel_actual, int* running, Bala* bala, int tiempo_transcurrido, int puntaje) {
    mostrar_pantalla_transicion(display, font, nivel_actual, tiempo_transcurrido, personaje, puntaje);
    printf("\nNivel entrante: %d\n", nivel_actual);

    if (nivel_actual > MAX_NIVEL) {
        // todos los niveles completados (max_nivel)
        al_draw_text(font, al_map_rgb(255, 255, 255), ANCHO / 2, LARGO / 2, ALLEGRO_ALIGN_CENTER, "¡Felicidades! Has completado el juego.");
        al_flip_display();
        al_rest(5.0);
        *running = 0;
    }
    else {
        // carga e inicializar el siguiente nivel
        cargar_nivel(nivel, "niveles", nivel_actual);
        inicializar_nivel(nivel, frutas, enemigos, personaje);
    }
}

void mostrar_pantalla_transicion(ALLEGRO_DISPLAY* display, ALLEGRO_FONT* font, int nivel, int tiempo_transcurrido, Personaje* personaje, int puntaje) {
    int puntuacion_nivel = puntaje - (tiempo_transcurrido * 1) - (personaje->cont_tiros * 3);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(font, al_map_rgb(255, 255, 255), al_get_display_width(display) / 2, al_get_display_height(display) / 2 - 40, ALLEGRO_ALIGN_CENTER, "Has pasado de Nivel.");
    al_draw_textf(font, al_map_rgb(255, 255, 255), al_get_display_width(display) / 2, al_get_display_height(display) / 2 - 20, ALLEGRO_ALIGN_CENTER, "Tiros:  %d", personaje->cont_tiros);
    al_draw_textf(font, al_map_rgb(255, 255, 255), al_get_display_width(display) / 2, al_get_display_height(display) / 2, ALLEGRO_ALIGN_CENTER, "Tiempo: %d", tiempo_transcurrido);
    al_draw_textf(font, al_map_rgb(255, 255, 255), al_get_display_width(display) / 2, al_get_display_height(display) / 2 + 20, ALLEGRO_ALIGN_CENTER, "Total:  %d", puntuacion_nivel);
    al_draw_textf(font, al_map_rgb(255, 255, 255), al_get_display_width(display) / 2, al_get_display_height(display) / 2 + 40, ALLEGRO_ALIGN_CENTER, "Presiona 'Espacio' para avanzar al siguiente Nivel.");
    al_flip_display();

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    while (true) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
            break;
        }
    }

    al_destroy_event_queue(event_queue);
}
