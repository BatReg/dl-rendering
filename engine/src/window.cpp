#include <engine/window.h>
#include "int_window.h"

#include <utility>

namespace Engine
{
    bool Window::Init(const WindowCreateInfo& info)
    {
        _handle = Low::WindowCreate(info);
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

    std::pair<int, int> Window::GetMousePos() const
    {
        if(_handle)
        {
            return Low::WindowGetMousePos(_handle);
        }

        return std::pair<int, int>(0, 0);
    }

    bool Window::IsKeyDown(const Key key) const
    {
        if(_handle)
        {
            return Low::WindowIsKeyDown(_handle, key);
        }

        return false;
    }

    bool Window::IsMouseButtonDown(const MouseButton button) const
    {
        if(_handle)
        {
            return Low::WindowIsMouseButtonDown(_handle, button);
        }

        return false;
    }

    void Window::SetTitle(const std::string& title)
    {
        if(_handle)
        {
            Low::WindowSetTitle(_handle, title);
        }
    }

    void Window::RequestQuit()
    {
        if(_handle)
        {
            Low::WindowRequestQuit(_handle);
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

    std::pair<int, int> WindowGetMousePos(const NativeWindow* handle)
    {
        const Internal::_NativeWindow* window = reinterpret_cast<const Internal::_NativeWindow*>(handle);
        return std::pair(window->mouse.lastX, window->mouse.lastY);
    }

    bool WindowIsKeyDown(const NativeWindow* handle, const Key key)
    {
        const Internal::_NativeWindow* window = reinterpret_cast<const Internal::_NativeWindow*>(handle);
        return window->keys[static_cast<uint16_t>(key)];
    }

    bool WindowIsMouseButtonDown(const NativeWindow* handle, const MouseButton button)
    {
        const Internal::_NativeWindow* window = reinterpret_cast<const Internal::_NativeWindow*>(handle);
        return window->mouseButtons[static_cast<uint8_t>(button)];
    }

    void WindowSetTitle(NativeWindow* handle, const std::string& title)
    {
        Internal::_NativeWindow* window = reinterpret_cast<Internal::_NativeWindow*>(handle);
        window->title = title;

        Internal::_WindowSetTitle(window, title);
    }

    void WindowRequestQuit(NativeWindow* handle)
    {
        Internal::_NativeWindow* window = reinterpret_cast<Internal::_NativeWindow*>(handle);
        window->shouldQuit = true;
    }
}
