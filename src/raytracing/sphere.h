//============================================================================
// @name        : sphere.h
// @author      : Thomas Dooms
// @date        : 4/5/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#pragma once

#include "hitable.h"
#include "../math/vec3.h"
#include "materials.h"

struct Sphere : public Hitable
{
    Sphere(const Vec3& centre, double radius, const Material* material) : centre(centre), radius(radius), material(material) {}

    bool intersect(const Ray &ray, double& dist) const override;
    HitInfo getInfo(const Ray &ray, double dist) const override;

    Vec3 centre;
    double radius;
    const Material* material;
};
