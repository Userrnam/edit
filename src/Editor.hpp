#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

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

    int topLine = 0;
    float scrollValue = 0;
    int firstVisibleLine = 0;
    int lastVisibleLine = 5;

    void draw(sf::RenderWindow& window);

    void init();

    void update(EditInfo info);
    void scroll(float delta);

    void updateDrawInfo(const sf::RenderWindow& window);
};
