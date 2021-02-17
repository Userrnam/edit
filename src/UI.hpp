#pragma once

#include "UIElement.hpp"
#include "Container.hpp"

struct ViewDescription {
    UIBaseElement* root;
    sf::Vector2f pos;
};

struct PositionedElement {
    UIElement* element;
    sf::FloatRect fr;
};

struct ViewLayout {
    std::vector<PositionedElement> elements;
    PositionedElement* pSelectedElement = nullptr;
};

struct View {
    ViewLayout layout;
    ViewDescription desc;

    View(ViewDescription description);

    void draw(sf::RenderWindow* window);
    void update(MouseEvent event);
};
