#include <engine/math/vec3.h>

#include <cmath>

namespace Engine::Math
{
    Vec3::Vec3() noexcept : e{0, 0, 0} {}
    Vec3::Vec3(float e0, float e1, float e2) noexcept : e{e0, e1, e2} {}

    float Vec3::X() const
    {
        return e[0];
    }

    float Vec3::Y() const
    {
        return e[1];
    }

    float Vec3::Z() const
    {
        return e[2];
    }

    float Vec3::R() const
    {
        return e[0];
    }

    float Vec3::G() const
    {
        return e[1];
    }

    float Vec3::B() const
    {
        return e[2];
    }

    Vec3 Vec3::operator~() const
    {
        return Vec3(-e[0], -e[1], -e[2]);
    }

    float Vec3::operator[](int i) const
    {
        return e[i];
    }

    float& Vec3::operator[](int i)
    {
        return e[i];
    }

    Vec3& Vec3::operator+=(const Vec3& v)
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];

        return *this;
    }

    Vec3& Vec3::operator*=(const float t)
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;

        return *this;
    }

    Vec3& Vec3::operator/=(const float t)
    {
        return *this *= 1/t;
    }

    float Vec3::Length() const
    {
        return sqrt(LengthSquared());
    }

    float Vec3::LengthSquared() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }
}
