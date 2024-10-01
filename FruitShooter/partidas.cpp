#include "partidas.h"
#include <allegro5/allegro_font.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PARTIDAS 5

void nueva_partida(ALLEGRO_DISPLAY* display, ALLEGRO_FONT* font) {
    char nombre_usuario[50];
    int puntuacion = 0; // Puedes establecer una puntuación inicial si lo deseas

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(font, al_map_rgb(255, 255, 255), 400, 100, ALLEGRO_ALIGN_CENTRE, "Nueva Partida");
    al_draw_text(font, al_map_rgb(255, 255, 255), 400, 200, ALLEGRO_ALIGN_CENTRE, "Introduce el nombre del usuario:");
    al_flip_display();
    // Aquí deberías capturar la entrada del usuario para el nombre, puedes usar un campo de texto
    // Por simplicidad, se omite la captura de texto en este ejemplo.
    printf("Introduce el nombre del usuario: ");
    fgets(nombre_usuario, sizeof(nombre_usuario), stdin);
    nombre_usuario[strcspn(nombre_usuario, "\n")] = 0; // Quitar el salto de línea al final
    guardar_partida(nombre_usuario, puntuacion);
}

void cargar_partida(ALLEGRO_DISPLAY* display, ALLEGRO_FONT* font) {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(font, al_map_rgb(255, 255, 255), 400, 100, ALLEGRO_ALIGN_CENTRE, "Cargar Partida");
    listar_partidas(display, font);
    al_flip_display();
}

void mostrar_frutas(ALLEGRO_DISPLAY* display, ALLEGRO_FONT* font) {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(font, al_map_rgb(255, 255, 255), 400, 100, ALLEGRO_ALIGN_CENTRE, "Frutas");
    al_draw_text(font, al_map_rgb(255, 255, 255), 400, 150, ALLEGRO_ALIGN_CENTRE, "Manzana: Fruta roja, se da en los árboles.");
    al_draw_text(font, al_map_rgb(255, 255, 255), 400, 200, ALLEGRO_ALIGN_CENTRE, "Naranja: Fruta cítrica de color naranja.");
    al_draw_text(font, al_map_rgb(255, 255, 255), 400, 250, ALLEGRO_ALIGN_CENTRE, "Pera: Fruta dulce de forma redondeada.");
    al_draw_text(font, al_map_rgb(255, 255, 255), 400, 300, ALLEGRO_ALIGN_CENTRE, "Sandía: Fruta grande con pulpa dulce.");
    al_draw_text(font, al_map_rgb(255, 255, 255), 400, 350, ALLEGRO_ALIGN_CENTRE, "Limón: Fruta cítrica de color amarillo.");

    al_draw_text(font, al_map_rgb(255, 255, 255), 400, 450, ALLEGRO_ALIGN_CENTRE, "Presiona cualquier tecla para volver.");
    al_flip_display();

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);

    al_destroy_event_queue(event_queue);
}

void mostrar_enemigos(ALLEGRO_DISPLAY* display, ALLEGRO_FONT* font) {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(font, al_map_rgb(255, 255, 255), 400, 100, ALLEGRO_ALIGN_CENTRE, "Enemigos");
    al_draw_text(font, al_map_rgb(255, 255, 255), 400, 150, ALLEGRO_ALIGN_CENTRE, "Murciélago: Enemigo que vuela y ataca al jugador.");
    al_draw_text(font, al_map_rgb(255, 255, 255), 400, 200, ALLEGRO_ALIGN_CENTRE, "Serpiente: Enemigo que se mueve en el suelo.");
    al_draw_text(font, al_map_rgb(255, 255, 255), 400, 250, ALLEGRO_ALIGN_CENTRE, "Fantasma: Enemigo que aparece y desaparece.");

    al_draw_text(font, al_map_rgb(255, 255, 255), 400, 350, ALLEGRO_ALIGN_CENTRE, "Presiona cualquier tecla para volver.");
    al_flip_display();

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);

    al_destroy_event_queue(event_queue);
}

void guardar_partida(const char* nombre_usuario, int puntuacion) {
    FILE* file = fopen("partidas.txt", "r+");
    if (!file) {
        file = fopen("partidas.txt", "w+");
    }
    if (file) {
        char linea[100];
        int partidas_guardadas = 0;
        char partidas[MAX_PARTIDAS][50];

        while (fgets(linea, sizeof(linea), file) && partidas_guardadas < MAX_PARTIDAS) {
            sscanf(linea, "%49[^,],%d", partidas[partidas_guardadas], &puntuacion);
            partidas_guardadas++;
        }

        // Si hay más de 5 partidas, se sobrescribe la más antigua
        if (partidas_guardadas >= MAX_PARTIDAS) {
            fseek(file, 0, SEEK_SET);
        }

        fprintf(file, "%s,%d\n", nombre_usuario, puntuacion);
        fclose(file);
    }
}

void listar_partidas(ALLEGRO_DISPLAY* display, ALLEGRO_FONT* font) {
    FILE* file = fopen("partidas.txt", "r");
    if (file) {
        char linea[100];
        int y = 150;
        while (fgets(linea, sizeof(linea), file)) {
            al_draw_text(font, al_map_rgb(255, 255, 255), 400, y, ALLEGRO_ALIGN_CENTRE, linea);
            y += 50;
        }

        fclose(file);
    }
}
