//============================================================================
// @name        : camera.cpp
// @author      : Thomas Dooms
// @date        : 4/6/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "camera.h"

Ray Camera::generate(uint32_t x, uint32_t y) const
{
    Vec3 onScreen = {(2 * x + drand48()) / height - aspectRatio, (2 * y + drand48()) / height - 1, 1};
    return {pos, normalize(onScreen - pos)};
}

Vec3 Camera::traceRay(Ray ray, const std::vector<Hitable*>& world, uint32_t depth)
{
    const double maxDist = 1000;

    if(depth > maxDepth)
        return ambient;

    std::pair<double, const Hitable*> closest = {maxDist, nullptr};
    double dist;
    for(const auto& item : world)
    {
        if(item->intersect(ray, dist) && dist < closest.first)
        {
            closest = {dist, item};
        }
    }
    if(closest.second == nullptr) return ambient;

    HitInfo info = closest.second->getInfo(ray, closest.first);
    info.material->scatter(ray, info);

    auto next = traceRay(ray, world, depth+1);
    return info.material->emittance + absorption*dot(ray.direction, info.normal) * (info.material->reflectance ^ next);
}

void Camera::traceFilm(const std::vector<Hitable*>& world)
{
    numSamples++;
    for(uint32_t x = 0; x < width; x++)
        for(uint32_t y = 0; y < height; y++)
        {
            Vec3 color = traceRay(generate(x,y), world, 0);
            film[x * height + y] += color;
        }
}

img::EasyImage Camera::exportFilm() const
{
    img::EasyImage image(width, height);
    const double correction = 255.99 / numSamples;
    auto convert = [correction](double color){ return static_cast<uint8_t>(std::min(255.0, color * correction)); };

    for(uint32_t i = 0; i < film.size(); i++)
        image(i) = {convert(film[i][0]), convert(film[i][1]), convert(film[i][2])};

    return image;
}