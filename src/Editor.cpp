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
    BufferFunction function;
    uint cmdKeys = 0;

    KeyBinding() {}
    KeyBinding(BufferFunction fp, uint keys) : function(fp), cmdKeys(keys) {}
};

std::unordered_map<sf::Keyboard::Key, std::vector<KeyBinding>> bindings = {
    {sf::Keyboard::H,    { KeyBinding(buffer_moveLeft, CmdKeyControl) } },
    {sf::Keyboard::L,    { KeyBinding(buffer_moveRight, CmdKeyControl) } },
    {sf::Keyboard::J,    { KeyBinding(buffer_moveDown, CmdKeyControl) } },
    {sf::Keyboard::K,    { KeyBinding(buffer_moveUp, CmdKeyControl) } },
    {sf::Keyboard::Num0, { KeyBinding(buffer_moveToBegginingOfLine, CmdKeyControl) } },
    {sf::Keyboard::E,    { KeyBinding(buffer_moveToEndOfLine, CmdKeyControl) } },

    {sf::Keyboard::BackSpace, { KeyBinding(buffer_eraseChar, 0), KeyBinding(buffer_removeline, CmdKeySystem) } },

    {sf::Keyboard::Left,  { KeyBinding(buffer_moveLeft, 0) } },
    {sf::Keyboard::Right, { KeyBinding(buffer_moveRight, 0) } },
    {sf::Keyboard::Up,    { KeyBinding(buffer_moveUp, 0) } },
    {sf::Keyboard::Down,  { KeyBinding(buffer_moveDown, 0) } },

    {sf::Keyboard::W,  { KeyBinding(buffer_removeWordForward,  CmdKeyShift | CmdKeySystem) } },
    {sf::Keyboard::B,  { KeyBinding(buffer_removeWordBackword, CmdKeyShift | CmdKeySystem) } },
};

void Editor::draw(sf::RenderWindow& window) {
    window.draw(cursor);

    sf::Text t;
    t.setFont(font);
    t.setCharacterSize(text.getCharacterSize());

    MyString ms;
    ms.s = text.getString();

    int index = 0;
    while (1) {
        auto word = readNextWord(ms);

        if (word == "") {
            buffer.s.pos = 0;
            break;
        }

        auto pos = text.findCharacterPos(index);

        if (pos.y > window.getSize().y) {
            buffer.s.pos = 0;
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

    text.setString("H\n");
    charSize = text.getGlobalBounds().getSize();
    charSize.y = text.getCharacterSize();
    text.setString("");

    cursor.setSize(charSize);
    cursor.setFillColor(sf::Color::Green);
}

void Editor::update(EditInfo info) {
    auto it = bindings.find(info.event.code);

    bool handled = false;

    auto code = eventToCode(info.event);

    if (it != bindings.end()) {
        for (auto v : it->second) {
            if (v.cmdKeys == code) {
                v.function(&buffer);
                handled = true;
                break;
            }
        }
    }

    if (!handled && info.c != -1) {
        buffer.addChar(info.c);
    }

    auto s = buffer.getLines(0, 30);

    text.setString(s);

    if (buffer.isEmpty()) {
        cursor.setPosition(sf::Vector2f(0, 0));
    } else {
        auto pos = text.findCharacterPos(buffer.cursorPos);

        cursor.setPosition(pos);
    }
}
