#include <engine/window.h>
#include "int_window.h"

namespace Engine
{
    bool Window::Init(const WindowInitInfo& info)
    {
        Internal::NativeWindowCreateInfo windowCreateInfo{};
        windowCreateInfo.width  = info.width;
        windowCreateInfo.height = info.height;
        windowCreateInfo.title  = info.title;        
        
        _handle = Internal::WindowCreate(windowCreateInfo);
        return true;
    }

    void Window::PollEvents()
    {
        if(_handle)
        {
            Internal::WindowPollEvents(_handle);
        }
    }

    void Window::SwapBuffers()
    {
        if(_handle)
        {
            Internal::WindowSwapBuffers(_handle);
        }
    }

    bool Window::ShouldQuit()
    {
        if(_handle)
        {
            return Internal::WindowShouldQuit(_handle);
        }

        return false;
    }

    void* Window::GetFramebuffer()
    {
        if(_handle)
        {
            return Internal::WindowGetFramebuffer(_handle);
        }

        return nullptr;
    }

    int Window::GetWidth()
    {
        if(_handle)
        {
            return Internal::WindowGetWidth(_handle);
        }

        return 0;
    }

    int Window::GetHeight()
    {
        if(_handle)
        {
            return Internal::WindowGetHeight(_handle);
        }

        return 0;
    }

    void Window::SetTitle(const std::string& title)
    {
        if(_handle)
        {
            Internal::WindowSetTitle(_handle, title);
        }
    }    
}
