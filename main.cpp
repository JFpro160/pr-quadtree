#include <time.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <sstream>
#include "QuadTree.h"

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

    QuadTree map = QuadTree(MAP_BOUNDS, 2, 15); // bounds, capacity, maxLevel
    std::vector<Object*> objects;

    
    sf::Font font;
    if (!font.loadFromFile("Roboto-Regular.ttf")) {
        return -1; 
    }
    map.setFont(font);

    sf::Text info("", font);
    info.setCharacterSize(22);
    info.setFillColor(sf::Color::Black);
    info.setStyle(sf::Text::Bold); 
    info.setPosition(sf::Vector2f(10, 10));

    sf::RectangleShape textBox;
    textBox.setFillColor(sf::Color(255, 255, 255, 180));
    textBox.setOutlineThickness(2.0f);
    textBox.setOutlineColor(sf::Color::Black);

    sf::Event event;
    sf::RectangleShape mouseBox;
    mouseBox.setOutlineThickness(3.0f);
    mouseBox.setFillColor(sf::Color(0, 255, 255, 128)); 
    mouseBox.setOutlineColor(sf::Color::Cyan);

    bool freezeObjects = false;
    Rect mouseBoundary = { 0, 0, 20, 20 };

    sf::Clock clock;

    while (window.isOpen()) {

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
                mouseBoundary.width += 8;
                mouseBoundary.height += 8;
                break;
            case sf::Keyboard::Down:
                mouseBoundary.width -= 8;
                mouseBoundary.height -= 8;
                break;
            }
        }

        window.clear();  

        map.draw(window);


        std::vector<Object*> mouseCollisions;

        for (auto&& obj : objects) {
          obj->shape.setFillColor(sf::Color(255, 255, 255));

          if (mouseBoundary.intersects(obj->item.bound)) {
            obj->shape.setFillColor(sf::Color(255, 99, 71));  
            mouseCollisions.push_back(obj);
          }
      
          if (!freezeObjects) {
            obj->move();
            map.update(&obj->item);
          }
          window.draw(obj->shape);
        }

        mouseBoundary.x = sf::Mouse::getPosition(window).x;
        mouseBoundary.y = sf::Mouse::getPosition(window).y;
        mouseBox.setSize(sf::Vector2f((float)mouseBoundary.width, (float)mouseBoundary.height));
        mouseBox.setPosition((float)mouseBoundary.x, (float)mouseBoundary.y);
        
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && MAP_BOUNDS.contains(mouseBoundary)) {
            if (clock.getElapsedTime().asSeconds() >= 0.1f) {
                objects.push_back(new Object(mouseBoundary.getRight(), mouseBoundary.getTop(), 5, 5));
                map.insert(&objects.back()->item);
                clock.restart();
            }
        }
        
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            for (auto&& obj : mouseCollisions) {
                objects.erase(std::find(objects.begin(), objects.end(), obj));
                map.remove(&obj->item);
                delete obj;
            }
        }

        std::stringstream ss;
        ss << "Cuadrantes: "                 << map.totalChildren()
           << "\nPuntos Totales: "                << map.totalObjects()
           << "\nPuntos Aqui: "         << map.getLeaf(mouseBoundary)->totalObjects();
        info.setString(ss.str());
        textBox.setSize(sf::Vector2f(info.getLocalBounds().width + 20, info.getLocalBounds().height + 20));  // Espacio adicional alrededor del texto
        window.draw(textBox);
        window.draw(info);
        window.draw(mouseBox);
        window.display();
    }

    
    map.clear();
    for (auto&& obj : objects)
        delete obj;
    objects.clear();
}

