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
const float leftSpacing  = 100;

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

    {sf::Keyboard::Left,  { KeyBinding(buffer_moveLeft, 0) } },
    {sf::Keyboard::Right, { KeyBinding(buffer_moveRight, 0) } },
    {sf::Keyboard::Up,    { KeyBinding(buffer_moveUp, 0) } },
    {sf::Keyboard::Down,  { KeyBinding(buffer_moveDown, 0) } },

    {sf::Keyboard::W,  { KeyBinding(buffer_moveWordForward,  CmdKeyShift | CmdKeySystem) } },
    {sf::Keyboard::B,  {
        KeyBinding(buffer_moveWordBackword, CmdKeyShift | CmdKeySystem),
        KeyBinding(buffer_moveToEndOfFile, CmdKeyControl)
    } },

    {sf::Keyboard::G, { KeyBinding(buffer_moveToBeginningOfFile, CmdKeyControl) } },
};

void Editor::draw(sf::RenderWindow& window) {
    window.draw(cursor);

    sf::Text t;
    t.setFont(font);
    t.setCharacterSize(text.getCharacterSize());

    // draw numbers
    std::string nums = "";
    for (int n = buffer.firstVisibleLine; n < buffer.lastVisibleLine; ++n) {
        nums += std::to_string(n) + "\n";
    }

    t.setPosition({0, 0});
    t.setFillColor(sf::Color::Black);
    t.setString(nums);

    window.draw(t);

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
    if (buffer.currentLine >= buffer.lastVisibleLine - 3) {
        topLine += (buffer.currentLine - buffer.lastVisibleLine + 4);
    } else if (buffer.currentLine <= buffer.firstVisibleLine) {
        topLine -= (buffer.firstVisibleLine - buffer.currentLine + 1);

        if (topLine < 0)  topLine = 0;
    }

    std::cout << "On last line: " << buffer.onLastLine() << std::endl;
    std::cout << "Current Line is " << buffer.currentLine << std::endl;
    std::cout << "firstVisible Line is " << buffer.firstVisibleLine << std::endl;
    std::cout << "lastVisible Line is " << buffer.lastVisibleLine << std::endl;
    std::cout << "Top Line is " << topLine << std::endl;

    int cursorPos;

    auto s = buffer.getLines(topLine, topLine + window.getSize().y / cursor.getSize().y + 2, &cursorPos);

    text.setString(s);
    text.setPosition({ leftSpacing, scrollValue });

    if (buffer.isEmpty()) {
        cursor.setPosition(sf::Vector2f(leftSpacing, text.getLineSpacing()));
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
