#include <engine/window.h>

#include "win32_core.h"

#include <iostream>
#include <string>

namespace Engine
{
    static constexpr wchar_t* WINDOW_CLASS_NAME = L"RendererWindow";

    class Window::Impl
    {
    public:
        bool shouldQuit = false;
        int _height = 0;
        int _width = 0;
        void* _framebuffer{};

    public:
        bool Init(const WindowInitInfo& info)   
        {
            _height = info.height;
            _width = info.width, 
            _title = info.title;

            WNDCLASSEXW wc{};
            wc.cbSize = sizeof(wc);
            wc.lpfnWndProc = ProxyWindowProc;
            wc.lpszClassName = WINDOW_CLASS_NAME;

            if(!RegisterClassExW(&wc))
            {
                return false;
            }

            std::wstring title(_title.begin(), _title.end());

            RECT rect{ 0, 0, _width, _height };
            AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

            _hWnd = CreateWindowExW(
                NULL,
                WINDOW_CLASS_NAME,
                title.c_str(),
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left , rect.bottom - rect.top,
                NULL, NULL,
                GetModuleHandleW(NULL),
                NULL
            );

            if(!_hWnd)
            {
                return false;
            }

            SetWindowLongPtrW(_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
            ShowWindow(_hWnd, true);

            _isInitialized = true;
            return true;
        }

        void PollEvents()
        {
            MSG msg;
            while(PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
            {
                if(msg.message == WM_QUIT)
                {
                    shouldQuit = true;
                }

                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        void SwapBuffers()
        {
            RECT r;
            GetClientRect(_hWnd, &r);
            HDC hdc = GetDC(_hWnd);

            BitBlt(
                hdc,
                0, 0, r.right - r.left, r.bottom - r.top,
                _bitmapDeviceContext,
                0, 0,
                SRCCOPY
            );

            DeleteDC(hdc);
        }

        void SetTitle(std::string title)
        {
            std::wstring wTitle(title.begin(), title.end());
            SetWindowTextW(_hWnd, wTitle.c_str());
        }

    private:
        bool _isInitialized = false;
        std::string _title{};

        HWND _hWnd{};
        HDC _windowDeviceContext{};
        HDC _bitmapDeviceContext{};
        HBITMAP _bitmapHandle{};

    private:
        static LRESULT CALLBACK ProxyWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
        {
            Window::Impl* window = reinterpret_cast<Window::Impl*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
            if (window)
            {
                return window->WindowProc(hWnd, message, wParam, lParam);
            }
            else
            {
                return DefWindowProcW(hWnd, message, wParam, lParam);
            }
        }

        LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
        {
            switch (message)
            {
            case WM_SIZE:
            {
                std::cout << "WM_SIZE" << std::endl;

                RECT r;
                GetClientRect(hWnd, &r);
                OnSize(r.right - r.left, r.bottom - r.top);
            } break;

            case WM_DESTROY:
            {
                std::cout << "WM_DESTROY" << std::endl;

                PostQuitMessage(0);
            } break;
            }

            return DefWindowProcW(hWnd, message, wParam, lParam);
        }

        void OnSize(int width, int height)
        {
            _width = width;
            _height = height;

            if(_bitmapHandle)
            {
                DeleteObject(_bitmapHandle);
            }

            if(!_bitmapDeviceContext)
            {
                _bitmapDeviceContext = CreateCompatibleDC(nullptr);
            }

            BITMAPINFO bmi{};
            bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
            bmi.bmiHeader.biWidth = width;
            bmi.bmiHeader.biHeight = -height;
            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biBitCount = 32;
            bmi.bmiHeader.biCompression = BI_RGB;

            _bitmapHandle = CreateDIBSection(
                _bitmapDeviceContext, &bmi,
                DIB_RGB_COLORS,
                &_framebuffer,
                0, 0);
            
            if(_bitmapHandle)
            {
                SelectObject(_bitmapDeviceContext, _bitmapHandle);
            }
        }
    };

    Window::~Window() = default;

    Window::Window() noexcept
        : _pimpl(new Impl)
    {
    }

    bool Window::Init(const WindowInitInfo& info)
    {
        return _pimpl->Init(info);
    }

    void Window::PollEvents()
    {
        _pimpl->PollEvents();
    }

    void Window::SwapBuffers()
    {
        _pimpl->SwapBuffers();
    }

    bool Window::ShouldQuit()
    {
        return _pimpl->shouldQuit;
    }

    void* Window::GetFramebuffer()
    {
        return _pimpl->_framebuffer;
    }

    int Window::GetWidth()
    {
        return _pimpl->_width;
    }

    int Window::GetHeight()
    {
        return _pimpl->_height;
    }

    void Window::SetTitle(std::string title)
    {
        _pimpl->SetTitle(title);
    }
}
