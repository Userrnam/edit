#pragma once

#include "UIElement.hpp"

struct Rectangle : public UIElement {
    sf::RectangleShape r;
    sf::Color c;

    Rectangle(sf::Color color) {
        c = color;

        description.size = {200, 50};

        r.setSize(description.size);
        r.setFillColor(color);
    }

    virtual void draw(sf::RenderWindow* window, sf::Vector2f pos) override {
        r.setPosition(pos);
        window->draw(r);
    }

    virtual void event(MouseEvent event) override {
        if (event.action == MOUSE_ENTER) {
            r.setFillColor(sf::Color::Black);
        } else if (event.action == MOUSE_LEAVE) {
            r.setFillColor(c);
        }
    }
};
