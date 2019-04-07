//============================================================================
// @name        : worldParser.h
// @author      : Thomas Dooms
// @date        : 4/6/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#pragma once

#include "hitable.h"
#include "../ini_configuration.h"
#include "sphere.h"

struct WorldParser
{
    static Material* parseMaterial(const ini::Section& section)
    {
        Material* material;

        const std::string type = section["type"];

        Vec3 emittance   = section["emittance"  ].as_double_tuple_or_die();
        Vec3 reflectance = section["reflectance"].as_double_tuple_or_die();

        if(type == "Lambertian")
        {
            material = new Lambertian{emittance, reflectance};
        }
        else if(type == "Metal")
        {
            double fuzzyness = 0.2;
            section["fuzzyness"].as_double_if_exists(fuzzyness);
            material = new Metal{emittance, reflectance, fuzzyness};
        }
        else
        {
            std::cerr << "unknown material type";
            exit(1);
        }
        return material;
    }
    static Hitable* parseFigure(const ini::Section& section, const std::vector<Material*> materials)
    {
        Hitable* hitable;
        const std::string type = section["type"];
        const uint32_t index = (int)section["material"];

        if(type == "Sphere")
        {
            Vec3 centre = section["centre"].as_double_tuple_or_die();
            double radius = section["radius"];

            hitable = new Sphere{centre, radius, materials[index]};
        }
        else
        {
            std::cerr << "unknown figure type";
            exit(1);
        }

        return hitable;
    }

    static std::pair<std::vector<Hitable*>, std::vector<Material*>> parseWorld(const ini::Configuration& configuration)
    {
        uint32_t numFigures   = (int)configuration["General"]["nrFigures"  ];
        uint32_t numMaterials = (int)configuration["General"]["nrMaterials"];

        std::pair<std::vector<Hitable*>, std::vector<Material*>> world;
        world.first .reserve(numFigures  );
        world.second.reserve(numMaterials);

        for(uint32_t i = 0; i < numMaterials; i++) world.second.push_back( parseMaterial(configuration["Material" + std::to_string(i)]              ));
        for(uint32_t i = 0; i < numFigures  ; i++) world.first.push_back ( parseFigure  (configuration["Figure"   + std::to_string(i)], world.second));
        return world;
    }
};



