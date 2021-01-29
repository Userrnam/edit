#pragma once

#include "Grid.hpp"


struct Editor {
    sf::RenderWindow& m_window;

    sf::RectangleShape m_activeTile;

    Editor(sf::RenderWindow& window) : m_window(window) {
        m_activeTile.setSize(sf::Vector2f(spacing, spacing));

        m_activeTile.setFillColor(sf::Color(5, 200, 5, 140));

        m_activeTile.setPosition(getTilePos(sf::Mouse::getPosition(m_window)));
    }

    void update();

    sf::Vector2f getTilePos(sf::Vector2i mousePos);
};
