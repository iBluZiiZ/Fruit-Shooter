#pragma once
#ifndef PARTIDAS_H
#define PARTIDAS_H

void nueva_partida(ALLEGRO_DISPLAY* display, ALLEGRO_FONT* font);
void cargar_partida(ALLEGRO_DISPLAY* display, ALLEGRO_FONT* font);
void mostrar_frutas(ALLEGRO_DISPLAY* display, ALLEGRO_FONT* font);
void mostrar_enemigos(ALLEGRO_DISPLAY* display, ALLEGRO_FONT* font);
void guardar_partida(const char* nombre_usuario, int puntuacion);
void listar_partidas(ALLEGRO_DISPLAY* display, ALLEGRO_FONT* font);

#endif // PARTIDAS_H