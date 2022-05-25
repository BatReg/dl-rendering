#pragma once

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

        Sphere(const Eigen::Vector3f& o, float r)
            : origin(o), radius(r)
        {}
    };
}
