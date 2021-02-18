#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

struct EditInfo {
    char c = -1;
    sf::Event::KeyEvent event;
};

void fullPrint(sf::String& s) {
    for (char c : s) {
        if (c == '\n') {
            std::cout << "\\n";
        } else if (c == '\t') {
            std::cout << "\\t";
        } else {
            std::cout << c;
        }
    }

    std::cout << std::endl;
}

struct Editor {
    sf::Text text;
    sf::String s;
    sf::RectangleShape cursor;
    sf::Vector2f charSize;
    sf::Font font;
    int cursorPos = 0;

    void init() {
        font.loadFromFile("font.otf");

        text.setFont(font);
        text.setFillColor(sf::Color::Black);

        text.setString("H\n");
        charSize = text.getGlobalBounds().getSize();
        charSize.y = text.getCharacterSize();
        text.setString("");

        cursor.setSize(charSize);
        cursor.setFillColor(sf::Color::Green);
    }

    void addChar(char c) {
        s.insert(cursorPos, c);
        cursorPos++;
    }

    void moveLeft() {
        if (cursorPos > 0)
        cursorPos--;
    }

    void moveRight() {
        if (cursorPos < s.getSize())
        cursorPos++;
    }

    void eraseChar() {
        if (s.isEmpty())  return;

        cursorPos--;
        s.erase(cursorPos);
    }

    void update(EditInfo info) {
        if (info.c != -1) {
            addChar(info.c);
        } else {
            if (info.event.code == sf::Keyboard::BackSpace && s.getSize()) {
                if (info.event.system) {
                    removeline();
                } else {
                    eraseChar();
                }
            } else if (info.event.code == sf::Keyboard::Left) {
                moveLeft();
            } else if (info.event.code == sf::Keyboard::Right) {
                moveRight();
            }
        }

        text.setString(s);

        if (s.isEmpty()) {
            cursor.setPosition(sf::Vector2f(0, 0));
        } else {
            auto pos = text.findCharacterPos(cursorPos);

            if (s[cursorPos] == '\n') {
                pos.x = 0;               
                pos.y += charSize.y + text.getLineSpacing();
            }

            cursor.setPosition(pos);

            fullPrint(s);
        }
    }

    void removeline() {
        int p = cursorPos;

        while (p > 0 && s[p] != '\n') {
            p--;
        }

        s.erase(p, cursorPos-p);
        cursorPos = p;
    }
};