#pragma once

#include <engine/math/sphere.h>

#include <list>

namespace App
{
    struct Scene
    {
        std::list<Engine::Math::Sphere> spheres{};
    };
}
