# 🎯 Fruit Shooter

**Fruit Shooter** es un juego tipo arcade desarrollado en C++ utilizando la biblioteca **Allegro**. El objetivo es disparar y eliminar frutas que aparecen en pantalla antes de que el tiempo se acabe o se acumulen demasiadas.

---

## 🕹️ Gameplay

- Controla a tu personaje con el teclado.
- Apunta y dispara proyectiles para eliminar frutas.
- A medida que avanzas, las frutas aparecen más rápido.
- ¡Evita que se acumulen o perderás!

---

## 🚀 Características

- Múltiples niveles de dificultad.
- Sistema de puntaje y partidas.
- Interfaz HUD en tiempo real.
- Animaciones básicas con Allegro.
- Menú principal, pausa y final de partida.

---

## 🎮 Controles

| Acción             | Tecla / Acción                           |
|--------------------|------------------------------------------|
| Mover izquierda     | Flecha Izquierda (←)                     |
| Mover derecha       | Flecha Derecha (→)                       |
| Apuntar             | Movimiento del mouse                    |
| Disparar            | Click Izquierdo del mouse               |
| Pausar/Reanudar     | Tecla P                                 |
| Salir del juego     | Tecla ESC                               |

---

## ⚙️ Requisitos

Para compilar y ejecutar el juego necesitas tener instalada la biblioteca **Allegro 5**.

### En Ubuntu/Debian:
```bash
sudo apt install liballegro5-dev
```

### En Windows:
- Descarga [Allegro 5](https://liballeg.org/download.html)
- Asegúrate de agregar las rutas de inclusión y bibliotecas en tu IDE (Code::Blocks, Visual Studio, etc.)

---

## 🛠️ Compilación

### Desde terminal (Linux o WSL):

```bash
g++ -std=c++11 -o fruit_shooter \\
    main.cpp menu.cpp municion.cpp nivel.cpp partidas.cpp personaje.cpp proyectil.cpp \\
    utilidades.cpp bala.cpp enemigos.cpp eventos.cpp frutas.cpp hud.cpp \\
    -lallegro -lallegro_image -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec -lallegro_primitives
```

> ⚠️ Asegúrate de que todos los archivos `.cpp` y `.h` estén en el mismo directorio o ajusta las rutas según sea necesario.

---

## 📁 Estructura del Proyecto

```css
FruitShooter/
├── main.cpp
├── menu.cpp / menu.h
├── personaje.cpp / personaje.h
├── frutas.cpp / frutas.h
├── enemigos.cpp / enemigos.h
├── proyectil.cpp / proyectil.h
├── bala.cpp / bala.h
├── hud.cpp / hud.h
├── nivel.cpp / nivel.h
├── partidas.cpp / partidas.h
├── eventos.cpp / eventos.h
├── utilidades.cpp / utilidades.h
└── constantes.h
```

---

## 👨‍💻 Autor

Desarrollado por **Oscar Cifuentes Rivera**  
Estudiante de Ingeniería Civil en Computación e Informática – Universidad de Magallanes

> Proyecto personal creado como parte de una Asignatura (Programación Estructurada).

---

## 🧩 Licencia

Este proyecto está bajo la licencia MIT. Puedes modificarlo y distribuirlo libremente.

---
