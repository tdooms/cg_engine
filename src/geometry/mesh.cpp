//============================================================================
// @name        : mesh.cpp
// @author      : Thomas Dooms
// @date        : 3/8/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "mesh.h"
#include <cmath>
#include <algorithm>
#include <fstream>
#include <unordered_set>
#include <assert.h>
#include "../l_parser.h"
#include "../l_renderer.h"

Mesh Mesh::parseFigure(const ini::Section& section, const Mat4& eye)
{
    Vec3 rotation    = {section["rotateX"], section["rotateY"], section["rotateZ"]};
    Vec3 translation = section["center"].as_double_tuple_or_die();
    double scale     = section["scale"];

    Material material;
    if(section["color"].exists())
    {
        material.ambient = section["color"].as_double_tuple_or_die();
    }
    else
    {
        material.ambient    = section["ambientReflection"    ].as_double_tuple_or_die();
        material.diffuse    = section["diffuseReflection"    ].as_double_tuple_or_default({0,0,0});
        material.specular   = section["specularReflection"   ].as_double_tuple_or_default({0,0,0});
        material.reflection = section["reflectionCoefficient"].as_double_or_default(0);
    }

    Mat4 transform = Mat4::createTotalTranslationMatrix(translation, scale, rotation * M_PI/180.0);

    std::string type = section["type"];
    Mesh result;

    if     (type == "LineDrawing" ) result = Mesh::createLineDrawing(material, section);
    else if(type == "Cube"        ) result = Mesh::createCube(material);
    else if(type == "Tetrahedron" ) result = Mesh::createTetrahedron(material);
    else if(type == "Octahedron"  ) result = Mesh::createOctahedron(material);
    else if(type == "Icosahedron" ) result = Mesh::createIcosahedron(material);
    else if(type == "Dodecahedron") result = Mesh::createDodecahedron(material);
    else if(type == "Cylinder"    )
    {
        double height = section["height"];
        uint32_t num = static_cast<uint32_t>((int)section["n"]);
        result = Mesh::createCylinder(material, height, num);
    }
    else if(type == "Cone"        )
    {
        double height = section["height"];
        uint32_t num = static_cast<uint32_t>((int)section["n"]);
        result = Mesh::createCone(material, height, num);
    }
    else if(type == "Sphere"      )
    {
        uint32_t depth = static_cast<uint32_t>((int)section["n"]);
        result = Mesh::createSphere(material, depth);
    }
    else if(type == "Torus"      )
    {
        double R = section["R"];
        double r = section["r"];
        uint32_t n = static_cast<uint32_t>((int)section["n"]);
        uint32_t m = static_cast<uint32_t>((int)section["m"]);
        result = Mesh::createTorus(material, R, r, n, m);
    }
    else if(type == "Sierpinski Sphere")
    {
        uint32_t depth = static_cast<uint32_t>((int)section["n"]);
        result = Mesh::createSierpinskiSphere(material, depth);
    }
    else if(type == "3DLSystem")
    {
        const std::string path = section["inputfile"];
        std::ifstream input_stream(path);
        if(!input_stream.is_open()) throw std::runtime_error("could not open the L3D file");

        LParser::LSystem3D l_system;
        input_stream >> l_system;
        input_stream.close();
        LSystem3DRenderer renderer = {l_system, material.ambient};
        result = renderer.generateMesh();
    }
    else if(type == "BuckyBall")
    {
        result = Mesh::createBuckyBall(material);
    }
    else if(type == "FractalCube")
    {
        double scale = section["fractalScale"];
        uint32_t depth = static_cast<uint32_t>((int)section["nrIterations"]);
        result = generateFractal(Mesh::createCube(material), depth, 1.0/scale);
    }
    else if(type == "FractalDodecahedron")
    {
        double scale = section["fractalScale"];
        uint32_t depth = static_cast<uint32_t>((int)section["nrIterations"]);
        result = generateFractal(Mesh::createDodecahedron(material), depth, 1.0/scale);
    }
    else if(type == "FractalIcosahedron")
    {
        double scale = section["fractalScale"];
        uint32_t depth = static_cast<uint32_t>((int)section["nrIterations"]);
        result = generateFractal(Mesh::createIcosahedron(material), depth, 1.0/scale);
    }
    else if(type == "FractalOctahedron")
    {
        double scale = section["fractalScale"];
        uint32_t depth = static_cast<uint32_t>((int)section["nrIterations"]);
        result = generateFractal(Mesh::createOctahedron(material), depth, 1.0/scale);
    }
    else if(type == "FractalTetrahedron")
    {
        double scale = section["fractalScale"];
        uint32_t depth = static_cast<uint32_t>((int)section["nrIterations"]);
        result = generateFractal(Mesh::createTetrahedron(material), depth, 1.0/scale);
    }
    else if(type == "FractalBuckyBall")
    {
        double scale = section["fractalScale"];
        uint32_t depth = static_cast<uint32_t>((int)section["nrIterations"]);
        result = generateFractal(Mesh::createBuckyBall(material), depth, 1.0/scale);
    }
    else if(type == "MengerSponge")
    {
        uint32_t depth = static_cast<uint32_t>((int)section["nrIterations"]);
        result = Mesh::createMengerSponge(material, depth);
    }
    else
    {
        std::cerr << "unrecognized type\n";
        exit(1);
    }

    result *= (transform*eye);
    return result;
}

