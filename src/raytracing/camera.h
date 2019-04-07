//============================================================================
// @name        : camera.h
// @author      : Thomas Dooms
// @date        : 4/5/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#pragma once

#include "ray.h"
#include "../math/vec3.h"
#include "../easy_image.h"
#include "hitable.h"
#include "materials.h"

class Camera
{
public:
    Camera(uint32_t width, uint32_t height, Vec3 ambient = {}, double absorption = 0.5, uint32_t maxDepth = 2) :
    width(width), height(height), aspectRatio(double(width)/double(height)), film(width*height), ambient(ambient), absorption(absorption), maxDepth(maxDepth) {}

    Ray generate(uint32_t x, uint32_t y) const;

    Vec3 traceRay(Ray ray, const std::vector<Hitable*>& world, uint32_t depth);
    void traceFilm(const std::vector<Hitable*>& world);
    img::EasyImage exportFilm() const;

private:
    uint32_t width;
    uint32_t height;
    double aspectRatio;

    std::vector<Vec3> film;
    Vec3 ambient;
    uint32_t numSamples = 0;

    double absorption;
    uint32_t maxDepth;

    const Vec3 pos = {0, 0, 0};
};


