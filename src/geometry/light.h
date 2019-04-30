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


class Light
{
public:
    Light(const Color& ambient, const Color& diffuse, const Color& specular, const Vec3& vector = {}) : ambient(ambient), diffuse(diffuse), specular(specular), vector(vector) {}

    Color getAmbient()   const { return ambient;  };
    Color getDiffuse()   const { return diffuse;  };
    Color getSpecular()  const { return specular; };
    Color getDirection() const { return vector;   };
    Color getPosition()  const { return vector;   };

private:
    Color ambient;
    Color diffuse;
    Color specular;
    Vec3 vector;
};


#endif //ENGINE_LIGHT_H
