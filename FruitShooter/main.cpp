#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "bala.h"
#include "frutas.h"
#include "personaje.h"
#include "utilidades.h"
#include "proyectil.h"
#include "eventos.h"
#include "constantes.h"
#include "enemigos.h"
#include "hud.h"
#include "nivel.h"
#include "municion.h"

void inicializar_allegro();
int manejar_menu(ALLEGRO_BITMAP* menu_null, ALLEGRO_BITMAP* menu_jugar, ALLEGRO_BITMAP* menu_ayuda, ALLEGRO_BITMAP* menu_salir, ALLEGRO_BITMAP* menu_ranking, ALLEGRO_EVENT_QUEUE* event_queue);
void obtener_nombre_jugador(char* nombre, ALLEGRO_FONT* font, ALLEGRO_EVENT_QUEUE* event_queue);
void actualizar_puntaje(char* nombre, int tiempo, int puntaje, int tiros);
void mostrar_ranking(ALLEGRO_FONT* font32, ALLEGRO_FONT* font, ALLEGRO_EVENT_QUEUE* event_queue);
void derrota(Fruta frutas[], int* running, ALLEGRO_FONT* font, ALLEGRO_DISPLAY* display, ALLEGRO_BITMAP* background, ALLEGRO_EVENT_QUEUE* event_queue, Personaje personaje);

