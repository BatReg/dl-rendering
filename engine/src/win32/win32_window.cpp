#include "../int_window.h"
#include "win32_core.h"

#include <cstdint>
#include <iostream>
#include <string>

namespace Engine::Low::Internal
{
    static constexpr wchar_t* WINDOW_CLASS_NAME = L"RendererWindow";

    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static void OnSize(_NativeWindow* window, int width, int height);
    static Key MapWParamToKey(WPARAM param);

    bool _WindowCreate(_NativeWindow* window)
    {
        WNDCLASSEXW wc{};
        wc.cbSize = sizeof(wc);
        wc.lpfnWndProc = WindowProc;
        wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
        wc.lpszClassName = WINDOW_CLASS_NAME;

        if(!RegisterClassExW(&wc))
        {
            return false;
        }

        std::wstring title(window->title.begin(), window->title.end());

        RECT rect{0, 0, window->width, window->height};
        AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

        window->win32.windowHandle = CreateWindowExW(
            NULL,
            WINDOW_CLASS_NAME,
            title.c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
            NULL, NULL,
            GetModuleHandleW(NULL),
            NULL
        );

        if(!window->win32.windowHandle)
        {
            return false;
        }

        SetWindowLongPtrW(window->win32.windowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        ShowWindow(window->win32.windowHandle, true);

        POINT p;
        GetCursorPos(&p);

        window->mouse.lastX = p.x;
        window->mouse.lastY = p.y;

        return true;
    }

    void _WindowPollEvents(_NativeWindow* window)
    {
        MSG msg;
        while(PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if(msg.message == WM_QUIT)
            {
                window->shouldQuit = true;
            }

            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    void _WindowSwapBuffers(const _NativeWindow* window)
    {        
        HDC hdc = GetDC(window->win32.windowHandle);
        BitBlt(
            hdc,
            0, 0, window->width, window->height,
            window->win32.bitmapDeviceContext,
            0, 0,
            SRCCOPY
        );

        DeleteDC(hdc);
    }

    void _WindowSetTitle(const _NativeWindow* window, const std::string& title)
    {
        std::wstring wTitle(title.begin(), title.end());
        SetWindowTextW(window->win32.windowHandle, wTitle.c_str());
    }

    LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        _NativeWindow* window = reinterpret_cast<_NativeWindow*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));

        if(window)
        {
            switch(message)
            {
                case WM_SIZE:
                {
                    RECT rect;
                    GetClientRect(hWnd, &rect);
                    OnSize(window, rect.right - rect.left, rect.bottom - rect.top);
                } break;

                case WM_DESTROY:
                {
                    PostQuitMessage(0);
                } break;

                case WM_KEYDOWN:
                case WM_KEYUP:
                {
                    Key key = MapWParamToKey(wParam);
                    bool isDown = message == WM_KEYDOWN;

                    window->keys[static_cast<uint16_t>(key)] = isDown;
                } break;

                case WM_LBUTTONDOWN:
                case WM_RBUTTONDOWN:
                case WM_MBUTTONDOWN:
                case WM_LBUTTONUP:
                case WM_RBUTTONUP:
                case WM_MBUTTONUP:
                {
                    MouseButton button;
                    if(message == WM_LBUTTONDOWN || message == WM_LBUTTONUP)
                    {
                        button = MouseButton::Left;
                    }
                    else if (message == WM_RBUTTONDOWN || message == WM_RBUTTONUP)
                    {
                        button = MouseButton::Right;
                    }
                    else if (message == WM_MBUTTONDOWN || message == WM_MBUTTONUP)
                    {
                        button = MouseButton::Middle;
                    }

                    bool isButtonDown = message == WM_LBUTTONDOWN || message == WM_RBUTTONDOWN || message == WM_MBUTTONDOWN;                    
                    window->mouseButtons[static_cast<uint8_t>(button)] = isButtonDown;

                    if(isButtonDown)
                    {
                        SetCapture(hWnd);
                    }
                    else
                    {
                        ReleaseCapture();
                    }
                } break;

                case WM_MOUSEMOVE:
                {
                    const int x = GET_X_LPARAM(lParam);
                    const int y = GET_Y_LPARAM(lParam);

                    window->mouse.lastX = x;
                    window->mouse.lastY = -y;
                } break;
            }
        }

        return DefWindowProcW(hWnd, message, wParam, lParam);
    }

    void OnSize(_NativeWindow* window, int width, int height)
    {
        window->width = width;
        window->height = height;

        if(window->win32.bitmapHandle)
        {
            DeleteObject(window->win32.bitmapHandle);
        }

        if(!window->win32.bitmapDeviceContext)
        {
            window->win32.bitmapDeviceContext = CreateCompatibleDC(nullptr);
        }

        BITMAPINFO bmi{};
        bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = height;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        window->win32.bitmapHandle = CreateDIBSection(
            window->win32.bitmapDeviceContext, &bmi,
            DIB_RGB_COLORS,
            &window->framebuffer,
            0, 0);
            
        if(window->win32.bitmapHandle)
        {
            SelectObject(window->win32.bitmapDeviceContext, window->win32.bitmapHandle);
        }
    }

    Key MapWParamToKey(WPARAM param)
    {
        switch(param)
        {
            case VK_UP:
                return Key::Up;
            case VK_DOWN:
                return Key::Down;
            case VK_RIGHT:
                return Key::Right;
            case VK_LEFT:
                return Key::Left;
            case VK_ESCAPE:
                return Key::Escape;
            default:
                return static_cast<Key>(param);
        }
    }
}
