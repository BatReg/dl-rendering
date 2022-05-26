#pragma once

#include "win32_core.h"

namespace Engine::Low::Internal
{
    struct Win32WindowState
    {
        HWND windowHandle{};
        HDC bitmapDeviceContext{};
        HBITMAP bitmapHandle{};

        bool keys[512]{};
    };
}