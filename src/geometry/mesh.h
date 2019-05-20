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
#include <set>
#include "../math/vec3.h"
#include "../math/color.h"
#include "../math/mat4.h"

#include "../ini_configuration.h"
#include "../easy_image.h"


struct Mesh
{
    struct Material
    {
        Color ambient;
        Color diffuse;
        Color specular;
        double reflection;
    };

    Mesh() = default;
    Mesh(const std::vector<Vec3>& vertices, const std::vector<std::vector<uint32_t>>& indices, const Material& material)
    : vertices(vertices), indices(indices), material(material) {}

    Mesh& operator*=(const Mat4& transform);
    Mesh operator*(const Mat4& transform) const;

    static Mesh parseFigure(const ini::Section& section);
    static Mesh createLineDrawing(const Material& material, const ini::Section& section);

    static Mesh createCube(const Material& material);
    static Mesh createTetrahedron(const Material& material);
    static Mesh createOctahedron(const Material& material);
    static Mesh createIcosahedron(const Material& material);
    static Mesh createDodecahedron(const Material& material);

    static Mesh createCylinder(const Material& material, double height, uint32_t num, bool edges = true);
    static Mesh createCone(const Material& material, double height, uint32_t num);
    static Mesh createSphere(const Material& material, uint32_t depth);
    static Mesh createTorus(const Material& material, double R, double r, uint32_t n, uint32_t m);
    static Mesh createSierpinskiSphere(const Material& material, uint32_t depth);
    static Mesh createBuckyBall(const Material& material);
    static Mesh createMengerSponge(const Material& material, uint32_t depth);


    static void subdivTriangle(std::vector<Vec3>& vertices, std::vector<std::vector<uint32_t>>& indices, uint32_t face);
    static void subdivSierpinskiTriangle(std::vector<Vec3>& vertices, std::vector<std::vector<uint32_t>>& indices, uint32_t face);

    static std::vector<std::vector<uint32_t>> triangulate(const std::vector<std::vector<uint32_t>>& indices);
    static void makeFaceCCW(const std::vector<Vec3>& vertices, std::vector<uint32_t>& face);

    static Mesh generateFractal(const Mesh& mesh, uint32_t depth, double scale);

    static Mesh mergeMeshes(const std::vector<Mesh>& meshes);
    static Mesh mergeMengerSponge(const std::vector<Mesh> &meshes);

    static Mesh generateThickMesh(const Mesh& mesh, double r, uint32_t cylinderDepth, uint32_t sphereDepth, const Material& material);

    std::vector<Vec3> vertices;
    std::vector<std::vector<uint32_t>> indices;

    Material material;
};

#endif //ENGINE_MESH_H
