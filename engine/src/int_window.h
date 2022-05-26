#pragma once

#include "platform.h"

#include <engine/core.h>

#include <cstdint>
#include <string>
#include <utility>

namespace Engine::Low::Internal
{
    struct _NativeWindow
    {
        int width{};
        int height{};
        std::string title{};
        bool shouldQuit{};
        void* framebuffer{};

        bool keys[Engine::KEY_SIZE]{};
        bool mouseButtons[Engine::MOUSE_BUTTON_SIZE]{};

        struct
        {
            uint32_t lastX{};
            uint32_t lastY{};
        } mouse {};

        struct
        {
        } callbacs{};

        WINDOW_NATIVE_STATE;
    };

    bool _WindowCreate(_NativeWindow* window);
    void _WindowPollEvents(_NativeWindow* window);
    void _WindowSwapBuffers(const _NativeWindow* window);

    void _WindowSetTitle(const _NativeWindow* window, const std::string& title);
}
