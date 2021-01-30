#pragma once

#include "Grid.hpp"


struct Editor {
    sf::RenderWindow& m_window;
    sf::View m_view;
    sf::View m_staticView;

    Grid m_grid;
    sf::RectangleShape m_activeTile;

    Editor(sf::RenderWindow& window);

    void update();

    sf::Vector2f getTilePos(sf::Vector2i mousePos);

    void move(sf::Vector2f v);

    void handleResize(sf::Event::SizeEvent e);

    void draw();
};
