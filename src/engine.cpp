#include "easy_image.h"
#include "ini_configuration.h"
#include "l_parser.h"
#include "l_renderer.h"
#include "math/mat4.h"
#include "geometry/mesh.h"

#include "raytracing/worldParser.h"
#include "raytracing/camera.h"
#include "math/vec3.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

Mat4 parseEyeSpace(const ini::Configuration& configuration)
{
    const std::vector<double> eyePos = configuration["General"]["eye"];
    return Mat4::createEyeTransformationMatrix(eyePos[0], eyePos[1], eyePos[2]);
}

std::vector<Mesh> parseFigures(const ini::Configuration& configuration)
{
    const uint32_t numFigures = static_cast<uint32_t>((int)configuration["General"]["nrFigures"]);

    std::vector<Mesh> figures(numFigures);
    for(uint32_t i = 0; i < numFigures; i++)
    {
        figures[i] =  Mesh::parseFigure(configuration["Figure" + std::to_string(i)]);
    }
    return figures;
}

std::pair<std::vector<Light>, std::vector<Light>> parseLights(const ini::Configuration& configuration, const std::vector<Mesh>& figures)
{
    const Mat4 eyeSpace = parseEyeSpace(configuration);
    Mat4 eyeRotation = parseEyeSpace(configuration);
    eyeRotation[14] = 0;

    bool shadows = configuration["General"]["shadowEnabled"].as_bool_or_default(false);
    uint32_t size = configuration["General"]["shadowMask"].as_int_or_default(0);

    const uint32_t numLights = static_cast<uint32_t>((int) configuration["General"]["nrLights"]);
    std::vector<Light> directional;
    std::vector<Light> point;

    for (uint32_t i = 0; i < numLights; i++)
    {
        std::vector<double> ambient = configuration["Light" +
                                                    std::to_string(i)]["ambientLight"].as_double_tuple_or_default({0, 0,
                                                                                                                   0});
        std::vector<double> diffuse = configuration["Light" +
                                                    std::to_string(i)]["diffuseLight"].as_double_tuple_or_default({0, 0,
                                                                                                                   0});
        std::vector<double> specular = configuration["Light" +
                                                     std::to_string(i)]["specularLight"].as_double_tuple_or_default({0,
                                                                                                                     0,
                                                                                                                     0});

        auto type = configuration["Light" + std::to_string(i)]["infinity"];
        if (type.exists() and type)
        {
            Vec3 direction = configuration["Light" + std::to_string(i)]["direction"].as_double_tuple_or_die();
            direction *= eyeRotation;
            directional.emplace_back(ambient, diffuse, specular, normalize(direction));
        } else if (type.exists() and not type)
        {
            Vec3 position = configuration["Light" + std::to_string(i)]["location"].as_double_tuple_or_die();
            if (shadows)
            {
                const Mat4 lightSpace = Mat4::createEyeTransformationMatrix(position[0], position[1], position[2]);
                const auto result = drawTriangulatedMeshes(figures, {}, size, {}, {}, lightSpace, true);
                const auto tuple = std::get<2>(result);
                //std::get<1>(result).toImage();
                point.emplace_back(ambient, diffuse, specular, position *= eyeSpace, lightSpace, std::get<1>(result), std::array<double, 3>{
                        std::get<0>(tuple), std::get<3>(tuple), std::get<4>(tuple)}, true);
            } else
            {
                position *= eyeSpace;
                point.emplace_back(ambient, diffuse, specular, position);
            }
        } else
        {
            point.emplace_back(ambient, diffuse, specular);
        }
    }
    return {directional, point};
}

img::EasyImage generate_L2D(const ini::Configuration& configuration)
{
    const int         size = configuration["General"]["size"];
    const std::string path = configuration["2DLSystem"]["inputfile"];

    const std::vector<double> linecolor  = configuration["2DLSystem"]["color"];
    const std::vector<double> background = configuration["General"]["backgroundcolor"];

    std::ifstream input_stream(path);
    if(!input_stream.is_open()) throw std::runtime_error("could not open the L2D file");

    LParser::LSystem2D l_system;
    input_stream >> l_system;
    input_stream.close();

    LSystem2DRenderer renderer = {l_system, linecolor};
    return renderer.generateImage(background, size);
}

img::EasyImage generate_wireframe(const ini::Configuration& configuration, bool depthBuffered)
{
    const int size = configuration["General"]["size"];
    const std::vector<double> background = configuration["General"]["backgroundcolor"];

    auto figures = parseFigures(configuration);
    return drawFigures(figures, background, size, 1, depthBuffered, parseEyeSpace(configuration));
}

