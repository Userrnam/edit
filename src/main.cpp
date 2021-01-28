#include <iostream>

#include <SFML/Graphics.hpp>

#include "Grid.hpp"


int main() {

    sf::RenderWindow window(sf::VideoMode(800, 600), "Window");

    Grid grid;

    float spacing = 50;

    grid.resize(window, spacing);

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));

                grid.resize(window, spacing);
            }
        }

        window.clear(sf::Color::White);

        window.draw(grid);

        window.display();
    }

    return 0;
}
