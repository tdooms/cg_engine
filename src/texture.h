//============================================================================
// @name        : texture->h
// @author      : Thomas Dooms
// @date        : 5/10/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#ifndef ENGINE_TEXTURE_H
#define ENGINE_TEXTURE_H

#include <fstream>
#include <limits>
#include "easy_image.h"
#include "math/mat4.h"
#include "ini_configuration.h"
#include "math/color.h"

struct Texture
{
    Texture static parseTexture(const ini::Section& section)
    {
        Texture result;
        std::ifstream stream(section["path"]);
        stream >> result.texture;
        result.p = section["p"].as_double_tuple_or_die();

        Vec3 a = section["a"].as_double_tuple_or_die();
        Vec3 b = section["b"].as_double_tuple_or_die();
        Vec3 c = cross(a,b);
        Mat4 temp = {{a[0], a[1], a[2], 0}, {b[0], b[1], b[2], 0}, {c[0], c[1], c[2], 0}, {0, 0, 0, 1}};
        result.invProj = Mat4::inverse(temp);
        return result;
    }

    Mesh::Material static getColor(const Texture* texture, const Vec3& pos)
    {
        Vec3 uvw = pos - texture->p;
        uvw *= texture->invProj;

        if(uvw[0] < 0 or uvw[0] > 1 or uvw[1] < 0 or uvw[1] > 1) return {texture->empty, texture->empty, texture->empty, 0};

        img::Color temp = texture->texture(static_cast<uint32_t>(uvw[0]*texture->texture.get_width()), static_cast<uint32_t>(uvw[1]*texture->texture.get_height()));
        Vec3 color = {temp.red/255.99, temp.green/255.99, temp.blue/255.99};
        return {color, color, color, 0};
    }

    img::EasyImage texture;
    Vec3 p;
    Mat4 invProj;
    Vec3 empty = {std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};
};

#endif //ENGINE_TEXTURE_H
