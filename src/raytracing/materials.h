//============================================================================
// @name        : Materials.h
// @author      : Thomas Dooms
// @date        : 4/5/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#pragma once

#include "../math/vec3.h"
#include "ray.h"
#include "hitInfo.h"

struct Material
{
    virtual ~Material() = default;
    Material(const Vec3& emittance, const Vec3& reflectance) : emittance(emittance), reflectance(reflectance) {}
    virtual void scatter(Ray& ray, const HitInfo& info) const = 0;

    Vec3 emittance;
    Vec3 reflectance;
};

struct Lambertian : public Material
{
    Lambertian(const Vec3& emittance, const Vec3& reflectance) : Material(emittance, reflectance) {}
    void scatter(Ray& ray, const HitInfo& info) const override;
};

struct Metal : public Material
{
    Metal(const Vec3& emittance, const Vec3& reflectance, double fuzzyness) : Material(emittance, reflectance), fuzzyness(fuzzyness) {}
    void scatter(Ray& ray, const HitInfo& info) const override;

    double fuzzyness;
};
