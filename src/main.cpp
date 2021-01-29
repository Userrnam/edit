#include <SFML/Graphics.hpp>

#include "Grid.hpp"
#include "Editor.hpp"

int main() {

    sf::RenderWindow window(sf::VideoMode(800, 600), "Window");

    Grid grid;
    Editor editor(window);

    grid.resize(window);

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));

                grid.resize(window);
            } else if (event.type == sf::Event::MouseMoved) {
                editor.update();
            }
        }

        window.clear(sf::Color::White);

        window.draw(grid);
        window.draw(editor.m_activeTile);

        window.display();
    }

    return 0;
}
