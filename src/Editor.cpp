#include "Editor.hpp"


void Editor::update() {
    auto pos = sf::Mouse::getPosition(m_window);

    m_activeTile.setPosition(getTilePos(pos));
}

sf::Vector2f Editor::getTilePos(sf::Vector2i mousePos) {
    int x = mousePos.x - mousePos.x % spacing;
    int y = mousePos.y - mousePos.y % spacing;

    return sf::Vector2f(x, y);
}
