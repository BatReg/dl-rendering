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

    void WindowSetTitle(NativeWindow* handle, const std::string& title);
    void WindowSetOnKey(NativeWindow* handle, const OnKey& callback);
}

namespace Engine
{
    typedef std::function<void (int keyCode)> OnKey;
   
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
        void SetOnKey(const OnKey& callback);

    private:
        void OnWindowKey(Low::NativeWindow* handle, int keyCode);
    
    private:
        Low::NativeWindow* _handle{};
        OnKey _onKey{};
    };
}
