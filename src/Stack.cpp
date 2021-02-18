#include "Stack.hpp"


Stack::Stack(std::initializer_list<UIBaseElement*> list, StackType t) {
    stackType = t;

    elements.reserve(list.size());

    for (auto e : list) {
        elements.push_back(e);
    }
}

Stack::~Stack() {
    for (auto e : elements) {
        delete e;
    }
}

sf::Vector2f Stack::fillLayout(ViewLayout* layout, sf::Vector2f pos) {
    sf::Vector2f border = {};

    int index = (int)stackType;

    for (auto element : elements) {
        if (element->elementType == UIElementType::ELEMENT) {
            PositionedElement pe;

            pe.element = (UIElement*)(element);

            pe.fr.left = pos.x;
            pe.fr.top  = pos.y;
            pe.fr.width  = pe.element->description.size.x;
            pe.fr.height = pe.element->description.size.y;

            layout->elements.push_back(pe);

            float *dst = (float*)(&pos);
            float *src = (float*)(&pe.element->description.size);

            dst[index] += (src[index] + element->padding);

            float *pBorder = (float*)(&border);

            if (pBorder[1-index] < src[1-index]) {
                pBorder[1-index] = src[1-index];
            }

        } else {
            auto container = (Container*)(element);

            auto size = container->fillLayout(layout, pos);

            float *dst = (float*)(&pos);
            float *src = (float*)(&size);

            dst[index] += (src[index] + element->padding);

            float *pBorder = (float*)(&border);

            if (pBorder[1-index] < src[1-index]) {
                pBorder[1-index] = src[1-index];
            }
        }
    }

    float *pBorder = (float*)(&border);
    float *pPos = (float*)(&pos);

    pBorder[index] = pPos[index];

    return border;
}
