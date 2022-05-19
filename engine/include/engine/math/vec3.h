#pragma once

#include <iostream>

namespace Engine::Math
{
    class Vec3
    {
    public:
        Vec3() noexcept;
        Vec3(float e0, float e1, float e2) noexcept;

        float X() const;
        float Y() const;
        float Z() const;

        float R() const;
        float G() const;
        float B() const;

        Vec3 operator~() const;
        float operator[](int i) const;
        float& operator[](int i);

        Vec3& operator+=(const Vec3& v);
        Vec3& operator*=(const float t);
        Vec3& operator/=(const float t);

        float Length() const;
        float LengthSquared() const;

    public:
        float e[3];
    };

    using Point3 = Vec3;
    using Color = Vec3;

    inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
    {
        return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
    }

    inline Vec3 operator+(const Vec3& u, const Vec3& v)
    {
        return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
    }

    inline Vec3 operator-(const Vec3& u, const Vec3& v)
    {
        return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
    }

    inline Vec3 operator*(const Vec3& u, const Vec3& v)
    {
        return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
    }

    inline Vec3 operator*(const float t, const Vec3& v)
    {
        return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
    }

    inline Vec3 operator*(const Vec3& v, const float t)
    {
        return t * v;
    }

    inline Vec3 operator/(const Vec3& v, const float t)
    {
        return (1 / t) * v;
    }

    inline float Dot(const Vec3& u, const Vec3& v)
    {
        return u.e[0] * v.e[0] 
             + u.e[1] * v.e[1]
             + u.e[2] * v.e[2];
    }

    inline Vec3 Cross(const Vec3& u, const Vec3& v)
    {
        return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                    u.e[2] * v.e[0] - u.e[0] * v.e[2],
                    u.e[0] * v.e[1] - u.e[1] * v.e[0]);
    }

    inline Vec3 UnitVector(const Vec3& v)
    {
        return v / v.Length();
    }
}
