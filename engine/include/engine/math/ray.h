#pragma once

#include <Eigen/Dense>

namespace Engine::Math
{
    class Ray
    {
    public:
        Ray() noexcept;
        Ray(const Eigen::Vector3f& origin, const Eigen::Vector3f& direction) noexcept;

        Eigen::Vector3f Origin() const;
        Eigen::Vector3f Direction() const;
        Eigen::Vector3f At(float t) const;

    public:
        Eigen::Vector3f orig;
        Eigen::Vector3f dir;
    };  
}

