#include <SFML/Graphics.hpp>
#include <iostream>

#include "Grid.hpp"
#include "Editor.hpp"

sf::Vector2u previousWindowSize;

int main() {

    sf::RenderWindow window(sf::VideoMode(800, 600), "Window");

    previousWindowSize = window.getSize();

    Editor editor(window);

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::Resized) {
                editor.handleResize(event.size);

                previousWindowSize = window.getSize();
                
            } else if (event.type == sf::Event::MouseMoved) {
                editor.update();
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Equal) {
                    editor.m_view.zoom(0.9);
                } else if (event.key.code == sf::Keyboard::G) {
                    editor.m_view.zoom(1.1);
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))  editor.move(sf::Vector2f(-1.0f, 0));
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))  editor.move(sf::Vector2f(1.0f, 0));

        window.clear(sf::Color::White);

        editor.draw();

        window.display();
    }

    return 0;
}
