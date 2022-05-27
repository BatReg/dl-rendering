#include "controller.h"

#include <engine/math/sphere.h>
#include <Eigen/Dense>

#include <cstdint>
#include <iostream>
#include <utility>

namespace App
{
    void Controller::Init(const Engine::Window& window)
    {
        _window = window;

        float width = static_cast<float>(window.GetWidth());
        float height = static_cast<float>(window.GetHeight());
        _scene.spheres.push_back(Engine::Math::Sphere(Eigen::Vector3f(width / 2.0f, height / 2.0f, 1.0f), 250.0f));
    }

    void Controller::Update(float deltaTime)
    {
        ProcessInput(deltaTime);
        Render();
    }

    void Controller::Destroy()
    {

    }

    void Controller::Render()
    {
        void* bitmapData = _window.GetFramebuffer();

        if (bitmapData) 
        {
            const int width = _window.GetWidth();
            const int height = _window.GetHeight();

            uint32_t* pixelData = reinterpret_cast<uint32_t*>(bitmapData);
            for (int row = 0; row < height; row++)
            {
                for (int col = 0; col < width; col++)
                {
                    int pixelIdx = (col + row * width);

                    float u = float(col + 0.5f);
                    float v = float(row + 0.5f);

                    Engine::Math::Ray r(Eigen::Vector3f(u, v, 0.0f), Eigen::Vector3f(0.0f, 0.0f, 1.0f)); // ortographic
                    Eigen::Vector3f c = RayColor(r);

                    uint8_t ib = unsigned char(255.99 * c.z());
                    uint8_t ig = unsigned char(255.99 * c.y());
                    uint8_t ir = unsigned char(255.99 * c.x());

                    uint32_t pixel = (0 << 24) | (ir << 16) | (ig << 8) | (ib);

                    pixelData[pixelIdx] = pixel;
                }
            }
        }
    }

    Eigen::Vector3f Controller::RayColor(const Engine::Math::Ray& r)
    {
        if(HitSphere(_scene.spheres.front(), r))
        {
            return Eigen::Vector3f(1.0f, 0.0f, 0.0f);
        }

        return Eigen::Vector3f(0.1f, 0.1f, 0.1f);
    }

    bool Controller::HitSphere(const Engine::Math::Sphere& s, const Engine::Math::Ray& r)
    {
        Eigen::Vector3f oc = r.Origin() - s.origin;
        float a = r.Direction().dot(r.Direction());
        float b = 2.0f * oc.dot(r.Direction());
        float c = oc.dot(oc) - s.radius * s.radius;
        float discriminant = b * b - 4 * a * c;

        return discriminant > 0;
    }

    void Controller::ProcessInput(float deltaTime)
    {
        if(_window.IsKeyDown(Engine::Key::Escape))
        {
            _window.RequestQuit();
        }

        const short testingBit = short(0x8000);
        Eigen::Vector3f offset = Eigen::Vector3f::Zero();

        if (_window.IsMouseButtonDown(Engine::MouseButton::Right))
        {
            std::pair<int, int> p = _window.GetMousePos();
            int x = p.first;
            int y = p.second;

            if (_isFirstMouse)
            {
                _lastX = x;
                _lastY = y;
                _isFirstMouse = false;
            }

            const float xOffset = static_cast<float>(x - _lastX);
            const float yOffset = static_cast<float>(y - _lastY);
            _lastX = x;
            _lastY = y;

            const float sensitivity = 10.0f;
            offset = Eigen::Vector3f(1.0f, 0.0f, 0.0f) * xOffset + Eigen::Vector3f(0.0f, 1.0f, 0.0f) * yOffset;

            _scene.spheres.front().origin += offset * sensitivity * deltaTime;
        }
        else
        {
            _isFirstMouse = true;

            const float speed = 100.0f;

            if (_window.IsKeyDown(Engine::Key::Left))
            {
                offset += Eigen::Vector3f(-1.0f, 0.0f, 0.0f);
            }

            if (_window.IsKeyDown(Engine::Key::Right))
            {
                offset += Eigen::Vector3f(1.0f, 0.0f, 0.0f);
            }

            if (_window.IsKeyDown(Engine::Key::Down))
            {
                offset += Eigen::Vector3f(0.0f, -1.0f, 0.0f);
            }

            if (_window.IsKeyDown(Engine::Key::Up))
            {
                offset += Eigen::Vector3f(0.0f, 1.0f, 0.0f);
            }

            _scene.spheres.front().origin += offset * speed * deltaTime;
        }
    }
}
