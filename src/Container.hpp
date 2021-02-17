#pragma once

#include <vector>

#include "UIElement.hpp"


struct Stack : public UIBaseElement {
    std::vector<UIBaseElement*> elements;

    Stack(std::initializer_list<UIBaseElement*> list, UIElementType t) : UIBaseElement(t) {
        elements.reserve(list.size());

        for (auto e : list) {
            elements.push_back(e);
        }
    }
};

struct HStack : public Stack {
    HStack(std::initializer_list<UIBaseElement*> list = {}) : Stack(list, UIElementType::HSTACK) {}
};

struct VStack : public Stack {
    VStack(std::initializer_list<UIBaseElement*> list = {}) : Stack(list, UIElementType::VSTACK) {}
};
