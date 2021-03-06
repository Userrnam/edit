#include "Config.hpp"

#include <fstream>

const uint32_t CmdKeyAlt     = 1;
const uint32_t CmdKeyControl = 2;
const uint32_t CmdKeyShift   = 4;
const uint32_t CmdKeySystem  = 8;

std::unordered_map<KeyBinding, BindingSequence> defaultKeyBindings = {
    { KeyBinding(sf::Keyboard::BackSpace, 0), BindingSequence(buffer_eraseChar) },
    { KeyBinding(sf::Keyboard::BackSpace, CmdKeySystem), BindingSequence(buffer_removeLine) },
    { KeyBinding(sf::Keyboard::Delete, 0), BindingSequence(buffer_deleteChar) },

    { KeyBinding(sf::Keyboard::Left, CmdKeyControl), BindingSequence(buffer_moveLeft) },
    { KeyBinding(sf::Keyboard::Right, CmdKeyControl), BindingSequence(buffer_moveRight) },
    { KeyBinding(sf::Keyboard::Down, CmdKeyControl), BindingSequence(buffer_moveDown) },
    { KeyBinding(sf::Keyboard::Up, CmdKeyControl), BindingSequence(buffer_moveUp) },
};

/*

format:

# comment
:<setting name>
settings

font: font_name

KeyBindings:
Control-H    buffer_moveRight

*/

void skipLeadingWS(std::fstream& f) {
    int c;

    while (1) {
        while ((f.peek() == '\t' || f.peek() == '\r' || f.peek() == '\n' || f.peek() == ' ') && f.peek() != EOF) {
            c = f.get();
        }

        if (f.peek() == '#') {
            while (c != EOF && c != '\n') {
                c = f.get();
            }

            if (c == EOF) {
                return;
            }
        } else {
            return;
        }
    }
}

std::vector<std::string> split(std::string str, char c){
    std::vector<std::string>result;

    while (str.size()) {
        int index = str.find(c);

        if (index != std::string::npos) {
            result.push_back(str.substr(0,index));
            str = str.substr(index + 1);
        } else {
            result.push_back(str);

            break;
        }
    }

    return result;
}

std::string nextToken(std::fstream& f, bool colIsToken = false) {
    std::string word;

    skipLeadingWS(f);

    while (1) {
        int c = f.get();

        if (c == EOF || c == '\t' || c == '\r' || c == ' ' || c == '\n') {
            return word;
        }

        if (c == ':' && colIsToken) {
            if (word.size()) {
                f.unget();
                return word;
            } else {
                word += c;
                return word;
            }
        }

        word += c;
    }
}

sf::Keyboard::Key stringToKey(const std::string& s) {
    if (s.size() == 1) {
        char c = s[0];

        if (isalpha(c)) {
            c = std::tolower(s[0]);
        }

        if ('a' <= c && c <= 'z') {
            return (sf::Keyboard::Key)(sf::Keyboard::A + (c - 'a'));
        }

        if ('0' <= c && c <= '9') {
            return (sf::Keyboard::Key)(sf::Keyboard::A + (c - '0'));
        }

        switch (c) {
            case '[':  return sf::Keyboard::LBracket;
            case ']':  return sf::Keyboard::RBracket;
            case ':':  return sf::Keyboard::SemiColon;
            case ',':  return sf::Keyboard::Comma;
            case '.':  return sf::Keyboard::Period;
            case '\'': return sf::Keyboard::Quote;
            case '/':  return sf::Keyboard::Slash;
            case '\\': return sf::Keyboard::BackSlash;
            case '~':  return sf::Keyboard::Tilde;
            case '=':  return sf::Keyboard::Equal;
            case '+':  return sf::Keyboard::Add;
            case '-':  return sf::Keyboard::Subtract;
            case '*':  return sf::Keyboard::Multiply;
        }
    }
    
    if (s == "Space")      return sf::Keyboard::Space;
    if (s == "Dash")       return sf::Keyboard::Hyphen;
    if (s == "Enter")      return sf::Keyboard::Enter;
    if (s == "Tab")        return sf::Keyboard::Tab;
    if (s == "Left")       return sf::Keyboard::Left;
    if (s == "Right")      return sf::Keyboard::Right;
    if (s == "Down")       return sf::Keyboard::Down;
    if (s == "Up")         return sf::Keyboard::Up;
    if (s == "Delete")     return sf::Keyboard::Delete;
    if (s == "BackSpace")  return sf::Keyboard::BackSpace;
    
    std::cout << "[3902] Unknown key " << s << std::endl;

    return sf::Keyboard::Hyphen;
}

