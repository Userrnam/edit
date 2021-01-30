#pragma once

#include <SFML/Graphics.hpp>


struct UIView {
    sf::RenderWindow& m_window;
    sf::View m_view;

    UIView(sf::RenderWindow& window);
};
