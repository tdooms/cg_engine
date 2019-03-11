#include "easy_image.h"
#include "ini_configuration.h"
#include "l_parser.h"
#include "l_renderer.h"
#include "math/mat4.h"

#include "geometry/mesh.h"
#include "geometry/lines2D.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

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

img::EasyImage generate_wireframe(const ini::Configuration& configuration)
{
    const int size = configuration["General"]["size"];
    const std::vector<double> background = configuration["General"]["backgroundcolor"];

    std::vector<double> eyePos = configuration["General"]["eye"];
    Mat4 eyeSpace = Mat4::createEyeTransformationMatrix(eyePos[0], eyePos[1], eyePos[2]);

    uint32_t numFigures = static_cast<uint32_t>((int)configuration["General"]["nrFigures"]);

    std::vector<Mesh> figures(numFigures);
    for(uint32_t i = 0; i < numFigures; i++)
    {
        figures[i] =  Mesh::parseFigure(configuration["Figure" + std::to_string(i)], eyeSpace) ;
    }

    return drawFigures(figures, background, size, 1);
}

img::EasyImage generate_image(const ini::Configuration& configuration)
{
    const std::string type = configuration["General"]["type"];

    if     (type == "2DLSystem") return generate_L2D(configuration);
    else if(type == "Wireframe") return generate_wireframe(configuration);
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