#define SET_BUFFER_FUNCTION(func, name) (name == #func) fp = func

void readKeyBindings(Config* config, std::fstream& f) {
    while (1) {
        auto w = nextToken(f, true);

        if (w == ":" || w.empty()) {
            return;
        }

        auto keys = split(w, '-');
        KeyBinding binding;

        for (auto& k : keys) {
            if (k == "Control")      binding.info.cmdKeys |= CmdKeyControl;
            else if (k == "Shift")   binding.info.cmdKeys |= CmdKeyShift;
            else if (k == "System")  binding.info.cmdKeys |= CmdKeySystem;
            else if (k == "Alt")     binding.info.cmdKeys |= CmdKeyAlt;
            else {
                binding.info.key = stringToKey(k);
            }
        }

        w = nextToken(f);

        BufferFunction fp = 0;

        if SET_BUFFER_FUNCTION(buffer_toggleSelection, w);
        else if SET_BUFFER_FUNCTION(buffer_beginSelection, w);
        else if SET_BUFFER_FUNCTION(buffer_endSelection, w);
        else if SET_BUFFER_FUNCTION(buffer_eraseChar, w);
        else if SET_BUFFER_FUNCTION(buffer_deleteChar, w);
        else if SET_BUFFER_FUNCTION(buffer_removeLine, w);
        else if SET_BUFFER_FUNCTION(buffer_moveLeft, w);
        else if SET_BUFFER_FUNCTION(buffer_moveRight, w);
        else if SET_BUFFER_FUNCTION(buffer_moveUp, w);
        else if SET_BUFFER_FUNCTION(buffer_moveDown, w);
        else if SET_BUFFER_FUNCTION(buffer_moveToEndOfLine, w);
        else if SET_BUFFER_FUNCTION(buffer_moveToBeginningOfLine, w);
        else if SET_BUFFER_FUNCTION(buffer_moveWordForward, w);
        else if SET_BUFFER_FUNCTION(buffer_moveWordBackword, w);
        else if SET_BUFFER_FUNCTION(buffer_moveToEndOfFile, w);
        else if SET_BUFFER_FUNCTION(buffer_moveToBeginningOfFile, w);
        else if SET_BUFFER_FUNCTION(buffer_removeSelection, w);
        else {
            std::cout << "Unknown buffer function: " << w << std::endl;
        }

        if (fp) {
            auto it = config->keyBindings.find(binding);

            if (it != config->keyBindings.end()) {
                // override previous key binding
                it->second = BindingSequence(fp);
            } else {
                config->keyBindings[binding] = { BindingSequence(fp) };
            }
        }
    }
}

void Config::load(const std::string& path) {
    std::cout << sizeof(sf::Keyboard::Key) << std::endl;
    std::fstream f;
    f.open(path, std::ios::in);

    auto word = nextToken(f, true);

    if (word != ":") {
        std::cout << "[4901] Error\n";
    }

    while (1) {
        word = nextToken(f);

        if (word.empty()) {
            return;
        }

        if (word == "font") {
            fontName = nextToken(f);
            if (nextToken(f) != ":") {
                std::cout << "[4013] Error\n";
            }
        } else if (word == "KeyBindings") {
            readKeyBindings(this, f);
        } else {
            std::cout << "[3023] Unknown Option: " << word << std::endl;
        }
    }
}
