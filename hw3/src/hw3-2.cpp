/**
 * @file hw3-2.cpp
 * @author sunfu-chou (sunfu-chou@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-11-17
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "bmp.h"
#include <cstdlib>
#include <filesystem>
#include <functional>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>

int main(int argc, char* argv[])
{
    std::filesystem::path input_filename;
    std::filesystem::path output_filename;
    std::string sequence;
    double contrast = 0.0;
    double hue = 0.0;
    double saturation = 0.0;
    double intensity = 0.0;
    double gamma = 0.0;
    double sharpness = 0.0;

    if (argc > 3)
    {
        input_filename = argv[1];
        output_filename = argv[2];
        for (int i = 3; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg == "--sequence" && i + 1 < argc)
            {
                sequence = argv[++i];
            }
            else if (arg == "--contrast" && i + 1 < argc)
            {
                contrast = std::atof(argv[++i]);
            }
            else if (arg == "--hue" && i + 1 < argc)
            {
                hue = std::atof(argv[++i]);
            }
            else if (arg == "--saturation" && i + 1 < argc)
            {
                saturation = std::atof(argv[++i]);
            }
            else if (arg == "--intensity" && i + 1 < argc)
            {
                intensity = std::atof(argv[++i]);
            }

            else if (arg == "--gamma" && i + 1 < argc)
            {
                gamma = std::atof(argv[++i]);
            }
            else if (arg == "--sharpness" && i + 1 < argc)
            {
                sharpness = std::atof(argv[++i]);
            }
            else
            {
                std::cerr << "Unknown option or missing value: " << arg << std::endl;
                return 1;
            }
        }
    }
    else
    {
        std::cerr << RED << "Usage: " << argv[0]
                  << " <input_file> <output_file> [--sequence <seq>] [--contrast <value>] [--intensity <value>] [--saturation <value>]"
                     " [--gamma <value>] [--sharpness <value>]"
                  << std::endl;
        return 1;
    }

    try
    {
        BMPImage image(input_filename);
        // Mapping of operations
        std::map<char, std::function<void()>> operations{
            { 'C', [&]() { image.adjustContrast(contrast); } },     { 'H', [&]() { image.adjustHue(hue); } },
            { 'S', [&]() { image.adjustSaturation(saturation); } }, { 'I', [&]() { image.adjustIntensity(intensity); } },
            { 'G', [&]() { image.applyGammaCorrection(gamma); } },  { 'A', [&]() { image.sharpen(sharpness); } }
        };

        // Execute operations in user-defined order
        for (char op : sequence)
        {
            auto it = operations.find(op);
            if (it != operations.end())
            {
                it->second();  // Execute the operation
            }
            else
            {
                std::cerr << "Invalid operation in sequence: " << op << std::endl;
                return 1;
            }
        }

        image.write(output_filename);
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Processing completed successfully!" << std::endl;
    return 0;
}
