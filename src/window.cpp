#include "window.h"
#include <vector>

static std::vector<uint32_t> g_pixels;
static Window *g_window = nullptr;
static Input *g_input = nullptr;

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_KEYDOWN:
        if (g_input)
            g_input->OnKeyDown((int)wparam);
        return 0;

    case WM_KEYUP:
        if (g_input)
            g_input->OnKeyUp((int)wparam);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

bool CreateAppWindow(Window &window, int width, int height)
{
    g_window = &window;

    window.width = width;
    window.height = height;

    g_pixels.resize(width * height);
    window.pixels = g_pixels.data();

    g_input = &window.input;

    HINSTANCE instance = GetModuleHandle(nullptr);

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = instance;
    wc.lpszClassName = L"MyWindowClass";

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        wc.lpszClassName,
        L"Framebuffer Window",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width,
        height,
        nullptr,
        nullptr,
        instance,
        nullptr);

    if (!hwnd)
        return false;

    window.handle = hwnd;
    return true;
}

void DestroyAppWindow(Window &window)
{
    if (window.handle)
    {
        DestroyWindow((HWND)window.handle);
        window.handle = nullptr;
    }
}

bool ProcessWindowMessages()
{
    MSG msg = {};

    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            return false;

        DispatchMessage(&msg);
    }

    return true;
}

void PresentWindowBuffer(Window &window)
{
    HWND hwnd = (HWND)window.handle;
    HDC dc = GetDC(hwnd);

    BITMAPINFO info = {};
    info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    info.bmiHeader.biWidth = window.width;
    info.bmiHeader.biHeight = -window.height;
    info.bmiHeader.biPlanes = 1;
    info.bmiHeader.biBitCount = 32;
    info.bmiHeader.biCompression = BI_RGB;

    StretchDIBits(
        dc,
        0,
        0,
        window.width,
        window.height,
        0,
        0,
        window.width,
        window.height,
        window.pixels,
        &info,
        DIB_RGB_COLORS,
        SRCCOPY);

    ReleaseDC(hwnd, dc);
}