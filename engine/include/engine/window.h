#pragma once

#include <string>

namespace Engine
{
    struct NativeWindow;

    struct WindowInitInfo
    {
        int height{};
        int width{};
        std::string title{};
    };

    class Window
    {
    public:
        bool Init(const WindowInitInfo& info);
        void PollEvents();
        void SwapBuffers();
        bool ShouldQuit();

        void* GetFramebuffer();
        int GetWidth();
        int GetHeight();

        void SetTitle(const std::string& title);
    
    private:
        NativeWindow* _handle;
    };
}