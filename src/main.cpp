#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

#include "Editor.hpp"


bool isPrintable(sf::Keyboard::Key key) {
    if (key >= sf::Keyboard::Key::A && key <= sf::Keyboard::Key::Num9 || 
        key >= sf::Keyboard::Key::LBracket && key <= sf::Keyboard::Key::Space ||
        key == sf::Keyboard::Key::Tab || key == sf::Keyboard::Enter ||
        key >= sf::Keyboard::Key::Add && key <= sf::Keyboard::Key::Divide) {
        return true;
    }

    return false;
}

char _getChar(sf::Keyboard::Key key) {
    if (key >= sf::Keyboard::Key::A && key <= sf::Keyboard::Z) {
        return 'a' + key - sf::Keyboard::A;
    }

    if (key >= sf::Keyboard::Key::Num0 && key <= sf::Keyboard::Num9) {
        return '0' + key - sf::Keyboard::Num0;
    }

    switch (key) {
        case sf::Keyboard::LBracket:  return '[';
        case sf::Keyboard::RBracket:  return ']';
        case sf::Keyboard::SemiColon:  return ';';
        case sf::Keyboard::Comma:  return ',';
        case sf::Keyboard::Quote:  return '\'';
        case sf::Keyboard::Slash:  return '/';
        case sf::Keyboard::BackSlash:  return '\\';
        case sf::Keyboard::Tilde:  return '~';
        case sf::Keyboard::Equal:  return '=';
        case sf::Keyboard::Hyphen:  return '-';
        case sf::Keyboard::Space:  return ' ';
        case sf::Keyboard::Enter:  return '\n';
        case sf::Keyboard::Tab:  return '\t';
        case sf::Keyboard::Add:  return '+';
        case sf::Keyboard::Subtract:  return '-';
        case sf::Keyboard::Multiply:  return '*';
        case sf::Keyboard::Divide:  return '/';
        default: return '-';
    }
}

char getChar(sf::Keyboard::Key key, bool shift) {
    char c = _getChar(key);

    if (!shift) {
        return c;
    }

    if ('a' <= c && c <= 'z') {
        return c - ('a' - 'A');
    }

    switch (c) {
        case '1' : return '!';
        case '2' : return '@';
        case '3' : return '#';
        case '4' : return '$';
        case '5' : return '%';
        case '6' : return '^';
        case '7' : return '&';
        case '8' : return '*';
        case '9' : return '(';
        case '0' : return ')';
        case '[' : return '{';
        case ']' : return '}';
        case ';' : return ':';
        case '\'' : return '\"';
    }

    return '-';
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Window");

    Editor editor;

    std::fstream f;

    editor.init();

    f.open("src/Editor.hpp", std::ios::in);
    std::string str((std::istreambuf_iterator<char>(f)),
                    std::istreambuf_iterator<char>());

    editor.s.s = str;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                auto code = event.key.code;

                EditInfo ei;
                ei.event = event.key;

                if (isPrintable(code)) {
                    ei.c = getChar(code, event.key.shift);
                }

                editor.update(ei);
            }

            window.clear(sf::Color::White);

            editor.draw(window);

            window.display();
        }
    }
}
