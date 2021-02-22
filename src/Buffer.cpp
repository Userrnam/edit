#include "Buffer.hpp"


std::vector<char> separators = {
    ' ', '\t', '\n',
    '(', ')', '[', ']', '{', '}', '<', '>',
    ':', ';', ',', '.', '!', '?',
    '=', '-', '+', '*', '/', '|', '&', '%', '$', '#', '@', '^', '\\'
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

String Buffer::getLines(int start, int end, int* relativeCursorPos) {
    int count = 0;

    firstVisibleLine = start;
    lastVisibleLine  = end;

    if (s.s.empty()) {
        return "";
    }

    char* pStart = &s.s[0];
    char* pEnd   = nullptr;

    for (int i = 0; i < s.s.size(); ++i) {
        if (s.s[i] == '\n') {
            count++;

            if (count == start) {
                pStart = &s.s[i+1];
            } else if (count == end) {
                pEnd = &s.s[i];
                break;
            }
        }
    }

    if (pEnd == nullptr) {
        pEnd = &s.s.back();
    }

    *relativeCursorPos = cursorPos - (pStart - &s.s[0]);

    return String(pStart, pEnd + 1);
}

void Buffer::addChar(char c) {
    s.s.insert(s.s.begin()+cursorPos, c);
    cursorPos++;
}

int Buffer::getCurrentPositionInLine() {
    int count = 0;

    while (1) {
        if (cursorPos-count == 0 || s.s[cursorPos-count-1] == '\n') {
            return count;
        }

        count++;
    }
}

void buffer_eraseChar(Buffer* buffer) {
    if (buffer->cursorPos == 0)  return;

    if (buffer->s.s[buffer->cursorPos] == '\n') {
        buffer->currentLine--;
    }

    buffer->cursorPos--;
    buffer->s.s.erase(buffer->cursorPos, 1);
}

void buffer_removeline(Buffer* buffer) {
    int p = buffer->cursorPos;

    if (p == 0)  return;

    p--;

    while (p > 0 && buffer->s.s[p] != '\n') {
        p--;
    }

    if (p != 0)  p++;

    buffer->s.s.erase(buffer->s.s.begin()+p, buffer->s.s.begin()+buffer->cursorPos);

    buffer->cursorPos = p;
}

void buffer_moveLeft(Buffer* buffer) {
    if (buffer->cursorPos > 0) {
        buffer->cursorPos--;
        if (buffer->s.s[buffer->cursorPos] == '\n') {
            buffer->currentLine--;
        }
    }
}

void buffer_moveRight(Buffer* buffer) {
    if (buffer->cursorPos < buffer->s.s.size()) {
        if (buffer->s.s[buffer->cursorPos] == '\n') {
            buffer->currentLine++;
        }
        buffer->cursorPos++;
    }
}

void buffer_moveUp(Buffer* buffer) {
    int linePos = buffer->getCurrentPositionInLine();

    if (linePos == buffer->cursorPos)  return;

    buffer->currentLine--;

    buffer_moveToBegginingOfLine(buffer);

    buffer->cursorPos--;
    buffer_moveToBegginingOfLine(buffer);

    for (int i = 0; i < linePos && buffer->s.s[buffer->cursorPos] != '\n'; ++i) {
        buffer->cursorPos++;
    }
}

void buffer_moveDown(Buffer* buffer) {
    int linePos = buffer->getCurrentPositionInLine();

    int previousCursorPos = buffer->cursorPos;

    buffer_moveToEndOfLine(buffer);

    if (buffer->cursorPos == buffer->s.s.size()-1) {
        buffer->cursorPos = previousCursorPos;
        return;
    }

    buffer->currentLine++;

    buffer->cursorPos++;

    for (int i = 0; i < linePos && buffer->s.s[buffer->cursorPos] != '\n' && buffer->cursorPos < buffer->s.s.size(); ++i) {
        buffer->cursorPos++;
    }
}

void buffer_moveToEndOfLine(Buffer* buffer) {
    while (1) {
        if (buffer->cursorPos == buffer->s.s.size() || buffer->s.s[buffer->cursorPos] == '\n') {
            return;
        }

        buffer->cursorPos++;
    }
}

void buffer_moveToBegginingOfLine(Buffer* buffer) {
    buffer->cursorPos -= buffer->getCurrentPositionInLine();
    if (buffer->cursorPos < 0) {
        buffer->cursorPos = 0;
    }
}

void buffer_moveWordForward(Buffer* buffer) {
    if (buffer->cursorPos == buffer->s.s.size())  return;

    MyString ms;
    ms.pos = buffer->cursorPos;
    ms.s = buffer->s.s;

    auto w = readNextWord(ms);

    if (buffer->s.s[buffer->cursorPos] == '\n') {
        buffer->currentLine++;
    }

    buffer->cursorPos += w.size();
}

void buffer_moveWordBackword(Buffer* buffer) {
    if (buffer->cursorPos == 0)  return;

    MyString ms;
    ms.pos = buffer->cursorPos - 1;
    ms.s = buffer->s.s;

    auto w = readPreviousWord(ms);

    buffer->cursorPos -= w.size();

    if (buffer->s.s[buffer->cursorPos] == '\n') {
        buffer->currentLine--;
    }
}
