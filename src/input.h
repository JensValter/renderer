#pragma once

namespace Key {
    constexpr int W     = 0x57;
    constexpr int A     = 0x41;
    constexpr int S     = 0x53;
    constexpr int D     = 0x44;
    constexpr int Space = 0x20;
    constexpr int LCtrl  = 341;
    constexpr int Right = 262;
    constexpr int Left = 263;
    constexpr int Down = 264;
    constexpr int Up = 265;

}

struct Input {
    bool keys[348] = { false };
    int mouseX = 0;
    int mouseY = 0;

    bool IsDown(int key) const {
        if (key < 0 || key >= 348) return false;
        return keys[key];
    }
};