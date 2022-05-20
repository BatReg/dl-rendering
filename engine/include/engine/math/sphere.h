#pragma once

#include <engine/math/vec3.h>

namespace Engine::Math
{
    struct Sphere
    {
        Point3 origin{};
        float radius{};

        Sphere() noexcept
            : origin(), radius(0)
        {}

        Sphere(const Point3& o, float r)
            : origin(o), radius(r)
        {}
    };
}
