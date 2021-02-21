#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Buffer.hpp"


struct EditInfo {
    char c = -1;
    sf::Event::KeyEvent event;
};

sf::Color getColorForWord(const sf::String& word) {
    static const std::vector<sf::String> keywords = {
        "if", "while", "do", "goto", "void", "int", "float", "double", "char"
    };

    if (std::find(keywords.begin(), keywords.end(), word) != keywords.end()) {
        return sf::Color::Red;
    }

    return sf::Color::Black;
}

struct Editor {
    Buffer buffer;

    sf::Text text;
    sf::RectangleShape cursor;
    sf::Vector2f charSize;
    sf::Font font;

    void draw(sf::RenderWindow& window) {
        window.draw(cursor);

        sf::Text t;
        t.setFont(font);
        t.setCharacterSize(text.getCharacterSize());

        MyString ms;
        ms.s = text.getString();

        int index = 0;
        while (1) {
            auto word = readNextWord(ms);

            if (word == "") {
                buffer.s.pos = 0;
                break;
            }

            auto pos = text.findCharacterPos(index);

            if (pos.y > window.getSize().y) {
                buffer.s.pos = 0;
                break;
            }

            t.setPosition(pos);
            t.setString(word);
            t.setFillColor(getColorForWord(word));

            index += word.size();

            window.draw(t);
        }
    }

    void init() {
        font.loadFromFile("Hack-Regular.ttf");

        text.setFont(font);
        text.setFillColor(sf::Color::Black);

        text.setString("H\n");
        charSize = text.getGlobalBounds().getSize();
        charSize.y = text.getCharacterSize();
        text.setString("");

        cursor.setSize(charSize);
        cursor.setFillColor(sf::Color::Green);
    }

    void update(EditInfo info) {
        if (info.c != -1) {
            if (info.event.control) {
                if (info.c == 'h') {
                    buffer.moveLeft();
                } else if (info.c == 'l') {
                    buffer.moveRight();
                }
            } else {
                buffer.addChar(info.c);
            }
        } else {
            if (info.event.code == sf::Keyboard::BackSpace && !buffer.isEmpty()) {
                if (info.event.system) {
                    buffer.removeline();
                } else {
                    buffer.eraseChar();
                    std::cout << "Buffer size is " << buffer.s.s.size() << std::endl;
                }
            } else if (info.event.code == sf::Keyboard::Left) {
                buffer.moveLeft();
            } else if (info.event.code == sf::Keyboard::Right) {
                buffer.moveRight();
            }
        }

        auto s = buffer.getLines(0, 30);

        text.setString(s);

        if (buffer.isEmpty()) {
            cursor.setPosition(sf::Vector2f(0, 0));
        } else {
            auto pos = text.findCharacterPos(buffer.cursorPos);

            cursor.setPosition(pos);
        }
    }
};