#pragma once

#include <vector>

#include "UIElement.hpp"


struct PositionedElement {
    UIElement* element;
    sf::FloatRect fr;
};

struct ViewLayout {
    std::vector<PositionedElement> elements;
    PositionedElement* pSelectedElement = nullptr;
};

struct Container : public UIBaseElement {
    Container() : UIBaseElement(UIElementType::CONTAINER) {}

    virtual sf::Vector2f fillLayout(ViewLayout* layout, sf::Vector2f pos) = 0;
};
