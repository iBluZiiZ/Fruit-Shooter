#include "frutas.h"
#include "constantes.h"
#include "utilidades.h" // Para cargar_bitmap
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void inicializar_fruta(Fruta* fruta, TipoFruta tipo) {
	fruta->tipo = tipo;
	fruta->estado = 1;
	const char* filename = NULL;
	switch (tipo)
	{
	case MANZANA:
		fruta->bitmap = cargar_bitmap("imagenes/manzana.png");
		filename = "archivos/init_manzana.txt";
		fruta->valor = 10;
		break;

	case NARANJA:
		fruta->bitmap = cargar_bitmap("imagenes/naranja.png");
		filename = "archivos/init_naranja.txt";
		fruta->valor = 20;
		break;

	case SANDIA:
		fruta->bitmap = cargar_bitmap("imagenes/sandia.png");
		filename = "archivos/init_sandia.txt";
		fruta->valor = 30;
		break;

	case PERA: 
		fruta->bitmap = cargar_bitmap("imagenes/pera.png");
		filename = "archivos/init_pera.txt";
		fruta->valor = 25;
		break;

	case LIMON:
		fruta->bitmap = cargar_bitmap("imagenes/limon.png");
		filename = "archivos/init_limon.txt";
		fruta->valor = 20;
		break;

	default:
		break;
	}

	if (filename) {
		FILE* file = NULL;
		fopen_s(&file, filename, "r") == 0;
		if (file) {
			fscanf_s(file, "%f %f %f %f %d", &fruta->x, &fruta->y, &fruta->vel_x, &fruta->vel_y, &fruta->vida);
			fclose(file);
		}
		else {
			// SI NO SE PUEDE ABRIR EL ARCHIVO
			fruta->x = 0;
			fruta->y = 0;
			fruta->vel_x = 0;
			fruta->vel_y = 0;
			fruta->vida = 1; // Vida por defecto
		}
	}
	else {
		fruta->x = 0;
		fruta->y = 0;
		fruta->vel_x = 0;
		fruta->vel_y = 0;
		fruta->vida = 1; // Vida por defecto
	}
}

void mover_fruta(Fruta* fruta) {
	if (!fruta->estado) return;
	fruta->x += fruta->vel_x;
	fruta->y += fruta->vel_y;
	switch (fruta->tipo)
	{
	case MANZANA:
		if (fruta->x + al_get_bitmap_width(fruta->bitmap) < 0 || fruta->y > SUELO) {
			fruta->estado = 0;
			inicializar_fruta(fruta, MANZANA);
		}
		break;

	case NARANJA:
		if (rand() % 100 < 5) {
			fruta->vel_y = (rand() % 
				- 2) * 1.0;
			fruta->vel_x = 5.0 + (rand() % 3);
		}
		if (fruta->x > ANCHO || fruta->y > SUELO) {
			fruta->estado = 0;
			inicializar_fruta(fruta, NARANJA);
		}
		break;

	case SANDIA:
		if(fruta->x < 0) {
			fruta->x = 0;
			fruta->vel_x = -fruta->vel_x; // Invertir dirección al alcanzar el borde izquierdo
		}
		else if (fruta->x > ANCHO - al_get_bitmap_width(fruta->bitmap)) {
			fruta->x = ANCHO - al_get_bitmap_width(fruta->bitmap);
			fruta->vel_x = -fruta->vel_x; // Invertir dirección al alcanzar el borde derecho
		}
		if (fruta->y > SUELO) {
			fruta->estado = 0;
			inicializar_fruta(fruta, SANDIA);
		}
		break;

	case PERA:
		fruta->y += AMPLITUD * sin(FRECUENCIA * fruta->x);
		if (fruta->x > ANCHO) {
			fruta->estado = 0;
			inicializar_fruta(fruta, PERA);
		}
		break;

	case LIMON:
		if (rand() % 100 < 5) {
			fruta->vel_y = (rand() % 5 - 2) * 1.0;
			fruta->vel_x = 5.0 + (rand() % 3);
		}
		if (fruta->x > ANCHO) {
			fruta->estado = 0;
			inicializar_fruta(fruta, LIMON);
		}
		break;
	
	default:
		break;
	}

}

void dibujar_fruta(const Fruta* fruta) {
	if (fruta->estado && fruta->cantidad > 0)
		if (fruta->bitmap)
			al_draw_bitmap(fruta->bitmap, fruta->x, fruta->y, 0);
}

void destruir_fruta(Fruta* fruta) {
	if (fruta->bitmap) {
		al_destroy_bitmap(fruta->bitmap);
	}
}