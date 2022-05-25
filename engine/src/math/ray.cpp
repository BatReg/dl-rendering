#include <engine/math/ray.h>

#include <Eigen/Dense>

namespace Engine::Math
{
    Ray::Ray() noexcept {}

    Ray::Ray(const Eigen::Vector3f& origin, const Eigen::Vector3f& direction) noexcept
        : orig(origin), dir(direction)
    {}

    Eigen::Vector3f Ray::Origin() const
    {
        return orig;
    }

    Eigen::Vector3f Ray::Direction() const
    {
        return dir;
    }

    Eigen::Vector3f Ray::At(float t) const
    {
        return orig + t * dir;
    }
}