Mesh& Mesh::operator*=(const Mat4& transform)
{
    for(Vec3& vertex : vertices) vertex *= transform;
    return *this;
}
Mesh Mesh::operator*(const Mat4& transform) const
{
    Mesh result = *this;
    for(Vec3& vertex : result.vertices) vertex *= transform;
    return result;
}

Mesh Mesh::createLineDrawing(const Material& material, const ini::Section& section)
{
    uint32_t numVertices = static_cast<uint32_t>((int)section["nrPoints"]);
    uint32_t numIndices  = static_cast<uint32_t>((int)section["nrLines" ]);

    std::vector<Vec3> vertices(numVertices);
    std::vector<std::vector<uint32_t>> indices(numIndices);

    for(uint32_t i = 0; i < numVertices; i++)
    {
        vertices[i] = section["point" + std::to_string(i)].as_double_tuple_or_die();
    }
    for(uint32_t i = 0; i < numIndices; i++)
    {
        const std::vector<int> point = section["line" + std::to_string(i)];
        indices[i] = { static_cast<uint32_t>(point[0]), static_cast<uint32_t>(point[1]) };
    }
    return {vertices, indices, material};
}

Mesh Mesh::createCube(const Material& material)
{
    const std::vector<Vec3> vertices = {{1,-1,-1}, {-1,1,-1}, {1,1,1}, {-1,-1,1}, {1,1,-1}, {-1,-1,-1}, {1,-1,1}, {-1,1,1}};
    const std::vector<std::vector<uint32_t>> indices = {{0,4,2,6}, {4,1,7,2}, {1,5,3,7}, {5,0,6,3}, {6,2,7,3}, {0,5,1,4}};
    return {vertices, indices, material};
}
Mesh Mesh::createTetrahedron(const Material& material)
{
    const std::vector<Vec3> vertices = {{1,-1,-1}, {-1,1,-1}, {1,1,1}, {-1,-1,1}};
    const std::vector<std::vector<uint32_t>> indices = {{0,1,2}, {1,3,2}, {0,3,1}, {0,2,3}};
    return {vertices, indices, material};
}
Mesh Mesh::createOctahedron(const Material& material)
{
    const std::vector<Vec3> vertices = {{1,0,0}, {0,1,0}, {-1,0,0}, {0,-1,0}, {0,0,-1}, {0,0,1}};
    const std::vector<std::vector<uint32_t>> indices = {{0,1,5} ,{1,2,5}, {2,3,5}, {3,0,5}, {1,0,4}, {2,1,4}, {3,2,4}, {0,3,4}};
    return {vertices, indices, material};
}
Mesh Mesh::createIcosahedron(const Material& material)
{
    std::vector<Vec3> vertices(12);
    vertices[0] = {0, 0, sqrt(5.0)/2.0};
    for(uint32_t i = 1; i < 6 ; i++) vertices[i] = { cos((i-1)*(M_PI*2.0) / 5.0)            , sin((i-1)*(M_PI*2.0) / 5.0)           ,  0.5 };
    for(uint32_t i = 6; i < 11; i++) vertices[i] = { cos( M_PI/5.0 + (i-6)*(M_PI*2.0) / 5.0), sin(M_PI/5.0 + (i-6)*(M_PI*2.0) / 5.0), -0.5 };
    vertices[11] = {0, 0, -sqrt(5.0)/2.0};

    const std::vector<std::vector<uint32_t>> indices ={{0,1,2}, {0,2,3}, {0,3,4 }, {0,4 ,5}, {0,5 ,1}, {1 ,6,2}, {2 ,6,7}, {2 ,7,3}, {3 ,7 ,8}, {3 ,8,4 },
                                                       {4,8,9}, {4,9,5}, {5,9,10}, {5,10,1}, {1,10,6}, {11,7,6}, {11,8,7}, {11,9,8}, {11,10,9}, {11,6,10}};
    return {vertices, indices, material};
}
Mesh Mesh::createDodecahedron(const Material& material)
{
    std::vector<Vec3> vertices(20);
    const Mesh icosahedron = Mesh::createIcosahedron(material);
    for(uint32_t i = 0; i < 20; i++)
    {
        vertices[i] = (icosahedron.vertices[ icosahedron.indices[i][0] ] + icosahedron.vertices[ icosahedron.indices[i][1] ] + icosahedron.vertices[ icosahedron.indices[i][2] ]) /3.0;
    }
    const std::vector<std::vector<uint32_t>> indices = {{0,1,2,3,4}     , {0,5,6,7,1}     , {1,7,8,9,2}     , {2,9,10,11,3} , {3,11,12,13,4}, {4,13,14,5,0},
                                                        {19,18,17,16,15}, {19,14,13,12,18}, {18,12,11,10,17}, {17,10,9,8,16}, {16,8,7,6,15} , {15,6,5,14,19}};
    return {vertices, indices, material};
}

