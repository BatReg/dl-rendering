#pragma once

#include <engine/window.h>
#include <engine/math/ray.h>
#include <engine/math/sphere.h>

namespace App
{
    class Controller
    {
    public:
        void Init(const Engine::Window& window);
        void Update(float deltaTime);
        void Destroy();

    private:
        void ProcessInput(float deltaTime);
        void Render();

        Eigen::Vector3f RayColor(const Engine::Math::Ray& r);
        bool HitSphere(const Engine::Math::Sphere& s, const Engine::Math::Ray& r);
    
    private:
        Engine::Window _window{};

        bool _isFirstMouse = true;
        long _lastX{};
        long _lastY{};

        Engine::Math::Sphere _sphere{};
    };
}
