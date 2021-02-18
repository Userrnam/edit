#pragma once

#include "UIElement.hpp"
#include "Container.hpp"

struct ViewDescription {
    Container* root;
    sf::Vector2f pos;
};

struct View {
    ViewLayout layout;
    ViewDescription desc;

    View(ViewDescription description);
    ~View();

    void draw(sf::RenderWindow* window);
    void update(MouseEvent event);
};
