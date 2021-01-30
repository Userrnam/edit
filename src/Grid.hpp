#pragma once

#include <SFML/Graphics.hpp>

const int spacing = 50;

struct Grid : public sf::Drawable, public sf::Transformable {

    sf::VertexArray m_vertices;

    void resize(sf::Vector2u size);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
