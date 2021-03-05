#include <unordered_map>

#include "Editor.hpp"


sf::Color getColorForWord(const sf::String& word) {
    static const std::vector<sf::String> keywords = {
        "if", "while", "do", "goto", "void", "int", "float", "double", "char"
    };

    if (std::find(keywords.begin(), keywords.end(), word) != keywords.end()) {
        return sf::Color::Red;
    }

    return sf::Color::Black;
}

const uint CmdKeyAlt     = 1;
const uint CmdKeyControl = 2;
const uint CmdKeyShift   = 4;
const uint CmdKeySystem  = 8;
const float leftSpacing  = 50;

uint eventToCode(sf::Event::KeyEvent e) {
    uint res = 0;

    if (e.alt)     res |= CmdKeyAlt;
    if (e.control) res |= CmdKeyControl;
    if (e.shift)   res |= CmdKeyShift;
    if (e.system)  res |= CmdKeySystem;

    return res;
}

typedef void (*BufferFunction)(Buffer*);

struct KeyBinding {
    std::vector<BufferFunction> sequence;
    uint cmdKeys = 0;

    KeyBinding() {}
    KeyBinding(std::vector<BufferFunction> _sequence, uint keys) : sequence(_sequence), cmdKeys(keys) {}
    KeyBinding(BufferFunction fp, uint keys) {
        sequence.push_back(fp);
        cmdKeys = keys;
    }
};

std::unordered_map<sf::Keyboard::Key, std::vector<KeyBinding>> bindings = {
    {sf::Keyboard::H,    { KeyBinding(buffer_moveLeft, CmdKeyControl) } },
    {sf::Keyboard::L,    { KeyBinding(buffer_moveRight, CmdKeyControl) } },
    {sf::Keyboard::J,    { KeyBinding(buffer_moveDown, CmdKeyControl) } },
    {sf::Keyboard::K,    { KeyBinding(buffer_moveUp, CmdKeyControl) } },
    {sf::Keyboard::Num0, { KeyBinding(buffer_moveToBeginningOfLine, CmdKeyControl) } },
    {sf::Keyboard::E,    { KeyBinding(buffer_moveToEndOfLine, CmdKeyControl) } },

    {sf::Keyboard::BackSpace, { KeyBinding(buffer_eraseChar, 0), KeyBinding(buffer_removeline, CmdKeySystem) } },
    {sf::Keyboard::Delete,    { KeyBinding(buffer_deleteChar, 0) } },

    {sf::Keyboard::Left,  { KeyBinding(buffer_moveLeft, 0) } },
    {sf::Keyboard::Right, { KeyBinding(buffer_moveRight, 0) } },
    {sf::Keyboard::Up,    { KeyBinding(buffer_moveUp, 0) } },
    {sf::Keyboard::Down,  { KeyBinding(buffer_moveDown, 0) } },

    {sf::Keyboard::W,  { KeyBinding(buffer_moveWordForward,  CmdKeyShift | CmdKeySystem) } },
    {sf::Keyboard::B,  {
        KeyBinding(buffer_moveWordBackword, CmdKeyShift | CmdKeySystem),
        KeyBinding(buffer_moveToEndOfFile, CmdKeyControl)
    } },

    {sf::Keyboard::D, { KeyBinding(buffer_removeSelection, CmdKeyControl) } },

    {sf::Keyboard::G, { KeyBinding(buffer_moveToBeginningOfFile, CmdKeyControl) } },

    {sf::Keyboard::S, { KeyBinding(buffer_toggleSelection, CmdKeyControl) } },
};

