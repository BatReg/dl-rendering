#include <engine/window.h>
#include "int_window.h"

#include <iostream>

namespace Engine
{
    bool Window::Init(const WindowCreateInfo& info)
    {
        _handle = Low::WindowCreate(info);
        Low::WindowSetOnKey(_handle, std::bind(&Window::OnWindowKey, this, std::placeholders::_1, std::placeholders::_2));
        return true;
    }

    void Window::PollEvents()
    {
        if(_handle)
        {
            Low::WindowPollEvents(_handle);
        }
    }

    void Window::SwapBuffers()
    {
        if(_handle)
        {
            Low::WindowSwapBuffers(_handle);
        }
    }

    bool Window::ShouldQuit() const
    {
        if(_handle)
        {
            return Low::WindowShouldQuit(_handle);
        }

        return false;
    }

    void* Window::GetFramebuffer() const
    {
        if(_handle)
        {
            return Low::WindowGetFramebuffer(_handle);
        }

        return nullptr;
    }

    int Window::GetWidth() const
    {
        if(_handle)
        {
            return Low::WindowGetWidth(_handle);
        }

        return 0;
    }

    int Window::GetHeight() const
    {
        if(_handle)
        {
            return Low::WindowGetHeight(_handle);
        }

        return 0;
    }

    void Window::SetTitle(const std::string& title)
    {
        if(_handle)
        {
            Low::WindowSetTitle(_handle, title);
        }
    }
 
    void Window::SetOnKey(const OnKey& callback)
    {
        _onKey = callback;
    }

    void Window::OnWindowKey(Engine::Low::NativeWindow* window, int keyCode)
    {
        if(_onKey)
        {
            _onKey(keyCode);
        }
    }    
}

namespace Engine::Low
{
    NativeWindow* WindowCreate(const WindowCreateInfo& info)
    {
        Internal::_NativeWindow* window = new Internal::_NativeWindow();
        window->width = info.width;
        window->height = info.height;
        window->title = info.title;

        if(!Internal::_WindowCreate(window))
        {
            delete window;
            return nullptr;
        }

        return reinterpret_cast<NativeWindow*>(window);
    }

    void WindowPollEvents(NativeWindow* handle)
    {
        Internal::_NativeWindow* window = reinterpret_cast<Internal::_NativeWindow*>(handle);
        Internal::_WindowPollEvents(window);
    }

    void WindowSwapBuffers(const NativeWindow* handle)
    {
        const Internal::_NativeWindow* window = reinterpret_cast<const Internal::_NativeWindow*>(handle);
        Internal::_WindowSwapBuffers(window);
    }

    bool WindowShouldQuit(const NativeWindow* handle)
    {
        const Internal::_NativeWindow* window = reinterpret_cast<const Internal::_NativeWindow*>(handle);
        return window->shouldQuit;
    }

    void* WindowGetFramebuffer(const NativeWindow* handle)
    {
        const Internal::_NativeWindow* window = reinterpret_cast<const Internal::_NativeWindow*>(handle);
        return window->framebuffer;
    }

    int WindowGetWidth(const NativeWindow* handle)
    {
        const Internal::_NativeWindow* window = reinterpret_cast<const Internal::_NativeWindow*>(handle);
        return window->width;
    }

    int WindowGetHeight(const NativeWindow* handle)
    {
        const Internal::_NativeWindow* window = reinterpret_cast<const Internal::_NativeWindow*>(handle);
        return window->height;
    }

    void WindowSetTitle(NativeWindow* handle, const std::string& title)
    {
        Internal::_NativeWindow* window = reinterpret_cast<Internal::_NativeWindow*>(handle);
        window->title = title;

        Internal::_WindowSetTitle(window, title);
    }

    void WindowSetOnKey(NativeWindow* handle, const OnKey& callback)
    {
        Internal::_NativeWindow* window = reinterpret_cast<Internal::_NativeWindow*>(handle);
        window->callbacs.onKey = callback;
    }
}
