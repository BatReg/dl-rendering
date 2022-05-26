#pragma once

#include <engine/core.h>

#include <functional>
#include <string>

namespace Engine::Low
{
    NativeWindow* WindowCreate(const WindowCreateInfo& info);
    void WindowPollEvents(NativeWindow* handle);
    void WindowSwapBuffers(const NativeWindow* handle);
    bool WindowShouldQuit(const NativeWindow* handle);

    void* WindowGetFramebuffer(const NativeWindow* handle);
    int WindowGetWidth(const NativeWindow* handle);
    int WindowGetHeight(const NativeWindow* handle);

    bool WindowGetKeyState(const NativeWindow* handle, const Key key);

    void WindowSetTitle(NativeWindow* handle, const std::string& title);
    
    void WindowRequestQuit(NativeWindow* handle);
}

namespace Engine
{   
    class Window
    {
    public:
        bool Init(const WindowCreateInfo& info);
        void PollEvents();
        void SwapBuffers();
        bool ShouldQuit() const;

        void* GetFramebuffer() const;
        int GetWidth() const;
        int GetHeight() const;
        bool GetKeyState(const Key key) const;

        void SetTitle(const std::string& title);

        void RequestQuit();
    
    private:
        Low::NativeWindow* _handle{};
    };
}
