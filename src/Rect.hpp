#pragma once

#include "UIElement.hpp"

struct Rectangle : public UIElement {
    sf::RectangleShape r;

    Rectangle(sf::Color color) {
        description.size = {200, 50};

        r.setSize(description.size);
        r.setFillColor(color);
    }

    virtual void draw(sf::RenderWindow* window, sf::Vector2f pos) override {
        r.setPosition(pos);
        window->draw(r);
    }
};
