#include <SFML/Graphics.hpp>
#include <vector>

struct Grid : public sf::Drawable, public sf::Transformable {

    sf::VertexArray m_vertices;

    void resize(const sf::RenderWindow& window, float spacing);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
