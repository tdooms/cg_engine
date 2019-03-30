//============================================================================
// @name        : mesh.h
// @author      : Thomas Dooms
// @date        : 3/8/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#ifndef ENGINE_MESH_H
#define ENGINE_MESH_H

#include <vector>
#include "../math/vec3.h"
#include "../math/color.h"
#include "../math/mat4.h"

#include "../ini_configuration.h"
#include "../easy_image.h"


struct Mesh
{
    Mesh() = default;
    Mesh(const std::vector<Vec3>& vertices, const std::vector<std::vector<uint32_t>>& indices, const Color& color)
    : vertices(vertices), indices(indices), color(color) {}

    Mesh& operator*=(const Mat4& transform);

    static Mesh parseFigure(const ini::Section& section, const Mat4& eye);
    static Mesh createLineDrawing(const Color& color, const ini::Section& section);

    static Mesh createCube(const Color& color);
    static Mesh createTetrahedron(const Color& color);
    static Mesh createOctahedron(const Color& color);
    static Mesh createIcosahedron(const Color& color);
    static Mesh createDodecahedron(const Color& color);

    static Mesh createCylinder(const Color& color, double height, uint32_t num);
    static Mesh createCone(const Color& color, double height, uint32_t num);
    static Mesh createSphere(const Color& color, uint32_t depth);
    static Mesh createTorus(const Color& color, double R, double r, uint32_t n, uint32_t m);
    static Mesh createSierpinskiSphere(const Color& color, uint32_t depth);

    static void subdivTriangle(std::vector<Vec3>& vertices, std::vector<std::vector<uint32_t>>& indices, uint32_t face);
    static void subdivSierpinskiTriangle(std::vector<Vec3>& vertices, std::vector<std::vector<uint32_t>>& indices, uint32_t face);

    static Mesh triangulate(const Mesh& mesh);

    std::vector<Vec3> vertices;
    std::vector<std::vector<uint32_t>> indices;

    Color color;
};

#endif //ENGINE_MESH_H