Mesh Mesh::createCylinder(const Material& material, const double height, const uint32_t num)
{
    std::vector<Vec3> vertices(2*num);
    for(uint32_t i = 0; i < num; i++)
    {
        const double alpha = double(2.0*M_PI*i)/double(num);
        vertices[i    ] = { cos(alpha), sin(alpha), 0};
        vertices[num+i] = { cos(alpha), sin(alpha), height};
    }
    std::vector<std::vector<uint32_t>> indices(num+2);
    for(uint32_t i = 0; i < num-1; i++) indices[i] = {i, i+1, num+i+1, num+i};
    indices[num-1] = {num-1, 0, num, 2*num-1};

    indices[num].resize(num);
    indices[num+1].resize(num);
    std::generate(begin(indices[num  ]), end(indices[num  ]), [n = num-1] () mutable { return n--; });
    std::generate(begin(indices[num+1]), end(indices[num+1]), [n = num  ] () mutable { return n++; });

    return {vertices, indices, material};
}
Mesh Mesh::createCone(const Material& material, const double height, const uint32_t num)
{
    std::vector<Vec3> vertices(num+1);
    for(uint32_t i = 0; i < num+1; i++)
    {
        const double alpha = (2.0*M_PI*i)/double(num);
        vertices[i] = { cos(alpha), sin(alpha), 0};
    }
    vertices[num] = {0, 0, height};

    std::vector<std::vector<uint32_t>> indices(num+1);
    for(uint32_t i = 0; i < num-1; i++) indices[i] = {i, i+1, num};
    indices[num-1] = {num-1, 0, num};

    indices[num].resize(num);
    std::generate(begin(indices[num]), end(indices[num]), [n = num-1] () mutable { return n--; });

    return {vertices, indices, material};
}
Mesh Mesh::createSphere(const Material& material, const uint32_t depth)
{
    Mesh icosahedron = createIcosahedron(material);
    icosahedron.vertices.reserve(24*(uint32_t)pow(4, depth));   // every step quadruples the vertices
    icosahedron.indices.reserve( 20*(uint32_t)pow(4, depth));   // every step quadruples the faces

    for(uint32_t i = 0; i < depth; i++)
    {
        const auto size = static_cast<uint32_t>(icosahedron.indices.size());
        for(uint32_t j = 0; j < size; j++) subdivTriangle(icosahedron.vertices, icosahedron.indices, j);
    }
    for(Vec3& vertex : icosahedron.vertices) normalize(vertex);

    return icosahedron;
}

