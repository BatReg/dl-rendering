#pragma once

#include <string>
#include <memory>

namespace Engine
{
    struct WindowInitInfo
    {
        int height;
        int width;
        std::string title;
    };

    class Window
    {
    public:
        ~Window();
        Window() noexcept;
        
        bool Init(const WindowInitInfo& info);
        void SwapBuffers();
        void PollEvents();
        bool ShouldQuit();

        void* GetFramebuffer();
        int GetWidth();
        int GetHeight();

        void SetTitle(std::string title);
    
    private:
        class Impl;
        std::unique_ptr<Impl> _pimpl;
    };
}