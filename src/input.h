#pragma once

namespace Key {
    constexpr int W     = 0x57;
    constexpr int A     = 0x41;
    constexpr int S     = 0x53;
    constexpr int D     = 0x44;
    constexpr int Space = 0x20;
    constexpr int LCtrl  = 341;

}

struct Input {
    bool keys[348] = { false };
    int mouseX = 0;
    int mouseY = 0;
    int lastX = 0;
    int lastY = 0;

    bool IsDown(int key) const {
        if (key < 0 || key >= 348) return false;
        return keys[key];
    }
};