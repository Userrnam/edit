#pragma once

#include "UIElement.hpp"
#include "Container.hpp"

struct ViewDescription {
    UIBaseElement* root;
};

struct PositionedElement {
    UIElement* element;
    sf::Vector2f position;
};

struct ViewLayout {
    std::vector<PositionedElement> elements;
};

struct View {
    ViewLayout layout;
    ViewDescription desc;

    View(ViewDescription description);

    void draw(sf::RenderWindow* window);
};
