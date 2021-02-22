#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>

#include "Buffer.hpp"


struct EditInfo {
    char c = -1;
    sf::Event::KeyEvent event;
};

struct Editor {
    Buffer buffer;

    sf::Text text;
    sf::RectangleShape cursor;
    sf::Vector2f charSize;
    sf::Font font;

    void draw(sf::RenderWindow& window);

    void init();

    void update(EditInfo info);
    void updateDrawInfo(const sf::RenderWindow& window);
};