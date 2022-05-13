#pragma once

#include <engine/window.h>
#include "platform.h"

#include <string>

namespace Engine::Internal
{
    struct _NativeWindow
    {
        int width{};
        int height{};
        std::string title{};
        bool shouldQuit{};
        void* framebuffer;

        WINDOW_NATIVE_STATE;
    };

    struct NativeWindowCreateInfo
    {
        int width{};
        int height{};
        std::string title{};
    };

    NativeWindow* WindowCreate(const NativeWindowCreateInfo& info);
    void WindowPollEvents(NativeWindow* handle);
    void WindowSwapBuffers(const NativeWindow* handle);
    bool WindowShouldQuit(const NativeWindow* handle);

    void* WindowGetFramebuffer(const NativeWindow* handle);
    int WindowGetWidth(const NativeWindow* handle);
    int WindowGetHeight(const NativeWindow* handle);

    void WindowSetTitle(const NativeWindow* handle, const std::string& title);

}
