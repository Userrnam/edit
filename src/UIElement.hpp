#pragma once

#include <SFML/Graphics.hpp>


enum class UIElementType {
    ELEMENT,
    VSTACK,
    HSTACK
};

struct ElementDescription {
    sf::Vector2f size;
};

struct UIBaseElement {
    UIElementType elementType;
    float padding = 5;

    UIBaseElement(UIElementType t) {
        elementType = t;
    }
};

struct UIElement : public UIBaseElement {
    ElementDescription description;

    UIElement() : UIBaseElement(UIElementType::ELEMENT) {}

    virtual void draw(sf::RenderWindow* window, sf::Vector2f position) = 0;
};
