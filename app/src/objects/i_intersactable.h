#pragma once

#include <engine/math/ray.h>

#include <Eigen/Dense>

namespace App::Objects
{
    struct IIntersactable
    {
        virtual ~IIntersactable() {}
        virtual bool Intersects(const Engine::Math::Ray& ray, Engine::Math::Intersection& outNearest, Eigen::Vector3f& outColor) const = 0;
    };
}
