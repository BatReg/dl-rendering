#pragma once

#include "platform.h"

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

        WINDOW_NATIVE_STATE;
    };

    bool _WindowCreate(_NativeWindow* handle);
    void _WindowPollEvents(_NativeWindow* handle);
    void _WindowSwapBuffers(const _NativeWindow* handle);

    void _WindowSetTitle(const _NativeWindow* handle, const std::string& title);

}
