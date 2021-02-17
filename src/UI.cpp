#include "UI.hpp"
#include "Container.hpp"

View::View(ViewDescription description) {
    desc = description;

    desc.root->fillLayout(&layout, description.pos);
}

void View::draw(sf::RenderWindow *window) {
    for (auto element : layout.elements) {
        element.element->draw(window, { element.fr.left, element.fr.top });
    }
}

void View::update(MouseEvent event) {
    if (layout.pSelectedElement != nullptr) {
        if (layout.pSelectedElement->fr.contains(event.pos)) {
            layout.pSelectedElement->element->event(event);
            return;
        } else {
            auto e = event;
            e.action = MOUSE_LEAVE;
            layout.pSelectedElement->element->event(e);
            layout.pSelectedElement = nullptr;
        }
    }

    for (auto& element : layout.elements) {
        if (element.fr.contains(event.pos)) {
            event.action = MOUSE_ENTER;
            element.element->event(event);
            layout.pSelectedElement = &element;
            return;
        }
    }
}
