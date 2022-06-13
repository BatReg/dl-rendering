#include <engine/math/sphere.h>

#include <Eigen/Dense>

namespace Engine::Math
{
    bool Sphere::Intersects(const Ray& ray, Intersection& outNearest) const
    {
        Eigen::Vector3f oc = ray.Origin() - origin;
        float a = ray.Direction().squaredNorm();
        float half_b = oc.dot(ray.Direction());
        float c = oc.squaredNorm() - radius * radius;
        float discriminant = half_b * half_b - a * c;

        float x;
        if(discriminant < 0)
        {
            return false;
        }
        else if (discriminant == 0)
        {
            x = (-half_b) / a;
        }
        else
        {
            x = (-half_b - sqrtf(discriminant)) / a;
        }
        

        if(x < 0 || x > outNearest.t)
        {
            return false;
        }

        outNearest.t = x;
        return true;
    }
}
