#pragma once

#include "win32_core.h"

namespace Engine::Internal
{
    struct Win32WindowState
    {
        HWND windowHandle{};
        HDC bitmapDeviceContext{};
        HBITMAP bitmapHandle{};
    };
}