#pragma once

#include "platform.h"

#include <engine/core.h>

#include <string>

namespace Engine::Low::Internal
{
    struct _NativeWindow
    {
        int width{};
        int height{};
        std::string title{};
        bool shouldQuit{};
        void* framebuffer{};

        struct
        {
        } callbacs{};

        WINDOW_NATIVE_STATE;
    };

    bool _WindowCreate(_NativeWindow* window);
    void _WindowPollEvents(_NativeWindow* window);
    void _WindowSwapBuffers(const _NativeWindow* window);

    bool _WindowGetKeyState(const _NativeWindow* window, const Key key);

    void _WindowSetTitle(const _NativeWindow* window, const std::string& title);
}