int main() {
    // Inicializar variables
    int niv = 1;
    int perder = 0;
    int puntaje = 0;
    int running = 1;
    int colision = 0;
    int i = 0;
    int pausa = 0;
    int opcion = 0; //opcion menu
    int tiempo_actual = 0;
    int tiempo_transcurrido = 0;
    int tiempo_inicial = 0;
    char nombre[MAX_NOMBRE] = "";
    float cursor_x = ANCHO / 2;
    float cursor_y = LARGO / 2;
    int contCaja = 0;   // Contador de aparición de caja de "Municion", se reestablece cuando comienza un nuevo nivel.
    inicializar_allegro();

    // CARGAR DISPLAY Y CURSOR
    ALLEGRO_DISPLAY* display = crear_display();
    ALLEGRO_BITMAP* background = cargar_bitmap("imagenes/fondo.png");
    ALLEGRO_BITMAP* cursor = cargar_bitmap("imagenes/cursor.png");

    // Inicializar personaje, proyectil (bala), frutas y enemigos
    Personaje personaje;
    inicializar_personaje(&personaje);
    Bala bala = { 0 };
    Fruta frutas[NUM_TIPOS_FRUTAS];
    Enemigo enemigos[NUM_TIPOS_ENEMIGOS];
    Caja caja;
    inicializar_caja(&caja);

    // Cargar Niveles
    Nivel nivel;
    cargar_nivel(&nivel, "niveles", niv);
    inicializar_nivel(&nivel, frutas, enemigos, &personaje);

    // 5 BITMAPS PARA MENU
    ALLEGRO_BITMAP* menu_null = cargar_bitmap("imagenes/menu_null.png");
    ALLEGRO_BITMAP* menu_jugar = cargar_bitmap("imagenes/menu_jugar.png");
    ALLEGRO_BITMAP* menu_ayuda = cargar_bitmap("imagenes/menu_ayuda.png");
    ALLEGRO_BITMAP* menu_ayuda2 = cargar_bitmap("imagenes/menu_ayuda2.png");
    ALLEGRO_BITMAP* menu_salir = cargar_bitmap("imagenes/menu_salir.png");
    ALLEGRO_BITMAP* menu_ranking = cargar_bitmap("imagenes/menu_ranking.png");

    // CARGAR FUENTE, SONIDOS, EVENTO Y TIEMPO
    ALLEGRO_FONT* font = cargar_fuente("fuentes/upheavtt.ttf", 20);
    ALLEGRO_FONT* font32 = cargar_fuente("fuentes/upheavtt.ttf", 32);
    ALLEGRO_SAMPLE* golpe_fruta = al_load_sample("sonidos/golpe.mp3");  // Sonido golpe a fruta
    ALLEGRO_SAMPLE* golpe_hamburguesa = al_load_sample("sonidos/golpe_hamburgesa.mp3");    // Sonido golpe a Hamburguesa
    ALLEGRO_SAMPLE* lanzar = al_load_sample("sonidos/lanzar.mp3");  // Sonido lanzamiento pelota
    ALLEGRO_SAMPLE* golpe_caja = al_load_sample("sonidos/caja.mp3");  // Sonido golpe caja
    ALLEGRO_SAMPLE* musica_menu = al_load_sample("sonidos/music_menu2.mp3");
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);

    registrar_eventos(event_queue, display, timer);

    // MENU
    al_play_sample(musica_menu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
    while (running == 1)
    {
        opcion = manejar_menu(menu_null, menu_jugar, menu_ayuda, menu_salir, menu_ranking, event_queue);
        switch (opcion)
        {
        case 1: {   // Jugar
            al_destroy_sample(musica_menu);
            obtener_nombre_jugador(nombre, font, event_queue);
            printf("Nombre de Jugador: %s\n", nombre);  // Muestra nombre registrado
            running = 2;
            break;
        }
        case 2: {   // Ayuda
            while (opcion==2) {
                ALLEGRO_EVENT ev;
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_bitmap(menu_ayuda2, 0, 0, 0);
                al_flip_display();
                al_wait_for_event(event_queue, &ev);
                if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                    running = 1;
                    opcion = 1;
                }
                al_rest(0.01);
            }
            break;
        }
        case 3: {   // Salir
            running = 0;
            break;
        }
        case 4: {   // Muestra Ranking
            mostrar_ranking(font32, font, event_queue);
            running = 1;
            break;
        }

        default:
            break;
        }
    }
    al_destroy_bitmap(menu_null);
    al_destroy_bitmap(menu_jugar);
    al_destroy_bitmap(menu_ayuda);
    al_destroy_bitmap(menu_ranking);
    al_destroy_bitmap(menu_salir);
    tiempo_inicial = al_get_time();
    al_start_timer(timer);
    while (running==2) { // Empieza Juego
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        manejar_evento(ev, &personaje, enemigos, &bala, &running, &pausa, timer, lanzar);

        if (pausa) {
            // Dibujar el mensaje de pausa
            al_draw_bitmap(background, 0, 0, 0);
            al_draw_filled_rectangle(0, LARGO / 2, ANCHO, LARGO / 2 + 20, al_map_rgba_f(0.5, 0.5, 0.5, 0.7));
            al_draw_text(font, al_map_rgb(0, 0, 0), ANCHO / 2, LARGO / 2, ALLEGRO_ALIGN_CENTRE, "Juego en pausa. Presiona 'P' para continuar.");
            al_flip_display();
            continue;  // Si el juego está en pausa, no actualizar ni dibujar
        }
        if (nivel_completado(frutas, enemigos)) {   // MANEJO DE NIVEL COMPLETADO O DERROTA
            tiempo_actual = al_get_time();
            al_stop_timer(timer);
            tiempo_transcurrido = tiempo_actual - tiempo_inicial;
            actualizar_puntaje(nombre, tiempo_transcurrido, puntaje, personaje.cont_tiros);
            printf("\nNIVEL %d COMPLETADO en %d segundos con %d tiros\n", niv, tiempo_transcurrido, personaje.cont_tiros);
            niv++;
            manejar_nivel(&nivel, &personaje, frutas, enemigos, display, font, niv, &running, &bala, tiempo_transcurrido, puntaje);
            personaje.cont_tiros = 0;
            if (contCaja == 0) {
                personaje.bala.tiros += 8;
            }
            else
            {
                personaje.bala.tiros += 5;
                contCaja = 0;
            }
            tiempo_inicial = al_get_time();
            al_start_timer(timer);
            inicializar_caja(&caja);
        }

        if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
            cursor_x = ev.mouse.x;
            cursor_y = ev.mouse.y;
        }

        if (ev.type == ALLEGRO_EVENT_TIMER && ev.timer.source == timer) {
            // MOVER FRUTAS
            for (i = 0; i < NUM_TIPOS_FRUTAS; i++) {
                if (frutas[i].cantidad == 0) {}
                else {
                    mover_fruta(&frutas[i]);
                }
            }
            // MOVER ENEMIGOS
            for (i = 0; i < NUM_TIPOS_ENEMIGOS; i++) {
                if (enemigos[i].cantidad == 0) {}
                else {
                    mover_enemigo(&enemigos[i], 1.0 / FPS, &personaje);
                }
            }
            //Caja
            if (personaje.bala.tiros < 3 && contCaja == 0) {
                // Hacer aparecer la caja de municiones
                caja.activo = 1;
                caja.x += caja.vel_x;
                if (caja.x > ANCHO + al_get_bitmap_width(caja.bitmap)) {
                    caja.activo = 0;
                }
            }
            // MOVER PROYECTIL (bala)
            mover_proyectil(&personaje, enemigos, Balon);

            colision = chequear_colision(&personaje, frutas, enemigos, &caja);
            switch (colision) {
            case 1:
                personaje.bala.activo = 0;
                frutas[MANZANA].vida--;
                if (frutas[MANZANA].vida <= 0) {
                    al_play_sample(golpe_fruta, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    printf("Hit Manzana\n");
                    puntaje += frutas[MANZANA].valor;
                    frutas[MANZANA].estado = 0;
                    frutas[MANZANA].cantidad--;
                    if (frutas[MANZANA].cantidad > 0) {
                        printf("Quedan %d Manzanas\n", frutas[MANZANA].cantidad);
                        inicializar_fruta(&frutas[MANZANA], MANZANA);
                    }
                    else {
                        printf("Se ha eliminado MANZANA\n");
                    }
                }
                break;
            case 2:
                personaje.bala.activo = 0;
                frutas[NARANJA].vida--;
                if (frutas[NARANJA].vida <= 0) { 
                    al_play_sample(golpe_fruta, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    printf("Hit Naranja\n");
                    puntaje += frutas[NARANJA].valor;
                    frutas[NARANJA].estado = 0;
                    frutas[NARANJA].cantidad--;
                    if (frutas[NARANJA].cantidad > 0) {
                        printf("Quedan %d Naranjas\n", frutas[NARANJA].cantidad);
                        inicializar_fruta(&frutas[NARANJA], NARANJA);
                    }
                    else {
                        printf("Se ha eliminado NARANJA\n");
                    }
                }
                break;
            case 3:
                personaje.bala.activo = 0;
                frutas[SANDIA].vida--;
                if (frutas[SANDIA].vida <= 0) {
                    al_play_sample(golpe_fruta, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    printf("Hit SANDIA\n");
                    puntaje += frutas[SANDIA].valor;
                    frutas[SANDIA].estado = 0;
                    frutas[SANDIA].cantidad--;
                    if (frutas[SANDIA].cantidad > 0) {
                        printf("Quedan %d Sandia\n", frutas[SANDIA].cantidad);
                        inicializar_fruta(&frutas[SANDIA], SANDIA);
                    }
                    else {
                        printf("Se ha eliminado SANDIA\n");
                    }
                }
                break;
            case 4:
                personaje.bala.activo = 0;
                frutas[PERA].vida--;
                if (frutas[PERA].vida <= 0) {
                    al_play_sample(golpe_fruta, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    printf("Hit PERA\n");
                    puntaje += frutas[PERA].valor;
                    frutas[PERA].estado = 0;
                    frutas[PERA].cantidad--;
                    if (frutas[PERA].cantidad > 0) {
                        printf("Quedan %d PERA\n", frutas[PERA].cantidad);
                        inicializar_fruta(&frutas[PERA], PERA);
                    }
                    else {
                        printf("Se ha eliminado PERA\n");
                    }
                }
                frutas[PERA].vel_x--;
                break;
            case 5:
                personaje.bala.activo = 0;
                frutas[LIMON].vida--;
                if (frutas[LIMON].vida <= 0) {
                    al_play_sample(golpe_fruta, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    printf("Hit LIMON\n");
                    puntaje += frutas[LIMON].valor;
                    frutas[LIMON].estado = 0;
                    frutas[LIMON].cantidad--;
                    if (frutas[LIMON].cantidad > 0) {
                        printf("Quedan %d LIMON\n", frutas[LIMON].cantidad);
                        inicializar_fruta(&frutas[LIMON], LIMON);
                    }
                    else {
                        printf("Se ha eliminado LIMON\n");
                    }
                }
                break;
            case 6:
                personaje.bala.activo = 0;
                enemigos[PALOMA].vida--;
                if (enemigos[PALOMA].vida <= 0) {
                    al_play_sample(golpe_fruta, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    printf("Hit PALOMA\n");
                    puntaje -= enemigos->valor;
                    enemigos[PALOMA].estado = 0;
                    enemigos[PALOMA].cantidad--;
                    personaje.bala.tiros--;
                    if (enemigos[PALOMA].cantidad > 0) {
                        printf("Quedan %d PALOMAS\n", enemigos[PALOMA].cantidad);
                        inicializar_enemigo(&enemigos[PALOMA], PALOMA);
                    }
                    else {
                        printf("Se ha eliminado PALOMA\n");
                    }
                    break;
                }
            case 7:
                personaje.bala.activo = 0;
                enemigos[HAMBURGUESA].vida--;
                personaje.gordito = true;
                if (enemigos[HAMBURGUESA].vida <= 0) {
                    al_play_sample(golpe_hamburguesa, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    printf("Hit Hamburguesa\n");
                    puntaje -= enemigos->valor;
                    enemigos[HAMBURGUESA].estado = 0;
                    enemigos[HAMBURGUESA].cantidad--;
                    personaje.vel = 4.0;
                    if (enemigos[HAMBURGUESA].cantidad > 0) {
                        printf("Quedan %d HAMBURGUESAS\n", enemigos[HAMBURGUESA].cantidad);
                        inicializar_enemigo(&enemigos[HAMBURGUESA], HAMBURGUESA);
                    }
                    else {
                        printf("Se ha eliminado HAMBURGUESAS\n");
                    }
                    break;
                }
            case 8:
                personaje.bala.activo = 0;
                inicializar_caja(&caja);
                personaje.bala.tiros += 5;
                contCaja++;
                al_play_sample(golpe_caja, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            default:
                break;
            }
            
        }
        if (personaje.bala.tiros == 0 && personaje.bala.activo == 0) {
            derrota(frutas, &running, font, display, background, event_queue, personaje);
        }
        else {}
        
        al_clear_to_color(al_map_rgb(0, 0, 0));
        // Dibujar imagen de fondo
        al_draw_bitmap(background, 0, 0, 0);

        // Dibujar Personaje, Proyectil (bala), Frutas y Enemigos
        dibujar_personaje(&personaje);
        dibujar_proyectil(&personaje, enemigos);
        
        for (i = 0; i < NUM_TIPOS_FRUTAS; i++) {
            if (frutas[i].cantidad == 0) {}
            else {
                dibujar_fruta(&frutas[i]);
            }
        }
        for (i = 0; i < NUM_TIPOS_ENEMIGOS; i++) {
            if (enemigos[i].cantidad == 0) {}
            else {
                dibujar_enemigo(&enemigos[i]);
            }
        }
        if (caja.activo==1) {
            dibujar_Caja(&caja);
        }

        // Dibujar el cursor personalizado en la posición del cursor
        dibujar_cursor(cursor, cursor_x, cursor_y);

        // Scoreboard
        dibujar_hud(font, &personaje, puntaje, niv, timer, nombre);

        // Actualizar la pantalla
        al_flip_display();
    }

    limpiar_recursos(event_queue, background, cursor, display);
    al_destroy_timer(timer);
    al_destroy_font(font);
    al_destroy_font(font32);
    al_destroy_sample(golpe_fruta);
    al_destroy_sample(lanzar);
    al_destroy_sample(golpe_caja);
    return 0;
}

void inicializar_allegro() {
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    al_install_mouse();
    al_install_audio();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_acodec_addon();
    al_reserve_samples(10);
}

int manejar_menu(ALLEGRO_BITMAP* menu_null, ALLEGRO_BITMAP* menu_jugar, ALLEGRO_BITMAP* menu_ayuda, ALLEGRO_BITMAP* menu_salir, ALLEGRO_BITMAP* menu_ranking, ALLEGRO_EVENT_QUEUE* event_queue) {
    int seleccion = 0; // 0: Nada seleccionado, 1: Jugar, 2: Ayuda, 3: Salir
    int running = 1;

    while (running) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            if (ev.mouse.x >= 281 && ev.mouse.y >= 299 && ev.mouse.x <= 519 && ev.mouse.y <= 371) {
                seleccion = 1; // Jugar
                running = 0;
            }
            else if (ev.mouse.x >= 281 && ev.mouse.y >= 381 && ev.mouse.x <= 519 && ev.mouse.y <= 453) {
                seleccion = 2; // Ayuda
                running = 0;
            }
            else if (ev.mouse.x >= 281 && ev.mouse.y >= 462 && ev.mouse.x <= 519 && ev.mouse.y <= 534) {
                seleccion = 3; // Salir
                running = 0;
            }
            else if (ev.mouse.x >= 55 && ev.mouse.y >= 481 && ev.mouse.x <= 116 && ev.mouse.y <= 540) {
                seleccion = 4;// Ranking
                running = 0;
            }
            else {}
        }
        else{}
        al_draw_bitmap(menu_null, 0, 0, 0);

        if (ev.mouse.x >= 281 && ev.mouse.y >= 299 && ev.mouse.x <= 519 && ev.mouse.y <= 371) {
            al_draw_bitmap(menu_jugar, 0, 0, 0);
        }
        else if (ev.mouse.x >= 281 && ev.mouse.y >= 381 && ev.mouse.x <= 519 && ev.mouse.y <= 453) {
            al_draw_bitmap(menu_ayuda, 0, 0, 0);
        }
        else if (ev.mouse.x >= 281 && ev.mouse.y >= 462 && ev.mouse.x <= 519 && ev.mouse.y <= 534) {
            al_draw_bitmap(menu_salir, 0, 0, 0);
        }
        else if (ev.mouse.x >= 55 && ev.mouse.y >= 481 && ev.mouse.x <= 116 && ev.mouse.y <= 540) {
            al_draw_bitmap(menu_ranking, 0, 0, 0);
        }
        else{}

        al_flip_display();
    }

    return seleccion;
}

void obtener_nombre_jugador(char* nombre, ALLEGRO_FONT* font, ALLEGRO_EVENT_QUEUE* event_queue) {
    ALLEGRO_EVENT ev;
    int nombre_incompleto = 1;
    int posicion_cursor = 0;
    char buffer[MAX_NOMBRE] = "";
    char line[MAX_NOMBRE + 20];
    int nombre_existente = 0;
    char nombre_existente_archivo[MAX_NOMBRE];

    while (nombre_incompleto) {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_text(font, al_map_rgb(255, 255, 255), ANCHO / 2, LARGO / 2 - 30, ALLEGRO_ALIGN_CENTRE, "Ingresa tu nombre:");
        al_draw_text(font, al_map_rgb(255, 255, 255), ANCHO / 2, LARGO / 2 + 10, ALLEGRO_ALIGN_CENTRE, buffer);
        al_flip_display();
        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_KEY_CHAR) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) { //continua con el nombre escogido
                if (strlen(buffer) > 0) {   // strlen sirve para calcular longitud de una cadena de caracteres, en este caso buffer que es el nombre que se ingresa
                    // verifica si el nombre ya se encuentra en el archivo
                    FILE* file;
                    fopen_s(&file, "archivos/ranking.txt", "r");
                    if (file != NULL) {
                        while (fgets(line, sizeof(line), file)) {   // fgets sirve para leer una linea de texto en un archivo
                            sscanf_s(line, "%s", nombre_existente_archivo, (unsigned)_countof(nombre_existente_archivo));
                            if (strcmp(nombre_existente_archivo, buffer) == 0) {    // strcmp comparacion de 2 cadenas
                                nombre_existente = 1;
                                break;
                            }
                        }
                        fclose(file);
                        if (nombre_existente) {
                            al_clear_to_color(al_map_rgb(0, 0, 0));
                            al_draw_text(font, al_map_rgb(255, 0, 0), ANCHO / 2, LARGO / 2 + 30, ALLEGRO_ALIGN_CENTRE, "No se encuentra disponible ese nombre. Intente con otro.");
                            al_flip_display();
                            nombre_existente = 0;
                            al_rest(2.0);
                        }
                        else {
                            strncpy_s(nombre, MAX_NOMBRE, buffer, _TRUNCATE);   // copia una cadena de caracteres, lo que se escribe en buffer se copia en nombre
                            nombre_incompleto = 0;
                        }
                    }
                    else {
                        fprintf(stderr, "No se puede abrir ranking.txt\n");
                        nombre_incompleto = 0;
                    }
                }
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {
                if (posicion_cursor > 0) {
                    buffer[--posicion_cursor] = '\0';
                }
            }
            else if (posicion_cursor < MAX_NOMBRE - 1 && ev.keyboard.unichar >= 32 && ev.keyboard.unichar <= 126) {
                buffer[posicion_cursor++] = (char)ev.keyboard.unichar;
                buffer[posicion_cursor] = '\0';
            }
        }
    }
}

void actualizar_puntaje(char* nombre, int tiempo, int puntaje, int tiros) {
    int puntuacion_nivel = puntaje - (tiempo * 1) - (tiros * 3);
    printf("\nPuntuacion Nivel: %d", puntuacion_nivel);
    char nombre_actual[100];
    int puntuacion;
    char linea[100];
    int encontrado = 0;
    int puntuacion_actual = 0;
    FILE* archivo;
    FILE* temp;
    // se abre archivo en modo r (modo lectura)
    fopen_s(&archivo, "archivos/ranking.txt", "r");
    if (archivo == NULL) {
        fprintf(stderr, "No se puede abrir ranking.txt\n");
        return;
    }
    char temp_file[] = "archivos/temp_ranking.txt";
    fopen_s(&temp, temp_file, "w");
    if (temp == NULL) {
        fprintf(stderr, "No se puede crear el archivo temporal\n");
        fclose(archivo);
        return;
    }
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        // leer el nombre y el puntaje de la línea actual
        if (sscanf_s(linea, "%99s %d", nombre_actual, (unsigned)_countof(nombre_actual), &puntuacion) == 2) {
            if (strcmp(nombre_actual, nombre) == 0) {
                puntuacion += puntuacion_nivel; // sumar puntaje de nivel más del de archivo
                encontrado = 1;
            }
            fprintf(temp, "%s %d\n", nombre_actual, puntuacion);
        }
        else {
            fprintf(temp, "%s", linea);
        }
    }
    if (!encontrado) {
        fprintf(temp, "%s %d\n", nombre, puntuacion_nivel);
    }
    fclose(archivo);
    fclose(temp);
    if (remove("archivos/ranking.txt") != 0) {}
    if (rename(temp_file, "archivos/ranking.txt") != 0) {}
}

void mostrar_ranking(ALLEGRO_FONT* font32, ALLEGRO_FONT* font, ALLEGRO_EVENT_QUEUE* event_queue) {
    ALLEGRO_EVENT ev;
    char ranking[100][MAX_NOMBRE + 20];
    int num_ranking = 0;
    int i = 0, j = 0, y = 0;
    FILE* archivo;
    int puntuacion_i, puntuacion_j;
    char temp[MAX_NOMBRE + 20];
    int running = 1;
    ALLEGRO_BITMAP* menu_ranking2 = cargar_bitmap("imagenes/menu_ranking2.jpg");
    // leer archivo ranking y almacenarlo en una lista
    fopen_s(&archivo, "archivos/ranking.txt", "r");
    if (archivo == NULL) {
        return;
    }
    while (fgets(ranking[num_ranking], sizeof(ranking[num_ranking]), archivo)) {
        num_ranking++;
    }
    fclose(archivo);
    for (i = 0; i < num_ranking - 1; i++) {
        for (j = i + 1; j < num_ranking; j++) {
            sscanf_s(ranking[i], "%*s %d", &puntuacion_i);
            sscanf_s(ranking[j], "%*s %d", &puntuacion_j);
            if (puntuacion_i < puntuacion_j) {
                strcpy_s(temp, sizeof(temp), ranking[i]);
                strcpy_s(ranking[i], sizeof(ranking[i]), ranking[j]);
                strcpy_s(ranking[j], sizeof(ranking[j]), temp);
            }
        }
    }
    while (running) {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(menu_ranking2, 0, 0, 0);
        al_draw_text(font32, al_map_rgb(184, 192, 22), ANCHO / 2, 20, ALLEGRO_ALIGN_CENTRE, "Ranking");
        y = 60;
        for (i = 0; i < num_ranking; i++) {
            al_draw_text(font, al_map_rgb(255, 255, 255), ANCHO / 2, y, ALLEGRO_ALIGN_CENTRE, ranking[i]);
            y += 30;
        }
        al_draw_text(font, al_map_rgb(255, 255, 255), ANCHO / 2, LARGO - 40, ALLEGRO_ALIGN_CENTRE, "Presiona 'ESPACIO' para volver al menu.");
        al_flip_display();
        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_SPACE) {
            running = 0;
        }
    }
}