img::EasyImage generate_mesh(const ini::Configuration& configuration)
{
    const int size = configuration["General"]["size"];
    const std::vector<double> background = configuration["General"]["backgroundcolor"];

    auto figures = parseFigures(configuration);
    return std::get<0>(drawTriangulatedMeshes(figures, background, size, {{{1,1,1}, {}, {}}}, {}, parseEyeSpace(configuration)));
}

img::EasyImage generate_lighting(const ini::Configuration& configuration)
{
    const int size = configuration["General"]["size"];
    const std::vector<double> background = configuration["General"]["backgroundcolor"];

    auto figures = parseFigures(configuration);
    auto lights = parseLights(configuration, figures);


    if(configuration["General"]["texture"].as_bool_or_default(false))
    {
        Texture texture = Texture::parseTexture(configuration["Texture"]);
        return std::get<0>(drawTriangulatedMeshes(figures, background, size, std::get<0>(lights), std::get<1>(lights), parseEyeSpace(configuration), false, &texture));
    }
    else
    {
        return std::get<0>(drawTriangulatedMeshes(figures, background, size, std::get<0>(lights), std::get<1>(lights), parseEyeSpace(configuration)));
    }


}

img::EasyImage generate_raytraced(const ini::Configuration& configuration)
{
    const uint32_t width   = (int)configuration["General"]["width"  ];
    const uint32_t height  = (int)configuration["General"]["height" ];
    const uint32_t samples = (int)configuration["General"]["samples"];
    const std::vector<double> ambient = configuration["General"]["ambient"];

    auto world = WorldParser::parseWorld(configuration);
    Camera camera = {width, height, ambient, 0.7, 5};

    for(uint32_t i = 0; i < samples; i++) camera.traceFilm(world.first);

    for(const auto& material : world.second) delete material;
    for(const auto& hitable  : world.first ) delete hitable ;

    return camera.exportFilm();
}

img::EasyImage generate_image(const ini::Configuration& configuration)
{
    const std::string type = configuration["General"]["type"];

    if     (type == "2DLSystem") return generate_L2D(configuration);
    else if(type == "Wireframe") return generate_wireframe(configuration, false);
    else if(type == "ZBufferedWireframe") return generate_wireframe(configuration, true);
    else if(type == "ZBuffering") return generate_mesh(configuration);
    else if(type == "RayTracing") return generate_raytraced(configuration);
    else if(type == "LightedZBuffering") return generate_lighting(configuration);
    else std::cerr << "unknown type\n";

    return img::EasyImage();
}

int main(int argc, char const* argv[])
{
    int retVal = 0;
    try
    {
        for(int i = 1; i < argc; ++i)
        {
            ini::Configuration conf;
            try
            {
                std::ifstream fin(argv[i]);
                fin >> conf;
                fin.close();
            }
            catch(ini::ParseException& ex)
            {
                std::cerr << "Error parsing file: " << argv[i] << ": " << ex.what() << std::endl;
                retVal = 1;
                continue;
            }

            img::EasyImage image = generate_image(conf);
            if(image.get_height() > 0 && image.get_width() > 0)
            {
                std::string fileName(argv[i]);
                std::string::size_type pos = fileName.rfind('.');
                if(pos == std::string::npos)
                {
                    //filename does not contain a '.' --> append a '.bmp' suffix
                    fileName += ".bmp";
                }
                else
                {
                    fileName = fileName.substr(0,pos) + ".bmp";
                }
                try
                {
                    std::ofstream f_out(fileName.c_str(),std::ios::trunc | std::ios::out | std::ios::binary);
                    f_out << image;

                }
                catch(std::exception& ex)
                {
                    std::cerr << "Failed to write image to file: " << ex.what() << std::endl;
                    retVal = 1;
                }
            }
            else
            {
                std::cout << "Could not generate image for " << argv[i] << std::endl;
            }
        }
    }
    catch(const std::bad_alloc &exception)
    {
        //When you run out of memory this exception is thrown. When this happens the return value of the program MUST be '100'.
        //Basically this return value tells our automated test scripts to run your engine on a pc with more memory.
        //(Unless of course you are already consuming the maximum allowed amount of memory)
        //If your engine does NOT adhere to this requirement you risk losing points because then our scripts will
        //mark the test as failed while in reality it just needed a bit more memory
        std::cerr << "Error: insufficient memory" << std::endl;
        retVal = 100;
    }
    return retVal;
}
