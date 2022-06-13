#pragma once

#include <Eigen/Dense>

namespace Engine::Math
{
    struct Intersection
    {
        float t;
        Eigen::Vector3f pos;
        Eigen::Vector3f normal;

        Intersection() = default;

        void Reset() { t = std::numeric_limits<float>::infinity(); }
        bool Exists() const { return std::isfinite(t); }

        static Intersection Infinite() { Intersection i; i.Reset(); return i; }
    };

    struct Ray
    {
        Ray() noexcept;
        Ray(const Eigen::Vector3f& origin, const Eigen::Vector3f& direction) noexcept;

        Eigen::Vector3f Origin() const;
        Eigen::Vector3f Direction() const;
        Eigen::Vector3f At(float t) const;

        Eigen::Vector3f orig;
        Eigen::Vector3f dir;
    };
}
