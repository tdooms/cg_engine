//============================================================================
// @name        : ray.h
// @author      : Thomas Dooms
// @date        : 4/5/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#pragma once

#include "../math/vec3.h"

struct Ray
{
    Ray(Vec3 origin, Vec3 direction) : origin(origin), direction(direction) {}
    Vec3 operator()(double dist) const { return origin + direction * dist; }

    Vec3 origin;
    Vec3 direction;
};
