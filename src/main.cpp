#include <SFML/Graphics.hpp>
#include <iostream>

#include "UI.hpp"
#include "Rect.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Window");

    ViewDescription desc;

    desc.root = new VStack ({
        new HStack({
            new VStack({
                new Rectangle(sf::Color::Red),
                new Rectangle(sf::Color::Blue),
                new Rectangle(sf::Color::Green),
            }),
            new VStack({
                new Rectangle(sf::Color::Black),
                new Rectangle(sf::Color::Yellow),
                new Rectangle(sf::Color::Red),
                new Rectangle(sf::Color::Blue),
                new Rectangle(sf::Color::Green),
            }),
        }),
        new Rectangle(sf::Color::Magenta)
    });

    View view(desc);

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color::White);

        view.draw(&window);

        window.display();
    }

    return 0;
}
