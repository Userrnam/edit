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

    while (p > 0 && s.s[p] != '\n') {
        p--;
    }

    s.s.erase(p, cursorPos-p);
    cursorPos = p;
}

void Buffer::moveLeft() {
    if (cursorPos > 0)
    cursorPos--;
}

void Buffer::moveRight() {
    if (cursorPos < s.s.size())
    cursorPos++;
}







