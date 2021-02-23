#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>


typedef std::string String;

struct MyString {
    String s;
    int pos = 0;
};

String readNextWord(MyString& source);
String readPreviousWord(MyString& source);

struct Buffer {
    std::vector<String> lines;

    sf::Vector2i cursorPos = {};

    String getLines(int start, int end, int* relativeCursorPos);

    void loadText(const String& text);
    bool isEmpty() { return lines.empty(); }
    void addChar(char c);
    bool onLastLine();

    void clampX();
};

void buffer_eraseChar(Buffer* buffer);
void buffer_removeline(Buffer* buffer);
void buffer_moveLeft(Buffer* buffer);
void buffer_moveRight(Buffer* buffer);
void buffer_moveUp(Buffer* buffer);
void buffer_moveDown(Buffer* buffer);
void buffer_moveToEndOfLine(Buffer* buffer);
void buffer_moveToBeginningOfLine(Buffer* buffer);
void buffer_moveWordForward(Buffer* buffer);
void buffer_moveWordBackword(Buffer* buffer);
void buffer_moveToEndOfFile(Buffer* buffer);
void buffer_moveToBeginningOfFile(Buffer* buffer);