void Editor::draw(sf::RenderWindow& window) {
    sf::Text t;
    t.setFont(font);
    t.setCharacterSize(text.getCharacterSize());

    // draw numbers
    std::string nums = "";
    for (int n = firstVisibleLine; n < lastVisibleLine; ++n) {
        nums += std::to_string(n) + "\n";
    }

    t.setPosition({0, 0});
    t.setFillColor(sf::Color::Black);
    t.setString(nums);

    float ls = t.getLocalBounds().width + leftSpacing;

    text.move({ls, 0});
    cursor.move({ls, 0});

    window.draw(t);

    // draw selection
    if (buffer.selectionStartPos.x != -1) {
        sf::RectangleShape selection;
        selection.setFillColor(sf::Color::Red);

        auto pos1 = buffer.selectionStartPos;
        int inc = pos1.y > buffer.cursorPos.y ? -1 : 1;

        if (inc < 0) {
            int k = 0;
        }

        while (1) {
            int index = buffer.getRelativeCursorPos(firstVisibleLine, pos1);
            selection.setPosition(text.findCharacterPos(index));

            if (pos1.y != buffer.cursorPos.y) {
                auto p = pos1;

                if (inc == 1) {
                    p.x = 10000000;
                    pos1.x = 0;
                } else {
                    p.x = 0;
                    pos1.x = 10000000;
                }

                index = buffer.getRelativeCursorPos(firstVisibleLine, p);
                selection.setSize({
                    text.findCharacterPos(index).x - selection.getPosition().x,
                    cursor.getSize().y
                });

            } else {
                selection.setSize({
                    cursor.getPosition().x - selection.getPosition().x,
                    cursor.getSize().y
                });
            }

            window.draw(selection);

            if (pos1.y == buffer.cursorPos.y) {
                break;
            }

            pos1.y += inc;
        }
    }

    window.draw(cursor);

    MyString ms;
    ms.s = text.getString();

    int index = 0;

    while (1) {
        auto word = readNextWord(ms);

        if (word == "") {
            break;
        }

        auto pos = text.findCharacterPos(index);

        if (pos.y > window.getSize().y) {
            break;
        }

        t.setPosition(pos);
        t.setString(word);
        t.setFillColor(getColorForWord(word));

        index += word.size();

        window.draw(t);
    }
}

void Editor::init() {
    font.loadFromFile("Hack-Regular.ttf");

    text.setFont(font);
    text.setFillColor(sf::Color::Black);

    std::string dummy = "";

    for (int i = 0; i < 100; ++i) {
        dummy += "H\n";
    }

    dummy.pop_back();

    text.setString(dummy);

    charSize = text.getLocalBounds().getSize();
    charSize.y /= 100;

    text.setString("");

    cursor.setSize(charSize);
    cursor.setFillColor(sf::Color::Green);
}

void Editor::updateDrawInfo(const sf::RenderWindow& window) {
    lastVisibleLine = firstVisibleLine + window.getSize().y / cursor.getSize().y + 2;

    if (buffer.cursorPos.y >= lastVisibleLine - 3) {
        topLine += (buffer.cursorPos.y - lastVisibleLine + 4);
    } else if (buffer.cursorPos.y < firstVisibleLine) {
        topLine -= (firstVisibleLine - buffer.cursorPos.y + 1);
    }

    int cursorPos;
    firstVisibleLine = topLine;
    lastVisibleLine  = topLine + window.getSize().y / cursor.getSize().y + 2;

    if (lastVisibleLine > buffer.lines.size() + 3) {
        auto diff = lastVisibleLine - buffer.lines.size() - 3;
        int val = firstVisibleLine - diff;
        if (val >= 0) {
            firstVisibleLine -= diff;
            lastVisibleLine  -= diff;
        }
    }

    auto s = buffer.getLines(firstVisibleLine, lastVisibleLine, &cursorPos);

    text.setString(s);
    text.setPosition({ 0, scrollValue });

    if (buffer.isEmpty()) {
        cursor.setPosition(sf::Vector2f(0, text.getLineSpacing()));
    } else {
        auto pos = text.findCharacterPos(cursorPos);

        cursor.setPosition(pos);
    }
}

void Editor::update(EditInfo info) {
    auto it = bindings.find(info.event.code);

    bool handled = false;

    auto code = eventToCode(info.event);

    if (it != bindings.end()) {
        for (auto v : it->second) {
            if (v.cmdKeys == code) {
                for (auto function : v.sequence) {
                    function(&buffer);
                }
                handled = true;
                break;
            }
        }
    }

    if (!handled && info.c != -1) {
        buffer.addChar(info.c);
    }
}

void Editor::scroll(float delta) {
    // scrollValue += delta;

    // int k = scrollValue / cursor.getSize().y;

    // std::cout << k << std::endl;

    // if (abs(k) > 0) {
    //     scrollValue -= k * cursor.getSize().y;
    //     topLine -= k;

    //     if (topLine < 0) {
    //         topLine = 0;
    //         scrollValue = 0;
    //     }
    // }

    // if (scrollValue > cursor.getSize().y) {
    //     scrollValue -= k * cursor.getSize().y;
    //     topLine++;
    // } else if (scrollValue < 0) {
    //     scrollValue -= k * cursor.getSize().y;
    //     topLine--;

    //     if (topLine < 0) {
    //         topLine = 0;
    //         scrollValue = 0;
    //     }
    // }
}
