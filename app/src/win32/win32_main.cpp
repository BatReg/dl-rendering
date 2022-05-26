#include "win32_win.h"
#include "../fps_counter.h"
#include "../controller.h"

#include <engine/window.h>
#include <engine/math/sphere.h>
#include <engine/math/ray.h>
#include <Eigen/Dense>

#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>


constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;
constexpr float MAX_FPS = 60.0f;
constexpr float MAX_PERIOD = 1.0f / MAX_FPS;
constexpr char* TITLE = "Renderer";

static Engine::Window window = Engine::Window();
static std::chrono::steady_clock::time_point lastTime{};
static App::FpsCounter counter{};
static App::Controller controller{};

static void InitConsole();

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
)
{
    InitConsole();

    Engine::WindowCreateInfo windowInitInfo{};
    windowInitInfo.width = WIDTH;
    windowInitInfo.height = HEIGHT;
    windowInitInfo.title = TITLE;

    window.Init(windowInitInfo);
    controller.Init(window);

    lastTime = std::chrono::high_resolution_clock::now();
    while(!window.ShouldQuit())
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();

        if(deltaTime > MAX_PERIOD)
        {
            counter.Update(deltaTime);

            std::stringstream ss{};
            ss << TITLE << " [FrameTime:" << std::to_string(counter.DeltaTime()) << " FPS:" << std::to_string(counter.FPS()) << "]";
            window.SetTitle(ss.str());

            controller.Update(deltaTime);

            window.SwapBuffers();
            window.PollEvents();

            lastTime = currentTime;
        }

        std::this_thread::yield();
    }

    controller.Destroy();

    return 0;
}

void InitConsole()
{    
    AllocConsole();
    FILE* dummy;
    auto s = freopen_s(&dummy, "CONOUT$", "w", stdout);
}
