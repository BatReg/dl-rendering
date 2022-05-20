#pragma once

#include <functional>
#include <string>

namespace Engine::Low
{
    struct NativeWindow;

    typedef std::function<void (NativeWindow* handle, int key)> OnKey;
}

namespace Engine
{
    struct WindowCreateInfo
    {
        int height{};
        int width{};
        std::string title{};
    };
}
