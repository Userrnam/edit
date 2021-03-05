#include "Buffer2D.hpp"


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

void Buffer::loadText(const String &text) {
    String t;
    for (int i = 0; i < text.size(); ++i) {
        if (text[i] == '\n') {
            lines.push_back(t);
            t = "";
        } else {
            t += text[i];
        }
    }
    if (t.size()) {
        lines.push_back(t);
    }
    std::cout << lines.size() << std::endl;
}

int Buffer::getRelativeCursorPos(int start, sf::Vector2i pos) {
    int res = 0;
    for (int i = start; i < pos.y; ++i) {
        res += (lines[i].size() + 1);
    }
    res += (pos.x < lines[pos.y].size() ? pos.x : lines[pos.y].size());

    return res;
}

String Buffer::getLines(int start, int end, int* relativeCursorPos) {
    if (lines.empty()) {
        return "";
    }

    String result;

    if (end > lines.size())  end = lines.size();

    for (int i = start; i < end; ++i) {
        result += (lines[i] + "\n");
    }

    *relativeCursorPos = 0;
    for (int i = start; i < cursorPos.y; ++i) {
        *relativeCursorPos += (lines[i].size() + 1);
    }
    *relativeCursorPos += (cursorPos.x < lines[cursorPos.y].size() ? cursorPos.x : lines[cursorPos.y].size());

    return result;
}

void Buffer::addChar(char c) {
    clampX();

    if (c == '\n') {
        String remaining = "";

        if (lines.size() != cursorPos.x) {
            remaining = lines[cursorPos.y].substr(cursorPos.x);
            lines[cursorPos.y].erase(cursorPos.x);
        }

        cursorPos.y++;
        lines.insert(lines.begin() + cursorPos.y, remaining);
        cursorPos.x = 0;

    } else {
        lines[cursorPos.y].insert(lines[cursorPos.y].begin() + cursorPos.x, c);
        cursorPos.x++;
    }
}

bool Buffer::onLastLine() {
    return cursorPos.y == lines.size()-1;
}

void Buffer::clampX() {
    if (cursorPos.x > lines[cursorPos.y].size()) {
        cursorPos.x = lines[cursorPos.y].size();
    }
}

void buffer_eraseChar(Buffer* buffer) {
    buffer->clampX();

    if (buffer->cursorPos.x == 0 && buffer->cursorPos.y == 0)  return;

    if (buffer->cursorPos.x == 0) {
        auto remaining = buffer->lines[buffer->cursorPos.y];
        buffer->lines.erase(buffer->lines.begin()+buffer->cursorPos.y);
        buffer->cursorPos.y--;
        buffer->cursorPos.x = buffer->lines[buffer->cursorPos.y].size();
        buffer->lines[buffer->cursorPos.y] += remaining;
    } else {
        buffer->cursorPos.x--;
        buffer->lines[buffer->cursorPos.y].erase(buffer->cursorPos.x, 1);
    }
}

void buffer_deleteChar(Buffer* buffer) {
    buffer->clampX();

    if (buffer->cursorPos.x == buffer->lines[buffer->cursorPos.y].size() && buffer->cursorPos.y == buffer->lines.size())  return;

    if (buffer->cursorPos.x == buffer->lines[buffer->cursorPos.y].size()) {
        auto nextLine = buffer->lines[buffer->cursorPos.y+1];
        buffer->lines.erase(buffer->lines.begin()+buffer->cursorPos.y+1);
        buffer->lines[buffer->cursorPos.y] += nextLine;
    } else {
        buffer->lines[buffer->cursorPos.y].erase(buffer->cursorPos.x, 1);
    }
}

void buffer_removeline(Buffer* buffer) {
    buffer->clampX();

    if (buffer->cursorPos.x == 0)  return;

    buffer->lines[buffer->cursorPos.y].erase(0, buffer->cursorPos.x);
    buffer->cursorPos.x = 0;
}

void buffer_moveLeft(Buffer* buffer) {
    buffer->clampX();

    if (buffer->cursorPos.x == 0)  return;

    buffer->cursorPos.x--;
}

