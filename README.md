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

### En Ubuntu

Para compilar el proyecto en un sistema basado en Ubuntu, primero asegúrate de tener instalada la librería SFML:

```bash
sudo apt-get install libsfml-dev
```

Luego, puedes compilar el proyecto con el siguiente comando en la terminal:

```bash
g++ -o QuadTreeDemo main.cpp QuadTree.cpp -lsfml-graphics -lsfml-window -lsfml-system
```

### En Windows

Para compilar el proyecto en Windows, sigue estos pasos:

1. **Instalar SFML en Windows:**
   - Descarga la librería SFML desde [SFML Downloads](https://www.sfml-dev.org/download.php).
   - Elige la versión adecuada para tu compilador (por ejemplo, MinGW o Visual Studio).

2. **Compilación con MinGW:**
   Si usas MinGW, el proceso es similar al de Ubuntu, pero debes especificar las rutas a las librerías SFML. Suponiendo que tienes SFML instalado en `C:\SFML`, el comando sería:

   ```bash
   g++ -o QuadTreeDemo main.cpp QuadTree.cpp -I C:\SFML\include -L C:\SFML\lib -lsfml-graphics -lsfml-window -lsfml-system
   ```

   Esto le indica al compilador que busque los archivos de cabecera en `C:\SFML\include` y las librerías en `C:\SFML\lib`.

3. **Compilación con Visual Studio:**
   Si prefieres usar Visual Studio:
   - Abre Visual Studio y crea un nuevo proyecto de consola en C++.
   - Agrega los archivos `main.cpp`, `QuadTree.cpp`, y `QuadTree.h` al proyecto.
   - Configura las propiedades del proyecto:
     - `C++ -> General -> Additional Include Directories`: Agrega la carpeta `include` de SFML.
     - `Linker -> General -> Additional Library Directories`: Agrega la carpeta `lib` de SFML.
     - `Linker -> Input -> Additional Dependencies`: Agrega las librerías `sfml-graphics.lib`, `sfml-window.lib` y `sfml-system.lib`.

Después de compilar, el archivo `QuadTreeDemo.exe` debería estar disponible en tu directorio actual, y puedes ejecutarlo con:

```bash
./QuadTreeDemo.exe
```

### Controles

- **Esc**: Cierra la ventana.
- **F**: Congela/descongela el movimiento de los objetos.
- **C**: Limpia el QuadTree y elimina todos los objetos.
- **Flecha arriba**: Aumenta el tamaño del cuadro de colisión del mouse.
- **Flecha abajo**: Disminuye el tamaño del cuadro de colisión del mouse.
- **Click izquierdo**: Agrega un objeto en la posición del mouse.
- **Click derecho**: Elimina los objetos dentro del cuadro de colisión del mouse.