Mesh Mesh::createTorus(const Material& material, const double R, const double r, const uint32_t n, const uint32_t m)
{
    std::vector<Vec3> vertices(n*m);

    for(uint32_t i = 0; i < n; i++)
    {
        const double u = (2.0 * M_PI * i)/double(n);
        for(uint32_t j = 0; j < m; j++)
        {
            const double v = (2.0 * M_PI * j)/double(m);
            vertices[i*m+j] = {( R + r*cos(v) ) * cos(u), ( R + r*cos(v) ) * sin(u), r * sin(v)};
        }
    }

    std::vector<std::vector<uint32_t>> indices(n*m);
    for(uint32_t i = 0; i < n; i++)
        for(uint32_t j = 0; j < m; j++)
            indices[i*m + j] = {i*m+j, ((i+1)%n)*m + j, ((i+1)%n) * m + ((j+1)%m), i*m + ((j+1)%m)};

    return {vertices, indices, material};
}

Mesh Mesh::createSierpinskiSphere(const Material& material, uint32_t depth)
{
    Mesh icosahedron = createIcosahedron(material);
    icosahedron.vertices.reserve(12*(uint32_t)pow(2, depth));   // every step doubles the vertices
    icosahedron.indices.reserve( 20*(uint32_t)pow(2, depth));   // every step doubles the faces

    for(uint32_t i = 0; i < depth; i++)
    {
        const auto size = static_cast<uint32_t>(icosahedron.indices.size());
        for(uint32_t j = 0; j < size; j++) subdivSierpinskiTriangle(icosahedron.vertices, icosahedron.indices, j);
    }
    for(Vec3& vertex : icosahedron.vertices) vertex /= norm(vertex);

    return icosahedron;
}
Mesh Mesh::createBuckyBall(const Material& material)
{
    auto icosahedron = Mesh::createIcosahedron(material);
    std::map<Vec3, uint32_t> converter;

    Mesh result;
    result.vertices.reserve(60);
    result.indices.resize(32);
    result.material = material;

    uint32_t index = 0;
    uint32_t vecIndex = 0;
    for(const auto& face : icosahedron.indices)
    {
        for(uint32_t i = 0; i < 3; i++)
        {
            const Vec3 a = icosahedron.vertices[face[i]]*(2.0/3.0) + icosahedron.vertices[face[(i+1)%3]]*(1.0/3.0);
            const Vec3 b = icosahedron.vertices[face[i]]*(1.0/3.0) + icosahedron.vertices[face[(i+1)%3]]*(2.0/3.0);

            const auto iter1 = converter.find(a);
            if(iter1 == end(converter))
            {
                result.indices[vecIndex].push_back(index);
                result.indices[20 + face[i]].push_back(index);
                result.vertices.emplace_back(a);
                converter.emplace(a, index);

                index++;
            }
            else
            {
                result.indices[vecIndex].push_back((*iter1).second);
                result.indices[20 + face[i]].push_back((*iter1).second);
            }

            const auto iter2 = converter.find(b);
            if(iter2 == end(converter))
            {
                result.indices[vecIndex].push_back(index);
                converter.emplace(b, index);
                result.vertices.emplace_back(b);
                index++;
            }
            else result.indices[vecIndex].push_back((*iter2).second);
        }
        vecIndex++;
    }

    for(uint32_t i = 20; i < result.indices.size(); i++) makeFaceCCW(result.vertices, result.indices[i]);

    return result;
}

