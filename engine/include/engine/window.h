#pragma once

#include <engine/core.h>

#include <string>

namespace Engine::Low
{
    struct NativeWindow;

    NativeWindow* WindowCreate(const WindowCreateInfo& info);
    void WindowPollEvents(NativeWindow* handle);
    void WindowSwapBuffers(const NativeWindow* handle);
    bool WindowShouldQuit(const NativeWindow* handle);

    void* WindowGetFramebuffer(const NativeWindow* handle);
    int WindowGetWidth(const NativeWindow* handle);
    int WindowGetHeight(const NativeWindow* handle);

    void WindowSetTitle(NativeWindow* handle, const std::string& title);
}

namespace Engine
{
    class Window
    {
    public:
        bool Init(const WindowCreateInfo& info);
        void PollEvents();
        void SwapBuffers();
        bool ShouldQuit();

        void* GetFramebuffer();
        int GetWidth();
        int GetHeight();

        void SetTitle(const std::string& title);
    
    private:
        Low::NativeWindow* _handle;
    };
}
