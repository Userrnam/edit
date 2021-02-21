#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>


typedef std::string String;

struct MyString {
    String s;
    int pos = 0;
};

String readNextWord(MyString& source);

struct Buffer {
    MyString s;
    int cursorPos = 0;

    String getLines(int start, int end);

    bool isEmpty() { return s.s.empty(); }

    void addChar(char c);
    void eraseChar();
    void removeline();

    void moveLeft();
    void moveRight();
};
