#include "win32_win.h"
#include "../fps_counter.h"

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
static Engine::Math::Sphere sphere(
    Eigen::Vector3f(static_cast<float>(WIDTH) / 2.0f, static_cast<float>(HEIGHT) / 2.0f, 0.0f), 
    250.0f
);

static bool isFirstMouse = true;
static long lastX{};
static long lastY{};

static void InitConsole();
static void Render();
static Eigen::Vector3f RayColor(const Engine::Math::Ray& r);
static bool HitSphere(const Engine::Math::Sphere& s, const Engine::Math::Ray& r);
static void ProcessInput(float deltaTime);

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
    windowInitInfo.title = TITLE;

    window.Init(windowInitInfo);

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

            ProcessInput(deltaTime);

            Render();

            window.SwapBuffers();
            window.PollEvents();

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
        const int width = window.GetWidth();
        const int height = window.GetHeight();

        unsigned int* pixelData = reinterpret_cast<unsigned int*>(bitmapData);
        for (int row = 0; row < height; row++)
        {
            for (int col = 0; col < width; col++)
            {
                int pixelIdx = (col + row * width);

                float u = float(col + 0.5f);
                float v = float(row + 0.5f);

                Engine::Math::Ray r(Eigen::Vector3f(u, v, 0.0f), Eigen::Vector3f(0.0f, 0.0f, 1.0f)); // ortographic
                Eigen::Vector3f c = RayColor(r);

                unsigned char ib = unsigned char(255.99 * c.z());
                unsigned char ig = unsigned char(255.99 * c.y());
                unsigned char ir = unsigned char(255.99 * c.x());

                unsigned int pixel = (0 << 24) | (ir << 16) | (ig << 8) | (ib);

                pixelData[pixelIdx] = pixel;
            }
        }
    }
}

Eigen::Vector3f RayColor(const Engine::Math::Ray& r)
{
    if(HitSphere(sphere, r))
    {
        return Eigen::Vector3f(1.0f, 0.0f, 0.0f);
    }

    return Eigen::Vector3f(0.1f, 0.1f, 0.1f);
}

bool HitSphere(const Engine::Math::Sphere& s, const Engine::Math::Ray& r)
{
    Eigen::Vector3f oc = r.Origin() - s.origin;
    float a = r.Direction().dot(r.Direction());
    float b = 2.0f * oc.dot(r.Direction());
    float c = oc.dot(oc) - s.radius * s.radius;
    float discriminant = b * b - 4 * a * c;

    return discriminant > 0;
}

void ProcessInput(float deltaTime)
{
    const short testingBit = short(0x8000);

    Eigen::Vector3f offset = Eigen::Vector3f::Zero();

    if (GetAsyncKeyState(VK_RBUTTON) & testingBit)
    {
        POINT p;
        GetCursorPos(&p);
        p.y = -p.y;

        if (isFirstMouse)
        {
            lastX = p.x;
            lastY = p.y;
            isFirstMouse = false;
        }

        const float xOffset = static_cast<float>(p.x - lastX);
        const float yOffset = static_cast<float>(p.y - lastY);
        lastX = p.x;
        lastY = p.y;

        const float sensitivity = 10.0f;
        offset = Eigen::Vector3f(1.0f, 0.0f, 0.0f) * xOffset + Eigen::Vector3f(0.0f, 1.0f, 0.0f) * yOffset;

        sphere.origin += offset * sensitivity * deltaTime;
    }
    else
    {
        isFirstMouse = true;

        const float speed = 100.0f;

        if (GetAsyncKeyState(VK_LEFT) & testingBit)
        {
            offset += Eigen::Vector3f(-1.0f, 0.0f, 0.0f);
        }

        if (GetAsyncKeyState(VK_RIGHT) & testingBit)
        {
            offset += Eigen::Vector3f(1.0f, 0.0f, 0.0f);
        }

        if (GetAsyncKeyState(VK_DOWN) & testingBit)
        {
            offset += Eigen::Vector3f(0.0f, -1.0f, 0.0f);
        }

        if (GetAsyncKeyState(VK_UP) & testingBit)
        {
            offset += Eigen::Vector3f(0.0f, 1.0f, 0.0f);
        }

        sphere.origin += offset * speed * deltaTime;
    }
}
