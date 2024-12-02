# PR-QuadTree

Este proyecto implementa una estructura de datos QuadTree en C++ utilizando la librería SFML para visualización gráfica. Se puede agregar, mover, eliminar y verificar colisiones entre objetos dentro de un QuadTree.

## Estructura del Proyecto

El proyecto tiene la siguiente estructura de archivos:

- `Roboto-Regular.ttf`: Archivo de fuente utilizado para la visualización de texto.
- `main.cpp`: Archivo principal que ejecuta la demo de SFML.
- `QuadTreeDemo`: Directorio para la demo de SFML.
- `QuadTree.cpp`: Implementación de la estructura QuadTree con SFML.
- `QuadTree.h`: Encabezado con las definiciones de las clases y estructuras principales.
- `README.md`: Este archivo con la documentación del proyecto.

## Compilación

Para compilar el proyecto en un sistema basado en Ubuntu, primero asegúrate de tener instalada la librería SFML:

```bash
sudo apt-get install libsfml-dev
```

Luego, puedes compilar el proyecto con el siguiente comando en la terminal:

```bash
g++ -o QuadTreeDemo main.cpp QuadTree.cpp -lsfml-graphics -lsfml-window -lsfml-system
```

### Controles

- **Esc**: Cierra la ventana.
- **F**: Congela/descongela el movimiento de los objetos.
- **C**: Limpia el QuadTree y elimina todos los objetos.
- **Flecha arriba**: Aumenta el tamaño del cuadro de colisión del mouse.
- **Flecha abajo**: Disminuye el tamaño del cuadro de colisión del mouse.
- **Click izquierdo**: Agrega un objeto en la posición del mouse.
- **Click derecho**: Elimina los objetos dentro del cuadro de colisión del mouse.

