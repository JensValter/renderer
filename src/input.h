#pragma once

namespace Key
{
    constexpr int W = 'W';
    constexpr int A = 'A';
    constexpr int S = 'S';
    constexpr int D = 'D';
    constexpr int Space = 0x20;
    constexpr int Ctrl  = 0x11;
}


struct Input
{
    bool keys[256] = {};
    bool keysPressed[256] = {};
    bool keysReleased[256] = {};


    void OnKeyDown(int vk)
    {
        keysPressed[vk] = true;
        keysReleased[vk] = false;
        keys[vk] = true;
    }

    void OnKeyUp(int vk)
    {   keysReleased[vk] = true;
        keysPressed[vk] = false;
        keys[vk] = false;   
    }

    bool IsDown(int vk) const      { return keys[vk]; }
    bool WasPressed(int vk) const  { return keysPressed[vk]; }
    bool WasReleased(int vk) const { return keysReleased[vk]; }
};
