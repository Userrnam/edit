#include "UI.hpp"
#include "Container.hpp"


sf::Vector2f fillLayout(ViewLayout *layout, UIBaseElement *root, sf::Vector2f pos = {}) {
    sf::Vector2f border = {};

    int index = -1;

    if (root->elementType == UIElementType::HSTACK) {
        index = 0;
    } else if (root->elementType == UIElementType::VSTACK) {
        index = 1;
    }

    if (index != -1) {
        Stack* stack = static_cast<Stack*>(root);

        for (auto element : stack->elements) {
            if (element->elementType == UIElementType::ELEMENT) {
                PositionedElement pe;

                pe.position = pos;
                pe.element = static_cast<UIElement*>(element);

                layout->elements.push_back(pe);

                float *dst = reinterpret_cast<float*>(&pos);
                float *src = reinterpret_cast<float*>(&pe.element->description.size);

                dst[index] += (src[index] + element->padding);

                float *pBorder = reinterpret_cast<float*>(&border);

                if (pBorder[1-index] < src[1-index]) {
                    pBorder[1-index] = src[1-index];
                }

            } else {
                auto size = fillLayout(layout, element, pos);

                float *dst = reinterpret_cast<float*>(&pos);
                float *src = reinterpret_cast<float*>(&size);

                dst[index] += (src[index] + element->padding);

                float *pBorder = reinterpret_cast<float*>(&border);

                if (pBorder[1-index] < src[1-index]) {
                    pBorder[1-index] = src[1-index];
                }
            }
        }
    } 

    float *pBorder = reinterpret_cast<float*>(&border);
    float *pPos = reinterpret_cast<float*>(&pos);

    pBorder[index] = pPos[index];

    return border;
}

View::View(ViewDescription description) {
    desc = description;

    fillLayout(&layout, description.root, description.pos);
}

void View::draw(sf::RenderWindow *window) {
    for (auto element : layout.elements) {
        element.element->draw(window, element.position);
    }
}
