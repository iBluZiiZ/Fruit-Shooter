#include "municion.h"
#include "constantes.h"
#include "utilidades.h" // Para cargar_bitmap
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void inicializar_caja(Caja* caja) {
	caja->bitmap = cargar_bitmap("imagenes/globo.png");
	caja->activo = 0;
	caja->x = 0 - al_get_bitmap_width(caja->bitmap)-31;
	caja->y = rand() % (LARGO - SUELO - al_get_bitmap_height(caja->bitmap));
	caja->vel_x = 2.0;
}

void dibujar_Caja(Caja* caja) {
	if (caja->activo==1) {
		al_draw_bitmap(caja->bitmap, caja->x, caja->y, 0);
	}
}