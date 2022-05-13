#include "../int_window.h"
#include "win32_core.h"

#include <iostream>
#include <string>

namespace Engine::Low::Internal
{
    static constexpr wchar_t* WINDOW_CLASS_NAME = L"RendererWindow";

    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static void OnSize(_NativeWindow* window, int width, int height);

    bool _WindowCreate(_NativeWindow* window)
    {
        WNDCLASSEXW wc{};
        wc.cbSize = sizeof(wc);
        wc.lpfnWndProc = WindowProc;
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

        return true;
    }

    void _WindowPollEvents(_NativeWindow* window)
    {
        MSG msg;
        while(PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if(msg.message == WM_QUIT)
            {
                std::cout << "WM_QUIT" << std::endl;
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
        bmi.bmiHeader.biHeight = -height;
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
}
