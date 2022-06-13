#pragma once

#include "i_intersactable.h"

#include <engine/math/ray.h>
#include <engine/math/sphere.h>

#include <Eigen/Dense>

namespace App::Objects
{
    struct Sphere final : public IIntersactable
    {
        Engine::Math::Sphere mathSphere;
        Eigen::Vector3f color;

        Sphere()
            : mathSphere(), color(1, 1, 1) {}

        Sphere(Eigen::Vector3f origin, float radius, Eigen::Vector3f color)
            : mathSphere(origin, radius), color(color) {}

        virtual bool Intersects(const Engine::Math::Ray& ray, Engine::Math::Intersection& outNearest, Eigen::Vector3f& outColor) const override;
    };
}