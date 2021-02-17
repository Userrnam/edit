#include "Stack.hpp"


Stack::Stack(std::initializer_list<UIBaseElement*> list, StackType t) {
    stackType = t;

    elements.reserve(list.size());

    for (auto e : list) {
        elements.push_back(e);
    }
}

sf::Vector2f Stack::fillLayout(ViewLayout* layout, sf::Vector2f pos) {
    sf::Vector2f border = {};

    int index = (int)stackType;

    for (auto element : elements) {
        if (element->elementType == UIElementType::ELEMENT) {
            PositionedElement pe;

            pe.element = static_cast<UIElement*>(element);

            pe.fr.left = pos.x;
            pe.fr.top  = pos.y;
            pe.fr.width  = pe.element->description.size.x;
            pe.fr.height = pe.element->description.size.y;

            layout->elements.push_back(pe);

            float *dst = reinterpret_cast<float*>(&pos);
            float *src = reinterpret_cast<float*>(&pe.element->description.size);

            dst[index] += (src[index] + element->padding);

            float *pBorder = reinterpret_cast<float*>(&border);

            if (pBorder[1-index] < src[1-index]) {
                pBorder[1-index] = src[1-index];
            }

        } else {
            auto container = static_cast<Container*>(element);

            auto size = container->fillLayout(layout, pos);

            float *dst = reinterpret_cast<float*>(&pos);
            float *src = reinterpret_cast<float*>(&size);

            dst[index] += (src[index] + element->padding);

            float *pBorder = reinterpret_cast<float*>(&border);

            if (pBorder[1-index] < src[1-index]) {
                pBorder[1-index] = src[1-index];
            }
        }
    }

    float *pBorder = reinterpret_cast<float*>(&border);
    float *pPos = reinterpret_cast<float*>(&pos);

    pBorder[index] = pPos[index];

    return border;
}