Mesh Mesh::createMengerSponge(const Material& material, uint32_t depth)
{
    const Mat4 scalar = Mat4::createScalarMatrix(pow(1.0/3.0, depth));
    const Mesh scaledCube = createCube(material) * scalar;
    const auto size = static_cast<uint32_t>(std::pow(20, depth));

    std::vector<Mesh> meshes = {size, scaledCube};
    std::vector<Vec3> diff = {{-1,-1, 1}, {0,-1, 1}, {1,-1, 1}, { 1,-1, 0}, {1,-1,-1}, {0,-1,-1}, {-1,-1, -1}, {-1,-1, 0},
                              {-1, 0, 1}, {1, 0, 1}, {1, 0,-1}, {-1, 0,-1},
                              {-1, 1, 1}, {0, 1, 1}, {1, 1, 1}, { 1, 1, 0}, {1, 1,-1}, {0, 1,-1}, {-1, 1, -1}, {-1, 1, 0}};

    for(Vec3& elem : diff) elem *= 2.0/3.0;

    const uint32_t vertSize = 20;
    for(uint32_t i = 0; i < size; i++)
    {
        uint32_t temp = i;
        for(uint32_t j = 0; j < depth; j++)
        {
            const Mat4 translation = Mat4::createTranslationMatrix(diff[temp % vertSize] * pow(1.0/3.0, j));
            meshes[i] *= translation;
            temp /= vertSize;
        }
    }
    //return mergeMeshes(meshes);
    return mergeMengerSponge(meshes);
}



void Mesh::subdivTriangle(std::vector<Vec3>& vertices, std::vector<std::vector<uint32_t>>& indices, const uint32_t face)
{
    const auto start = static_cast<uint32_t>(vertices.size());
    const Vec3 p0 = vertices[ indices[face][0] ];
    const Vec3 p1 = vertices[ indices[face][1] ];
    const Vec3 p2 = vertices[ indices[face][2] ];

    vertices.push_back((p0+p1)/2.0);
    vertices.push_back((p1+p2)/2.0);
    vertices.push_back((p2+p0)/2.0);

    indices.push_back({indices[face][0], start, start+2});
    indices.push_back({start, indices[face][1], start+1});
    indices.push_back({start+2, start+1, indices[face][2]});
    indices[face] = {start, start+1, start+2};
}

void Mesh::subdivSierpinskiTriangle(std::vector<Vec3>& vertices, std::vector<std::vector<uint32_t>>& indices, uint32_t face)
{
    const auto start = static_cast<uint32_t>(vertices.size());
    const Vec3 p0 = vertices[ indices[face][0] ];
    const Vec3 p1 = vertices[ indices[face][1] ];
    const Vec3 p2 = vertices[ indices[face][2] ];

    vertices.push_back((p0+p1)/2.0);
    vertices.push_back((p1+p2)/2.0);
    vertices.push_back((p2+p0)/2.0);

    indices.push_back({indices[face][0], start, start+2});
    indices.push_back({start, indices[face][1], start+1});
    indices[face] = {start+2, start+1, indices[face][2]};
}

std::vector<std::vector<uint32_t>> Mesh::triangulate(const std::vector<std::vector<uint32_t>>& indices)
{
    uint32_t newSize = 0;
    for(const auto& face : indices) newSize += face.size() - 2;

    std::vector<std::vector<uint32_t>> newIndices;
    newIndices.reserve(newSize);

    for(auto& face : indices)
    {
        if(face.size() == 3)
        {
            newIndices.push_back({face[0], face[1], face[2]});
            continue;
        }
        for(uint32_t i = 1; i < face.size() - 2; i++)
        {
            newIndices.push_back({face[0], face[i], face[i+1]});
        }
        newIndices.push_back({face[0], face[face.size()-2], face.back()});
    }
    return newIndices;
}

