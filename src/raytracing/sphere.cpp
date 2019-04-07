//============================================================================
// @name        : sphere.cpp
// @author      : Thomas Dooms
// @date        : 4/5/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include <cmath>
#include "sphere.h"

bool Sphere::intersect(const Ray& ray, double& dist) const
{
    Vec3 oc = ray.origin - centre;
    double b = dot(oc, ray.direction);
    double c = norm(oc) - radius * radius;
    double discriminant = b * b - c;

    if(discriminant < 0) return false;

    discriminant = sqrt(discriminant);
    double temp = -b - discriminant;
    if(temp > 0.01)
    {
        dist = temp;
        return true;
    }
    temp = -b + discriminant;
    if(temp > 0.01)
    {
        dist = temp;
        return true;
    }
    return false;
}

HitInfo Sphere::getInfo(const Ray& ray, double dist) const
{
    const Vec3 intersection = ray(dist);
    return {(intersection - centre)/radius, intersection, material};
}