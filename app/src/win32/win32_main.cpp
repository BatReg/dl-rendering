#include "win32_win.h"
#include <app/fps_counter.h>

#include <engine/window.h>
#include <engine/math/vec3.h>
#include <engine/math/sphere.h>
#include <engine/math/ray.h>

#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;
constexpr float MAX_FPS = 60.0;
constexpr float MAX_PERIOD = 1.0f / MAX_FPS;

static Engine::Window window = Engine::Window();
static std::chrono::steady_clock::time_point lastTime{};
static App::FpsCounter counter{};
static Engine::Math::Sphere sphere(Engine::Math::Point3(0, 0, -1), 0.5);

static void InitConsole();
static void Render();
static Engine::Math::Color RayColor(const Engine::Math::Ray& r);
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
    windowInitInfo.title = "Test";

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
            ss << "Test [FrameTime:" << std::to_string(counter.DeltaTime()) << " FPS:" << std::to_string(counter.FPS()) << "]";
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

        const float aspectRatio = float(width) / float(height);
        const float viewportHeight = 2.0f;
        const float viewportWidth = aspectRatio * viewportHeight;
        const float focalLength = 1.0f;

        const Engine::Math::Point3 origin(0, 0, 0);
        const Engine::Math::Vec3 horizontal(viewportWidth, 0, 0);
        const Engine::Math::Vec3 vertical(0, viewportHeight, 0);
        const Engine::Math::Vec3 lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - Engine::Math::Vec3(0, 0, focalLength);

        unsigned int* pixelData = reinterpret_cast<unsigned int*>(bitmapData);
        for (int j = 0; j < height; j++)
        {
            for (int i = 0; i < width; i++)
            {
                int pixelIdx = (i + j * width);

                float u = float(i) / (width - 1);
                float v = float(height - 1 - j) / (height - 1);

                Engine::Math::Ray r(lowerLeftCorner + u * horizontal + v * vertical, Engine::Math::Vec3(0, 0, -1)); // ortographic
                Engine::Math::Color c = RayColor(r);

                unsigned char ib = unsigned char(255.99 * c.B());
                unsigned char ig = unsigned char(255.99 * c.G());
                unsigned char ir = unsigned char(255.99 * c.R());

                unsigned int pixel = (0 << 24) | (ir << 16) | (ig << 8) | (ib);

                pixelData[pixelIdx] = pixel;
            }
        }
    }
}

Engine::Math::Color RayColor(const Engine::Math::Ray& r)
{
    if(HitSphere(sphere, r))
    {
        return Engine::Math::Color(1, 0, 0);
    }

    Engine::Math::Vec3 unitDirection = Engine::Math::UnitVector(r.Direction());
    float t = 0.5f * (unitDirection.Y() + 1.0f);

    return (1.0f - t) * Engine::Math::Color(1.0f, 1.0f, 1.0f) + t * Engine::Math::Color(0.5f, 0.7f, 1.0f);
}

bool HitSphere(const Engine::Math::Sphere& s, const Engine::Math::Ray& r)
{
    Engine::Math::Vec3 oc = r.Origin() - s.origin;
    float a = Engine::Math::Dot(r.Direction(), r.Direction());
    float b = 2.0f * Engine::Math::Dot(oc, r.Direction());
    float c = Engine::Math::Dot(oc, oc) - s.radius * s.radius;
    float discriminant = b * b - 4 * a * c;

    return discriminant > 0;
}

void ProcessInput(float deltaTime)
{
    const float speed = 1;
    Engine::Math::Vec3 offset{};

    short testingBit = 0x8000;

    if(GetAsyncKeyState(VK_LEFT) & testingBit) 
    {
        offset += Engine::Math::Vec3(-1, 0, 0);
    }

    if (GetAsyncKeyState(VK_RIGHT) & testingBit)
    {
        offset += Engine::Math::Vec3(1, 0, 0);
    }

    if (GetAsyncKeyState(VK_DOWN) & testingBit)
    {
        offset += Engine::Math::Vec3(0, -1, 0);
    }

    if (GetAsyncKeyState(VK_UP) & testingBit)
    {
        offset += Engine::Math::Vec3(0, 1, 0);
    }

    sphere.origin += offset * speed * deltaTime;
}
