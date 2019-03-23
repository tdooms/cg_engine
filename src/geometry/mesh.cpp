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
#include "../l_parser.h"
#include "../l_renderer.h"

Mesh Mesh::parseFigure(const ini::Section& section, const Mat4& eye)
{
    Vec3 rotation    = {section["rotateX"], section["rotateY"], section["rotateZ"]};
    Vec3 translation = section["center"].as_double_tuple_or_die();
    Vec3 color       = section["color"].as_double_tuple_or_die();
    double scale     = section["scale"];

    Mat4 transform = Mat4::createTotalTranslationMatrix(translation, scale, rotation * M_PI/180.0);

    std::string type = section["type"];
    Mesh result;

    if     (type == "LineDrawing" ) result = Mesh::createLineDrawing(color, section);
    else if(type == "Cube"        ) result = Mesh::createCube(color);
    else if(type == "Tetrahedron" ) result = Mesh::createTetrahedron(color);
    else if(type == "Octahedron"  ) result = Mesh::createOctahedron(color);
    else if(type == "Icosahedron" ) result = Mesh::createIcosahedron(color);
    else if(type == "Dodecahedron") result = Mesh::createDodecahedron(color);
    else if(type == "Cylinder"    )
    {
        double height = section["height"];
        uint32_t num = static_cast<uint32_t>((int)section["n"]);
        result = Mesh::createCylinder(color, height, num);
    }
    else if(type == "Cone"        )
    {
        double height = section["height"];
        uint32_t num = static_cast<uint32_t>((int)section["n"]);
        result = Mesh::createCone(color, height, num);
    }
    else if(type == "Sphere"      )
    {
        uint32_t depth = static_cast<uint32_t>((int)section["n"]);
        result = Mesh::createSphere(color, depth);
    }
    else if(type == "Torus"      )
    {
        double R = section["R"];
        double r = section["r"];
        uint32_t n = static_cast<uint32_t>((int)section["n"]);
        uint32_t m = static_cast<uint32_t>((int)section["m"]);
        result = Mesh::createTorus(color, R, r, n, m);
    }
    else if(type == "Sierpinski Sphere")
    {
        uint32_t depth = static_cast<uint32_t>((int)section["n"]);
        result = Mesh::createSierpinskiSphere(color, depth);
    }
    else if(type == "3DLSystem")
    {
        const std::string path = section["inputfile"];
        std::ifstream input_stream(path);
        if(!input_stream.is_open()) throw std::runtime_error("could not open the L3D file");

        LParser::LSystem3D l_system;
        input_stream >> l_system;
        input_stream.close();
        LSystem3DRenderer renderer = {l_system, color};
        result = renderer.generateMesh();
    }
    else throw std::runtime_error("unrecognized type");

    result *= (transform*eye);
    return result;
}

Mesh& Mesh::operator*=(const Mat4& transform)
{
    for(Vec3& vertex : vertices) vertex *= transform;
    return *this;
}

Mesh Mesh::createLineDrawing(const Color& color, const ini::Section& section)
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
    return {vertices, indices, color};
}