void buffer_moveRight(Buffer* buffer) {
    buffer->clampX();

    if (buffer->cursorPos.x == buffer->lines[buffer->cursorPos.y].size())  return;

    buffer->cursorPos.x++;
}

void buffer_moveUp(Buffer* buffer) {
    if (buffer->cursorPos.y == 0)  return;

    buffer->cursorPos.y--;
}

void buffer_moveDown(Buffer* buffer) {
    if (buffer->cursorPos.y == buffer->lines.size()-1)  return;

    buffer->cursorPos.y++;
}

void buffer_moveToEndOfLine(Buffer* buffer) {
    buffer->cursorPos.x = buffer->lines[buffer->cursorPos.y].size();
}

void buffer_moveToBeginningOfLine(Buffer* buffer) {
    buffer->cursorPos.x = 0;
}

void buffer_moveWordForward(Buffer* buffer) {
    buffer->clampX();

    if (buffer->cursorPos.y == buffer->lines.size()-1 && buffer->cursorPos.x == buffer->lines[buffer->cursorPos.y].size())  return;

    if (buffer->cursorPos.x == buffer->lines[buffer->cursorPos.y].size()) {
        buffer->cursorPos.y++;
        buffer->cursorPos.x = 0;
    } else {
        MyString ms;
        ms.pos = buffer->cursorPos.x;
        ms.s = buffer->lines[buffer->cursorPos.y];

        auto w = readNextWord(ms);

        buffer->cursorPos.x += w.size();
    }
}

void buffer_moveWordBackword(Buffer* buffer) {
    buffer->clampX();

    if (buffer->cursorPos.y == 0 && buffer->cursorPos.x == 0)  return;

    if (buffer->cursorPos.x == 0) {
        buffer->cursorPos.y--;
        buffer->cursorPos.x = buffer->lines[buffer->cursorPos.y].size();
    } else {
        MyString ms;
        ms.pos = buffer->cursorPos.x - 1;
        ms.s = buffer->lines[buffer->cursorPos.y];

        auto w = readPreviousWord(ms);

        buffer->cursorPos.x -= w.size();
    }
}

void buffer_moveToEndOfFile(Buffer* buffer) {
    buffer->cursorPos.y = buffer->lines.size()-1;
    buffer->cursorPos.x = buffer->lines[buffer->cursorPos.y].size();
}

void buffer_moveToBeginningOfFile(Buffer* buffer) {
    buffer->cursorPos.x = 0;
    buffer->cursorPos.y = 0;
}

void buffer_toggleSelection(Buffer* buffer) {
    if (buffer->selectionStartPos.x < 0) {
        buffer->selectionStartPos = buffer->cursorPos;
    } else {
        buffer->selectionStartPos = {-1, 0};
    }
}

void buffer_beginSelection(Buffer* buffer) {
    buffer->selectionStartPos = buffer->cursorPos;
}

void buffer_endSelection(Buffer* buffer) {
    buffer->selectionStartPos = {-1, 0};
}

void buffer_removeSelection(Buffer* buffer) {
    if (buffer->selectionStartPos.x == -1) {
        return;
    }

    auto start = buffer->selectionStartPos;
    auto end   = buffer->cursorPos;

    if (start.y != end.y) {
        if (start.y > end.y) {
            end   = buffer->selectionStartPos;
            start = buffer->cursorPos;
        }
        
        auto& line = buffer->lines[start.y];
        line.erase(line.begin() + start.x, line.begin() + line.size());

        if (end.y - start.y - 1 > 0) {
            buffer->lines.erase(buffer->lines.begin() + start.y + 1, buffer->lines.begin() + end.y);
        }

        buffer->lines[start.y + 1].erase(0, end.x);

        buffer->lines[start.y] += buffer->lines[start.y+1];

        buffer->lines.erase(buffer->lines.begin() + start.y + 1);
    } else {
        if (start.x > end.x) {
            end   = buffer->selectionStartPos;
            start = buffer->cursorPos;
        }

        auto& line = buffer->lines[start.y];
        line.erase(line.begin() + start.x, line.begin() + end.x);
    }

    buffer->cursorPos = start;
    buffer->selectionStartPos = {-1, 0};
}
