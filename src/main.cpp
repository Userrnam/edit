#include <SFML/Graphics.hpp>
#include <iostream>

#include "UI.hpp"
#include "Rect.hpp"
#include "Stack.hpp"


int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Window");

    ViewDescription desc;
    desc.pos = {5, 5};

    desc.root = new VStack ({
        new HStack({
            new VStack({
                new Rectangle(sf::Color::Red),
            }),
            new VStack({
                new Rectangle(sf::Color::Black),
                new Rectangle(sf::Color::Cyan),
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

            else if (event.type == sf::Event::MouseButtonPressed) {
                MouseEvent me;

                me.action = sf::Event::MouseButtonPressed;
                me.pos = { (float)event.mouseButton.x, (float)event.mouseButton.y };

                view.update(me);
            } else if (event.type == sf::Event::MouseButtonReleased) {
                MouseEvent me;

                me.action = sf::Event::MouseButtonReleased;
                me.pos = { (float)event.mouseButton.x, (float)event.mouseButton.y };

                view.update(me);
            } else if (event.type == sf::Event::MouseMoved) {
                MouseEvent me;

                me.action = MOUSE_MOVE;
                me.pos = { (float)event.mouseMove.x, (float)event.mouseMove.y };

                view.update(me);
            }
        }

        window.clear(sf::Color::White);

        view.draw(&window);

        window.display();
    }

    return 0;
}
