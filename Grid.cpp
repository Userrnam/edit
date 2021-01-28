#include "Grid.hpp"


void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

    // states.texture = &m_texture;

    // draw the vertex array
    // target.draw(m_vertices, states);
}