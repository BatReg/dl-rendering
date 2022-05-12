#include "win32_win.h"

#include <engine/test.h>

#include <iostream>
#include <chrono>
#include <sstream>
#include <thread>

static void InitConsole();
static HWND InitWindow(HINSTANCE hInstance);
static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
static void ResizeFramebuffer(HWND hWnd, int width, int height);
static void Render();

constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;
constexpr float MAX_FPS = 60.0;
constexpr float MAX_PERIOD = 1.0f / MAX_FPS;

static HWND hwnd;
static HDC windowDeviceContext;
static HDC bitmapDeviceContext;
static HBITMAP bitmapHandle;
static void* bitmapData;
static bool isRed = true;

static std::chrono::steady_clock::time_point lastTime;

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
)
{
    InitConsole();
    Engine::Test("Test");

    HWND hwnd = InitWindow(hInstance);
    ShowWindow(hwnd, nShowCmd);

    lastTime = std::chrono::high_resolution_clock::now();

    MSG msg;
    while(true)
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();

        bool isQuitPosted = false;
        while(PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                isQuitPosted = true;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if(isQuitPosted)
        {
            break;
        }

        if (deltaTime > MAX_PERIOD) 
        {
            std::wstringstream ss{};
            ss << "Test [FrameTime:" << std::to_wstring(deltaTime) << " FPS:" << std::to_wstring(1.0f / deltaTime) << "]";
            SetWindowText(hwnd, ss.str().c_str());

            Render();

            lastTime = currentTime;
        }

        std::this_thread::yield();
    }

    return 0;
}

void InitConsole()
{    
    AllocConsole();
    FILE* dummy;
    auto s = freopen_s(&dummy, "CONOUT$", "w", stdout);
}

HWND InitWindow(HINSTANCE hInstance)
{
    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"RendererWindow";

    RegisterClassExW(&wc);

    hwnd = CreateWindowExW(
        NULL,
        L"RendererWindow",
        L"Test",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT,
        NULL,NULL,
        hInstance,
        NULL);

    return hwnd;
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
            ResizeFramebuffer(hWnd, r.right - r.left, r.bottom - r.top);            
        } break;
        case WM_PAINT:
        {
            std::cout << "WM_PAINT" << std::endl;
            Render();
        } break;
        case WM_KEYUP:
        {
            if (wParam == VK_SPACE)
            {
                isRed = !isRed;
            }
        } break;
        case WM_DESTROY:
        {
            PostQuitMessage(0);
        } break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

void ResizeFramebuffer(HWND hWnd, int width, int height)
{
    if (bitmapHandle)
    {
        DeleteObject(bitmapHandle);
    }

    if (!bitmapDeviceContext)
    {
        bitmapDeviceContext = CreateCompatibleDC(nullptr);
    }

    BITMAPINFO bmi{};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    bitmapHandle = CreateDIBSection(
        bitmapDeviceContext, &bmi,
        DIB_RGB_COLORS,
        &bitmapData,
        0, 0);

    if (bitmapHandle)
    {
        SelectObject(bitmapDeviceContext, bitmapHandle);
    }
}

void Render()
{
    RECT r;
    GetClientRect(hwnd, &r);

    char* pixelData = reinterpret_cast<char*>(bitmapData);
    int width = r.right - r.left;
    int height = r.bottom - r.top;
    for (int i = 0; i < width * height * 4; i += 4)
    {
        pixelData[i] = !isRed ? 255 : 0;
        pixelData[i + 2] = isRed ? 255 : 0;
    }

    HDC hdc = GetDC(hwnd);
    BitBlt(hdc, 0, 0, r.right - r.left, r.bottom - r.top, bitmapDeviceContext, 0, 0, SRCCOPY);
    DeleteDC(hdc);
};
