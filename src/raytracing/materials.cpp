//============================================================================
// @name        : Materials.cpp
// @author      : Thomas Dooms
// @date        : 4/5/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "materials.h"
#include <cmath>

Vec3 reflect(const Vec3& dir, const Vec3& normal)
{
    return dir - 2*normal*dot(dir, normal);
}
// https://math.stackexchange.com/questions/44689/how-to-find-a-random-axis-or-unit-vector-in-3d
Vec3 randomVector()
{
    const double theta = 2*M_PI*drand48();
    const double z = 2*drand48()-1;
    const double temp = sqrt(1 - z*z);
    return {temp*cos(theta), temp*sin(theta), z};
}

void Lambertian::scatter(Ray &ray, const HitInfo &info) const
{
    ray.origin = info.intersection;
    ray.direction = normalize(info.normal + randomVector());
}

void Metal::scatter(Ray &ray, const HitInfo &info) const
{
    ray.origin = info.intersection;
    ray.direction = normalize(reflect(ray.direction, info.normal) + randomVector()*fuzzyness);
}