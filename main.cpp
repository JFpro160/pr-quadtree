#include <time.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <sstream>
#include "QuadTree_SFML_DEMO.h"

Rect MAP_BOUNDS = { 0, 0, 1920, 1020 }; // 1280, 720

struct Object {
    Object(double _x, double _y, double _width, double _height) {
        item = Collidable({ _x, _y, _width, _height }, this);
        shape.setPosition((float)item.bound.x, (float)item.bound.y);
        shape.setSize(sf::Vector2f((float)item.bound.width, (float)item.bound.height));
    }
    void move() {
        if (item.bound.x + dx < 0 || item.bound.x + item.bound.width + dx > MAP_BOUNDS.width)
            dx = -dx;
        if (item.bound.y + dy < 0 || item.bound.y + item.bound.height + dy > MAP_BOUNDS.height)
            dy = -dy;
        item.bound.x += dx;
        item.bound.y += dy;
        shape.setPosition((float)item.bound.x, (float)item.bound.y);
    }
    double dx = (rand() % 201 - 100) * 0.05f;
    double dy = (rand() % 201 - 100) * 0.05f;
    sf::RectangleShape shape;
    Collidable item;
};

int main() {
    srand((unsigned)time(NULL));
    sf::RenderWindow window(sf::VideoMode((unsigned)MAP_BOUNDS.width, (unsigned)MAP_BOUNDS.height), "QuadTree");
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);

    QuadTree map = QuadTree(MAP_BOUNDS, 2, 12); // bounds, capacity, maxLevel
    std::vector<Object*> objects;

    // Cargar una fuente moderna
    sf::Font font;
    if (!font.loadFromFile("Roboto-Regular.ttf")) { // Cambia a la fuente deseada
        return -1; // Si no se carga la fuente, salir del programa
    }
    map.setFont(font);

    // Crear el texto de información con un tamaño y color más atractivo
    sf::Text info("", font);
    info.setCharacterSize(22);  // Tamaño de fuente mayor
    info.setFillColor(sf::Color::Black); // Texto negro para buena visibilidad
    info.setStyle(sf::Text::Bold); // Texto en negrita
    info.setPosition(sf::Vector2f(10, 10));

    // Cuadro para el texto con fondo más transparente
    sf::RectangleShape textBox;
    textBox.setFillColor(sf::Color(255, 255, 255, 180)); // Fondo blanco con algo de transparencia
    textBox.setOutlineThickness(2.0f);
    textBox.setOutlineColor(sf::Color::Black);

    sf::Event event;
    sf::RectangleShape mouseBox;
    mouseBox.setOutlineThickness(3.0f);
    mouseBox.setFillColor(sf::Color(0, 255, 255, 128)); // Color de relleno más claro y semitransparente
    mouseBox.setOutlineColor(sf::Color::Cyan);

    bool freezeObjects = false;
    Rect mouseBoundary = { 0, 0, 20, 20 };

    // Temporizador para controlar el intervalo de inserción
    sf::Clock clock;

    while (window.isOpen()) {
        // Actualización de controles
        while (window.pollEvent(event) && event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
            case sf::Keyboard::Escape:
                window.close();
                break;
            case sf::Keyboard::F:
                freezeObjects = !freezeObjects;
                break;
            case sf::Keyboard::C:
                map.clear();
                for (auto&& obj : objects)
                    delete obj;
                objects.clear();
                break;
            case sf::Keyboard::Up:
                mouseBoundary.width += 2;
                mouseBoundary.height += 2;
                break;
            case sf::Keyboard::Down:
                mouseBoundary.width -= 2;
                mouseBoundary.height -= 2;
                break;
            }
        }

        // **Cambio de fondo**: Mantén el fondo blanco o claro, sin usar el negro.
        window.clear(sf::Color::White);  // Fondo blanco

        map.draw(window);

        // Actualización de colisiones
        std::vector<Object*> mouseCollisions;
        unsigned long long collisions = 0;
        unsigned long long qtCollisionChecks = 0;
        unsigned long long bfCollisionChecks = 0;
    for (auto&& obj : objects) {
      obj->shape.setFillColor(sf::Color(220, 220, 220));  // Paneles claros en gris claro

      if (mouseBoundary.intersects(obj->item.bound)) {
        obj->shape.setFillColor(sf::Color(200, 200, 200));  // Paneles en colisión en gris más claro
        mouseCollisions.push_back(obj);
        ++collisions;
      }
      for (auto&& otherObj : objects)
      ++bfCollisionChecks;
      for (auto&& found : map.getObjectsInBound_unchecked(obj->item.bound)) {
        ++qtCollisionChecks;
        if (&obj->item != found && found->bound.intersects(obj->item.bound)) {
          ++collisions;
          obj->shape.setFillColor(sf::Color(200, 200, 200));  // Resaltar los paneles en colisión con gris claro
        }
      }
      if (!freezeObjects) {
        obj->move();
        map.update(&obj->item);
      }
      window.draw(obj->shape);
    }

        // Actualizar el cuadro del mouse
        mouseBoundary.x = sf::Mouse::getPosition(window).x;
        mouseBoundary.y = sf::Mouse::getPosition(window).y;
        mouseBox.setSize(sf::Vector2f((float)mouseBoundary.width, (float)mouseBoundary.height));
        mouseBox.setPosition((float)mouseBoundary.x, (float)mouseBoundary.y);

        // Clic izquierdo = agregar objetos
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && MAP_BOUNDS.contains(mouseBoundary)) {
            if (clock.getElapsedTime().asSeconds() >= 0.1f) {
                objects.push_back(new Object(mouseBoundary.getRight(), mouseBoundary.getTop(), 5, 5));
                map.insert(&objects.back()->item);
                clock.restart();
            }
        }

        // Clic derecho = eliminar objetos dentro del cuadro del mouse
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            for (auto&& obj : mouseCollisions) {
                objects.erase(std::find(objects.begin(), objects.end(), obj));
                map.remove(&obj->item);
                delete obj;
            }
        }

        // Mostrar información de depuración del quadtree
        std::stringstream ss;
        ss << "Hijos Totales: "                 << map.totalChildren()
           << "\nObjetos Totales: "                << map.totalObjects()
           << "\nColisiones Totales: "             << collisions
           << "\nCollision Checks QuadTree: "    << qtCollisionChecks
           << "\nCollision Checks Fuerza Bruta: " << bfCollisionChecks
           << "\nColisiones con el Mouse: "        << mouseCollisions.size()
           << "\nObjetos en este quad: "         << map.getLeaf(mouseBoundary)->totalObjects();
        info.setString(ss.str());
        textBox.setSize(sf::Vector2f(info.getLocalBounds().width + 20, info.getLocalBounds().height + 20));  // Espacio adicional alrededor del texto
        window.draw(textBox);
        window.draw(info);
        window.draw(mouseBox);
        window.display();
    }

    // Limpieza
    map.clear();
    for (auto&& obj : objects)
        delete obj;
    objects.clear();
}

