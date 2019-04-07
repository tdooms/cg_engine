//============================================================================
// @name        : hitInfo.h
// @author      : Thomas Dooms
// @date        : 4/5/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#pragma once

#include "../math/vec3.h"

struct Material;

struct HitInfo
{
    Vec3 normal;
    Vec3 intersection;
    const Material* material;
};
