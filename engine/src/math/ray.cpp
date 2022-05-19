#include <engine/math/ray.h>

namespace Engine::Math
{
    Ray::Ray(){}

    Ray::Ray(const Point3& origin, const Vec3& direction)
        : orig(origin), dir(direction)
    {}

    Point3 Ray::Origin() const
    {
        return orig;
    }

    Vec3 Ray::Direction() const
    {
        return dir;
    }

    Point3 Ray::At(float t) const
    {
        return orig + t * dir;
    }
}
