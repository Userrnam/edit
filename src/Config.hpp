#pragma once

#include <unordered_map>
#include <vector>
#include <SFML/Window/Keyboard.hpp>
#include <stdint.h>
#include <string>

#include "Buffer2D.hpp"

typedef void (*BufferFunction)(Buffer*);

struct KeyBindingInfo {
    uint16_t key;
    uint8_t  cmdKeys;
    uint8_t  mode;
};

union KeyBinding {
    uint32_t code;
    KeyBindingInfo info;

    KeyBinding(uint16_t key, uint8_t cmdKeys, uint8_t mode = 0) {
        info.key = key;
        info.cmdKeys = cmdKeys;
        info.mode = mode;
    }

    KeyBinding() {
        code = 0;
    }

    bool operator==(KeyBinding binding) const {
        return code == binding.code;
    }
};

namespace std {
    template <>
    struct hash<KeyBinding> {
        std::size_t operator()(KeyBinding k) const {
            using std::size_t;
            using std::hash;
            using std::string;

            return hash<uint32_t>()(k.code);
        }
    };
}

struct BindingSequence {
    std::vector<BufferFunction> sequence;

    BindingSequence(std::vector<BufferFunction> _sequence) : sequence(_sequence) {}
    BindingSequence(BufferFunction fp) {
        sequence.push_back(fp);
    }
    BindingSequence() {}
};

struct Config {
    std::unordered_map<KeyBinding, BindingSequence> keyBindings;
    std::string fontName = "Hack-Regular.ttf";

    void load(const std::string& path);
};
