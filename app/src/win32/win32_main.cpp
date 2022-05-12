#include "win32_win.h"

#include <engine/window.h>

#include <chrono>
#include <sstream>
#include <thread>

static void InitConsole();
static void Render();

constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;
constexpr float MAX_FPS = 60.0;
constexpr float MAX_PERIOD = 1.0f / MAX_FPS;

static Engine::Window window = Engine::Window();
static std::chrono::steady_clock::time_point lastTime;

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
)
{
    InitConsole();

    Engine::WindowInitInfo windowInitInfo{};
    windowInitInfo.width = 1280;
    windowInitInfo.height = 720;
    windowInitInfo.title = "Test";

    window.Init(windowInitInfo);

    lastTime = std::chrono::high_resolution_clock::now();
    while(!window.ShouldQuit())
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();

        window.PollEvents();

        if(deltaTime > MAX_PERIOD)
        {
            std::stringstream ss{};
            ss << "Test [FrameTime:" << std::to_string(deltaTime) << " FPS:" << std::to_string(1.0f / deltaTime) << "]";
            window.SetTitle(ss.str());

            Render();
            window.SwapBuffers();

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

void Render()
{
    void* bitmapData = window.GetFramebuffer();

    if (bitmapData) 
    {
        char* pixelData = reinterpret_cast<char*>(bitmapData);
        int width = window.GetWidth();
        int height = window.GetHeight();
        for (int i = 0; i < width * height * 4; i += 4)
        {
            pixelData[i] = char(255);
            pixelData[i + 2] = char(255);
        }
    }
};
