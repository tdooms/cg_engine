//============================================================================
// @name        : light.h
// @author      : Thomas Dooms
// @date        : 4/26/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#ifndef ENGINE_LIGHT_H
#define ENGINE_LIGHT_H

#include "../math/color.h"

struct Light
{
    Light(const Color& ambient, const Color& diffuse, const Color& specular, const Vec3& vector = {}, const Mat4& eye = {}, const ZBuffer& shadowMask = {0,0}, std::array<double, 3> dValues = {}, bool shadow = false) :
    light{ambient, diffuse, specular}, vector(vector), eye(eye), shadowMask(shadowMask), dValues(dValues), shadow(shadow) {}

    std::array<Vec3, 3> light;
    Vec3 vector;

    // needed for shadows
    Mat4 eye;
    ZBuffer shadowMask;
    std::array<double, 3> dValues; // d, dx, dy
    bool shadow;
};

#endif //ENGINE_LIGHT_H
