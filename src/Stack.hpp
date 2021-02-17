#pragma once

#include "Container.hpp"

enum class StackType {
    HORIZONTAL = 0,
    VERTICAL   = 1,
};

struct Stack : public Container {
    std::vector<UIBaseElement*> elements;
    StackType stackType = StackType::HORIZONTAL;

    Stack(std::initializer_list<UIBaseElement*> list, StackType t);
    virtual sf::Vector2f fillLayout(ViewLayout* layout, sf::Vector2f pos) override;
};

struct HStack : public Stack {
    HStack(std::initializer_list<UIBaseElement*> list = {}) : Stack(list, StackType::HORIZONTAL) {}
};

struct VStack : public Stack {
    VStack(std::initializer_list<UIBaseElement*> list = {}) : Stack(list, StackType::VERTICAL) {}
};