void derrota(Fruta frutas[], int* running, ALLEGRO_FONT* font, ALLEGRO_DISPLAY* display, ALLEGRO_BITMAP* background, ALLEGRO_EVENT_QUEUE* event_queue, Personaje personaje) {
    int caida = 0, i = 0;
    ALLEGRO_SAMPLE* perder = al_load_sample("sonidos/derrota.mp3");
    al_play_sample(perder, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

    while (!caida) {
        caida = 1;
        for (i = 0; i < NUM_TIPOS_FRUTAS; i++) {
            if (frutas[i].cantidad > 0 && frutas[i].y < SUELO) {
                frutas[i].y += 3; // Velocidad de caída
                caida = 0;
            }
        }
        al_draw_bitmap(background, 0, 0, 0);
        for (i = 0; i < NUM_TIPOS_FRUTAS; i++) {
            if (frutas[i].cantidad > 0) {
                dibujar_fruta(&frutas[i]);
            }
        }
        al_flip_display();
        al_rest(0.01);
    }
    al_rest(0.2);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(font, al_map_rgb(255, 255, 255), ANCHO / 2, LARGO / 2, ALLEGRO_ALIGN_CENTER, "¡Has perdido! Te has quedado sin tiros.");
    al_flip_display();
    while (true) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(event_queue, &evento);
        if (evento.type == ALLEGRO_EVENT_KEY_DOWN && evento.keyboard.keycode == ALLEGRO_KEY_SPACE) {
            break;
        }
    }
    *running = 0; // Finaliza el juego
    al_destroy_sample(perder);
}

