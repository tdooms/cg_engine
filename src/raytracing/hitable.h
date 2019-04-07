//============================================================================
// @name        : hitable.h
// @author      : Thomas Dooms
// @date        : 4/5/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#pragma once

#include "hitInfo.h"
#include "ray.h"

struct Hitable
{
    virtual ~Hitable() = default;
    virtual bool intersect(const Ray& ray, double& dist) const = 0;
    virtual HitInfo getInfo(const Ray& ray, double dist) const = 0;
};