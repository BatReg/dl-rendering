#include "win32_win.h"
#include <app/fps_counter.h>

#include <engine/window.h>
#include <engine/math/vec3.h>

#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;
constexpr float MAX_FPS = 60.0;
constexpr float MAX_PERIOD = 1.0f / MAX_FPS;

static Engine::Window window = Engine::Window();
static std::chrono::steady_clock::time_point lastTime;
static App::FpsCounter counter{};

static void InitConsole();
static void Render();

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
)
{
    InitConsole();

    Engine::WindowCreateInfo windowInitInfo{};
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
            counter.Update(deltaTime);

            std::stringstream ss{};
            ss << "Test [FrameTime:" << std::to_string(counter.DeltaTime()) << " FPS:" << std::to_string(counter.FPS()) << "]";
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
        unsigned int* pixelData = reinterpret_cast<unsigned int*>(bitmapData);
        int width = window.GetWidth();
        int height = window.GetHeight();

        for (int j = 0; j < height; j++)
        {
            for (int i = 0; i < width; i++)
            {
                int pixelIdx = (i + j * width);

                Engine::Math::Vec3 c{float(i) / float(width), float(height - 1 - j) / float(height), 0.2f};

                unsigned char ib = unsigned char(255.99 * c.B());
                unsigned char ig = unsigned char(255.99 * c.G());
                unsigned char ir = unsigned char(255.99 * c.R());

                unsigned int pixel = (0 << 24) | (ir << 16) | (ig << 8) | (ib);

                pixelData[pixelIdx] = pixel;
            }
        }
    }
};
