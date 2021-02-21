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
    void moveUp();
    void moveDown();

    int getCurrentPositionInLine();

    void moveToEndOfLine();
    void moveToBegginingOfLine();
};

void buffer_eraseChar(Buffer* buffer);
void buffer_removeline(Buffer* buffer);
void buffer_moveLeft(Buffer* buffer);
void buffer_moveRight(Buffer* buffer);
void buffer_moveUp(Buffer* buffer);
void buffer_moveDown(Buffer* buffer);
void buffer_moveToEndOfLine(Buffer* buffer);
void buffer_moveToBegginingOfLine(Buffer* buffer);
void buffer_removeWordForward(Buffer* buffer);
void buffer_removeWordBackword(Buffer* buffer);
