#pragma once

#include <engine/math/sphere.h>

#include <Eigen/Dense>

namespace App::Objects
{
	struct Sphere
	{
	public:

		Sphere() = default;
		Sphere(Engine::Math::Sphere s, Eigen::Vector3f c) noexcept
			: sphere(s), color(c)
		{}

	public:
		Engine::Math::Sphere sphere = Engine::Math::Sphere();
		Eigen::Vector3f color = Eigen::Vector3f::Ones();
	};
}