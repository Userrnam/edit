#include "Grid.hpp"


void Grid::resize(const sf::RenderWindow& window) {
    auto size = window.getSize();

    int xCount = size.x / spacing + 1;
    int yCount = size.y / spacing + 1;

    m_vertices.resize((xCount + yCount) * 2);
    m_vertices.setPrimitiveType(sf::Lines);

    for (int x = 0; x < xCount; ++x) {
        m_vertices[2*x].position.x = x * spacing;
        m_vertices[2*x].position.y = 0;

        m_vertices[2*x+1].position.x = x * spacing;
        m_vertices[2*x+1].position.y = size.y;

        m_vertices[2*x  ].color = sf::Color::Black;
        m_vertices[2*x+1].color = sf::Color::Black;
    }

    for (int y = 0; y < yCount; ++y) {
        m_vertices[2*y + 2*xCount].position.y = y * spacing;
        m_vertices[2*y + 2*xCount].position.x = 0;

        m_vertices[2*y+1 + 2*xCount].position.y = y * spacing;
        m_vertices[2*y+1 + 2*xCount].position.x = size.x;

        m_vertices[2*y   + 2*xCount].color   = sf::Color::Black;
        m_vertices[2*y+1 + 2*xCount].color = sf::Color::Black;
    }
}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    target.draw(m_vertices, states);
}
