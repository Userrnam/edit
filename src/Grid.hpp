#pragma once

#include <SFML/Graphics.hpp>

const int spacing = 50;

struct Grid : public sf::Drawable, public sf::Transformable {

    sf::VertexArray m_vertices;

    void resize(const sf::RenderWindow& window);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
