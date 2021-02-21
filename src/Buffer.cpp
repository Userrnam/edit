#include "Buffer.hpp"


std::vector<char> separators = {
    ' ', '\t', '\n', ',', '.',
    '(', ')', '[', ']', '{', '}',
    ':', ';',
    '=', '-', '+', '*', '/'
};

String readNextWord(MyString& source) {
    String word = "";

    while (source.pos < source.s.size()) {
        char c = source.s[source.pos];

        if (std::find(separators.begin(), separators.end(), c) != separators.end()) {
            if (word.empty()) {
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

String readPreviousWord(MyString& source) {
    String word = "";

    while (source.pos >= 0) {
        char c = source.s[source.pos];

        if (std::find(separators.begin(), separators.end(), c) != separators.end()) {
            if (word.empty()) {
                word = c;
                source.pos--;
            }
            return word;
        }

        word += c;
        source.pos--;
    }

    return word;
}

void fullPrint(const String& s) {
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

String Buffer::getLines(int start, int end) {
    int count = 0;

    if (s.s.empty()) {
        return "";
    }

    char* pStart = &s.s[0];
    char* pEnd   = nullptr;

    for (int i = 0; i < s.s.size(); ++i) {
        if (s.s[i] == '\n') {
            count++;

            if (count == start) {
                pStart = &s.s[i];
            } else if (count == end) {
                pEnd = &s.s[i];
                break;
            }
        }
    }

    if (pEnd == nullptr) {
        pEnd = &s.s.back();
    }

    return String(pStart, pEnd);
}

void Buffer::addChar(char c) {
    s.s.insert(s.s.begin()+cursorPos, c);
    cursorPos++;
}

void Buffer::eraseChar() {
    if (cursorPos == 0)  return;

    cursorPos--;
    s.s.erase(cursorPos, 1);
}

void Buffer::removeline() {
    int p = cursorPos;

    if (p == 0)  return;

    p--;

    while (p > 0 && s.s[p] != '\n') {
        p--;
    }

    if (p != 0)  p++;

    s.s.erase(s.s.begin()+p, s.s.begin()+cursorPos);

    cursorPos = p;
}

void Buffer::moveLeft() {
    if (cursorPos > 0)
    cursorPos--;
}

void Buffer::moveRight() {
    if (cursorPos < s.s.size()-1)
    cursorPos++;
}

void Buffer::moveUp() {
    int linePos = getCurrentPositionInLine();

    if (linePos == cursorPos)  return;

    moveToBegginingOfLine();

    cursorPos--;
    moveToBegginingOfLine();

    for (int i = 0; i < linePos && s.s[cursorPos] != '\n'; ++i) {
        cursorPos++;
    }
}

void Buffer::moveDown() {
    int linePos = getCurrentPositionInLine();

    moveToEndOfLine();

    if (cursorPos == s.s.size()-1)  return;

    cursorPos++;

    for (int i = 0; i < linePos && s.s[cursorPos] != '\n' && cursorPos < s.s.size()-1; ++i) {
        cursorPos++;
    }
}

int Buffer::getCurrentPositionInLine() {
    int pos = cursorPos;
    int count = 0;

    while (1) {
        if (pos-count == 0 || s.s[pos-count-1] == '\n') {
            return count;
        }

        count++;
    }
}

void Buffer::moveToEndOfLine() {
    while (1) {
        if (cursorPos == s.s.size()-1 || s.s[cursorPos] == '\n') {
            return;
        }

        cursorPos++;
    }
}

void Buffer::moveToBegginingOfLine() {
    cursorPos -= getCurrentPositionInLine();
    if (cursorPos < 0) {
        cursorPos = 0;
    }
}

void buffer_eraseChar(Buffer* buffer) {
    buffer->eraseChar();
}

void buffer_removeline(Buffer* buffer) {
    buffer->removeline();
}

void buffer_moveLeft(Buffer* buffer) {
    buffer->moveLeft();
}

void buffer_moveRight(Buffer* buffer) {
    buffer->moveRight();
}

void buffer_moveUp(Buffer* buffer) {
    buffer->moveUp();
}

void buffer_moveDown(Buffer* buffer) {
    buffer->moveDown();
}

void buffer_moveToEndOfLine(Buffer* buffer) {
    buffer->moveToEndOfLine();
}

void buffer_moveToBegginingOfLine(Buffer* buffer) {
    buffer->moveToBegginingOfLine();
}

void buffer_removeWordForward(Buffer* buffer) {
    if (buffer->cursorPos == buffer->s.s.size()-1)  return;

    MyString ms;
    ms.pos = buffer->cursorPos;
    ms.s = buffer->s.s;

    auto w = readNextWord(ms);

    buffer->s.s.erase(buffer->s.s.begin() + buffer->cursorPos, buffer->s.s.begin() + buffer->cursorPos + w.size());
}

void buffer_removeWordBackword(Buffer* buffer) {
    if (buffer->cursorPos == 0)  return;

    MyString ms;
    ms.pos = buffer->cursorPos - 1;
    ms.s = buffer->s.s;

    auto w = readPreviousWord(ms);

    buffer->s.s.erase(buffer->s.s.begin() + buffer->cursorPos - w.size(), buffer->s.s.begin() + buffer->cursorPos);
}
