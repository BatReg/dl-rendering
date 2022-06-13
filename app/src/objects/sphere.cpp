#include "sphere.h"

namespace App::Objects
{
    bool Sphere::Intersects(const Engine::Math::Ray &ray, Engine::Math::Intersection &outNearest, Eigen::Vector3f &outColor) const
    {
        bool result = mathSphere.Intersects(ray, outNearest);

        if (result)
        {
            outColor = color;
        }

        return result;
    }
}
