#include "win32_win.h"

#include <engine/test.h>

#include <iostream>

static void InitConsole();
static HWND InitWindow(HINSTANCE hInstance);
static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

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

    MSG msg;
    while(true)
    {
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

    HWND hwnd = CreateWindowExW(NULL,
                                L"RendererWindow",
                                L"Test",
                                WS_OVERLAPPEDWINDOW,
                                CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,
                                NULL,
                                NULL,
                                hInstance,
                                NULL);

    return hwnd;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        } break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}
