#pragma once

#include <string>

namespace Engine
{
    struct WindowCreateInfo
    {
        int height{};
        int width{};
        std::string title{};
    };
}
