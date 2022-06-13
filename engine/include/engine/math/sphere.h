#pragma once

#include "ray.h"

#include <Eigen/Dense>

namespace Engine::Math
{
    struct Sphere
    {
        Eigen::Vector3f origin{};
        float radius{};

        Sphere() noexcept
            : origin(), radius(0)
        {}

        Sphere(const Eigen::Vector3f& o, float r) noexcept
            : origin(o), radius(r)
        {}

        Sphere(const Sphere& s) noexcept
            : origin(s.origin), radius(s.radius)
        {}

        bool Intersects(const Ray& ray, Intersection& outNearest) const;
    };
}
