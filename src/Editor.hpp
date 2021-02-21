#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

struct EditInfo {
    char c = -1;
    sf::Event::KeyEvent event;
};

struct MyString {
    sf::String s;
    int pos = 0;
};

std::vector<char> separators = {
    ' ', '\t', '\n', ',', '.',
    '(', ')', '[', ']', '{', '}',
    ':', ';',
    '=', '-', '+', '*', '/'
};

sf::String readNextWord(MyString& source) {
    sf::String word = "";

    while (source.pos < source.s.getSize()) {
        char c = source.s[source.pos];

        if (std::find(separators.begin(), separators.end(), c) != separators.end()) {
            if (word.isEmpty()) {
                word = c;
                source.pos++;
            }
            return word;
        }

        word += c;
        source.pos++;
    }

    return word;
}

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
    sf::Text text;
    MyString s;
    sf::RectangleShape cursor;
    sf::Vector2f charSize;
    sf::Font font;
    int cursorPos = 0;

    void draw(sf::RenderWindow& window) {
        window.draw(cursor);
        // window.draw(text);

        sf::Text t;
        t.setFont(font);
        t.setCharacterSize(text.getCharacterSize());

        int index = 0;
        while (1) {
            auto word = readNextWord(s);

            if (word == "") {
                s.pos = 0;
                break;
            }

            auto pos = text.findCharacterPos(index);

            if (pos.y > window.getSize().y) {
                s.pos = 0;
                break;
            }

            t.setPosition(pos);
            t.setString(word);
            t.setFillColor(getColorForWord(word));

            index += word.getSize();

            window.draw(t);
        }
    }

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
        s.s.insert(cursorPos, c);
        cursorPos++;
    }

    void moveLeft() {
        if (cursorPos > 0)
        cursorPos--;
    }

    void moveRight() {
        if (cursorPos < s.s.getSize())
        cursorPos++;
    }

    void eraseChar() {
        if (s.s.isEmpty())  return;

        cursorPos--;
        s.s.erase(cursorPos);
    }

    void update(EditInfo info) {
        if (info.c != -1) {
            addChar(info.c);
        } else {
            if (info.event.code == sf::Keyboard::BackSpace && s.s.getSize()) {
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

        text.setString(s.s);

        if (s.s.isEmpty()) {
            cursor.setPosition(sf::Vector2f(0, 0));
        } else {
            auto pos = text.findCharacterPos(cursorPos);

            cursor.setPosition(pos);
        }
    }

    void removeline() {
        int p = cursorPos;

        while (p > 0 && s.s[p] != '\n') {
            p--;
        }

        s.s.erase(p, cursorPos-p);
        cursorPos = p;
    }
};