#include <SFML/Graphics.hpp>


struct Grid : public sf::Drawable, public sf::Transformable {

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
