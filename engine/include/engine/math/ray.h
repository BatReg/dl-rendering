#pragma once

#include <engine/math/vec3.h>

namespace Engine::Math
{
    class Ray
    {
    public:
        Ray() noexcept;
        Ray(const Point3& origin, const Vec3& direction) noexcept;

        Point3 Origin() const;
        Vec3 Direction() const;
        Point3 At(float t) const;

    public:
        Point3 orig;
        Vec3 dir;
    };  
}

