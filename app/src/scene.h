#pragma once

#include "objects/sphere.h"

#include <vector>

namespace App
{
    struct Scene
    {
        std::vector<App::Objects::Sphere> spheres{};
    };
}