Mesh Mesh::createCube(const Color& color)
{
    const std::vector<Vec3> vertices = {{1,-1,-1}, {-1,1,-1}, {1,1,1}, {-1,-1,1}, {1,1,-1}, {-1,-1,-1}, {1,-1,1}, {-1,1,1}};
    const std::vector<std::vector<uint32_t>> indices = {{0,4,2,6}, {4,1,7,2}, {1,5,3,7}, {5,0,6,3}, {6,2,7,3}, {0,5,1,4}};
    return {vertices, indices, color};
}
Mesh Mesh::createTetrahedron(const Color& color)
{
    const std::vector<Vec3> vertices = {{1,-1,-1}, {-1,1,-1}, {1,1,1}, {-1,-1,1}};
    const std::vector<std::vector<uint32_t>> indices = {{0,1,2}, {1,3,2}, {0,3,1}, {0,2,3}};
    return {vertices, indices, color};
}
Mesh Mesh::createOctahedron(const Color& color)
{
    const std::vector<Vec3> vertices = {{1,0,0}, {0,1,0}, {-1,0,0}, {0,-1,0}, {0,0,-1}, {0,0,1}};
    const std::vector<std::vector<uint32_t>> indices = {{0,1,5} ,{1,2,5}, {2,3,5}, {3,0,5}, {1,0,4}, {2,1,4}, {3,2,4}, {0,3,4}};
    return {vertices, indices, color};
}
Mesh Mesh::createIcosahedron(const Color& color)
{
    std::vector<Vec3> vertices(12);
    vertices[0] = {0, 0, sqrt(5.0)/2.0};
    for(uint32_t i = 1; i < 6 ; i++) vertices[i] = { cos((i-1)*(M_PI*2.0) / 5.0)            , sin((i-1)*(M_PI*2.0) / 5.0)           ,  0.5 };
    for(uint32_t i = 6; i < 11; i++) vertices[i] = { cos( M_PI/5.0 + (i-6)*(M_PI*2.0) / 5.0), sin(M_PI/5.0 + (i-6)*(M_PI*2.0) / 5.0), -0.5 };
    vertices[11] = {0, 0, -sqrt(5.0)/2.0};

    const std::vector<std::vector<uint32_t>> indices ={{0,1,2}, {0,2,3}, {0,3,4 }, {0,4 ,5}, {0,5 ,1}, {1 ,6,2}, {2 ,6,7}, {2 ,7,3}, {3 ,7 ,8}, {3 ,8,4 },
                                                       {4,8,9}, {4,9,5}, {5,9,10}, {5,10,1}, {1,10,6}, {11,7,6}, {11,8,7}, {11,9,8}, {11,10,9}, {11,6,10}};
    return {vertices, indices, color};
}
Mesh Mesh::createDodecahedron(const Color& color)
{
    std::vector<Vec3> vertices(20);
    const Mesh icosahedron = Mesh::createIcosahedron(color);
    for(uint32_t i = 0; i < 20; i++)
    {
        vertices[i] = (icosahedron.vertices[ icosahedron.indices[i][0] ] + icosahedron.vertices[ icosahedron.indices[i][1] ] + icosahedron.vertices[ icosahedron.indices[i][2] ]) /3.0;
    }
    const std::vector<std::vector<uint32_t>> indices = {{0,1,2,3,4}, {0,5,6,7,1}, {1,7,8,9,2}, {2,9,10,11,3}, {3,11,12,13,4},{19,18,17,16,15},
                                                        {19,14,13,12,18}, {18,12,11,10,17}, {17,10,9,8,16}, {16,8,7,6,15}, {15,6,5,14,19}};
    return {vertices, indices, color};
}

Mesh Mesh::createCylinder(const Color& color, const double height, const uint32_t num)
{
    std::vector<Vec3> vertices(2*num);
    for(uint32_t i = 0; i < num; i++)
    {
        const double alpha = (2.0*M_PI*i)/double(num);
        vertices[i    ] = { cos(alpha), sin(alpha), 0};
        vertices[num+i] = { cos(alpha), sin(alpha), height};
    }
    std::vector<std::vector<uint32_t>> indices(num);
    for(uint32_t i = 0; i < num-1; i++) indices[i] = {i, i+1, num+i+1, num+i};
    indices[num-1] = {num-1, 0, num, 2*num-1};

    return {vertices, indices, color};
}
Mesh Mesh::createCone(const Color& color, const double height, const uint32_t num)
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

    indices[num  ].resize(num);
    std::generate(begin(indices[num]), end(indices[num]), [n = num-1] () mutable { return n--; });

    return {vertices, indices, color};
}
Mesh Mesh::createSphere(const Color& color, const uint32_t depth)
{
    Mesh icosahedron = createIcosahedron(color);
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

Mesh Mesh::createTorus(const Color& color, const double R, const double r, const uint32_t n, const uint32_t m)
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

    return {vertices, indices, color};
}

Mesh Mesh::createSierpinskiSphere(const Color &color, uint32_t depth)
{
    Mesh icosahedron = createIcosahedron(color);
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