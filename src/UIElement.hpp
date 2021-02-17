#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#define MOUSE_ENTER -1
#define MOUSE_MOVE  -2
#define MOUSE_LEAVE -3

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

struct MouseEvent {
    sf::Vector2f pos;
    int action = 0;
};

struct UIElement : public UIBaseElement {
    ElementDescription description;

    UIElement() : UIBaseElement(UIElementType::ELEMENT) {}

    virtual void draw(sf::RenderWindow* window, sf::Vector2f position) = 0;
    virtual void event(MouseEvent event) {}
};