void Mesh::makeFaceCCW(const std::vector<Vec3>& vertices, std::vector<uint32_t>& face)
{
    Vec3 centre;
    for(const auto& index : face) centre += vertices[index];
    centre /= face.size();
    Vec3 normal = cross(vertices[face[0]] - centre, vertices[face[1]] - centre);
    if(norm(centre + normal) > norm(centre - normal)) normal = -normal;

    for(uint32_t i = 0; i < face.size()-1; i++)
    {
        bool sorted = true;
        for(uint32_t j = 0; j < face.size()-i-1; j++)
        {
            double res = dot(normal, cross(vertices[face[j]] - centre, vertices[face[j+1]] - centre));
            if(res > 0)
            {
                std::swap(face[j], face[j+1]);
                sorted = false;
            }
        }
        if(sorted) return;
    }

}

Mesh Mesh::generateFractal(const Mesh& mesh, const uint32_t depth, const double scale)
{
    auto vertSize = mesh.vertices.size();
    auto size = static_cast<uint32_t>(pow(vertSize, depth));

    Mesh scaled = mesh * Mat4::createScalarMatrix(pow(scale, depth));
    std::vector<Mesh> meshes = {size, scaled};

    Mesh smaller = mesh * Mat4::createScalarMatrix(scale);
    std::vector<Vec3> diff;
    diff.reserve(vertSize);

    for(uint32_t i = 0; i < vertSize; i++) diff[i] = mesh.vertices[i] - smaller.vertices[i];

    for(uint32_t i = 0; i < size; i++)
    {
        uint32_t temp = i;
        for(uint32_t j = 0; j < depth; j++)
        {
            const Mat4 translation = Mat4::createTranslationMatrix(diff[temp % vertSize] * pow(scale, j));
            meshes[i] *= translation;
            temp /= vertSize;
        }
    }
    return mergeMeshes(meshes);
}

Mesh Mesh::mergeMeshes(const std::vector<Mesh>& meshes)
{
    std::vector<Vec3> vertices;
    std::vector<std::vector<uint32_t>> indices;
    vertices.reserve(meshes[0].vertices.size() * meshes.size());
    indices.reserve(meshes[0].indices.size() * meshes.size());

    uint32_t offset = 0;
    for(const Mesh& mesh : meshes)
    {
        auto begin = end(indices);
        for(const Vec3& vec  : mesh.vertices) vertices.push_back(vec);
        for(const auto& face : mesh.indices)  indices.push_back(face);

        std::for_each(begin, end(indices), [offset](std::vector<uint32_t>& face){ for(uint32_t& index : face) index += offset; });
        offset += mesh.vertices.size();
    }
    return {vertices, indices, meshes[0].material};
}


struct Compare
{
    bool operator()(const std::vector<uint32_t>& a, const std::vector<uint32_t>& b) const noexcept
    {
        std::vector<uint32_t> temp0 = a;
        std::vector<uint32_t> temp1 = b;
        std::sort(begin(temp0), end(temp0));
        std::sort(begin(temp1), end(temp1));

        return temp0 < temp1;
    }
};

Mesh Mesh::mergeMengerSponge(const std::vector<Mesh> &meshes)
{
    std::map<Vec3, uint32_t> vertices;
    std::vector<uint32_t> converter;
    std::set<std::vector<uint32_t>, Compare> indices;

    converter.reserve(meshes[0].vertices.size() * meshes.size());

    for(const Mesh& mesh : meshes)
        for(const Vec3& vertex : mesh.vertices)
        {
            auto pair = vertices.emplace(vertex, vertices.size());

            if(pair.second) converter.push_back(vertices.size()-1);
            else converter.push_back(pair.first->second);
        }

    uint32_t offset = 0;
    for(const Mesh& mesh : meshes)
    {
        for(const auto& face : mesh.indices)
        {
            std::vector<uint32_t> temp;
            temp.reserve(face.size());
            for(const uint32_t& index : face) temp.push_back(converter[index+offset]);

            const auto iter = indices.find(temp);
            if(iter == end(indices)) indices.insert(temp);
            else indices.erase(iter);
        }
        offset += mesh.vertices.size();
    }

    Mesh result;
    result.vertices.resize(vertices.size());
    result.indices.reserve(indices.size());
    result.material = meshes[0].material;

    for(const auto& pair : vertices) result.vertices[pair.second] = pair.first;
    for(const auto& face : indices )
    {
        result.indices.push_back(face);
    }
    return result;
}
